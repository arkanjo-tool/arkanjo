include(FetchContent)

option(ARKANJO_USE_SYSTEM_TREE_SITTER
    "Use system-installed tree-sitter if available" ON)

set(ARKANJO_TREE_SITTER_VERSION "v0.27.2")

function(find_system_tree_sitter OUT_TARGET OUT_INCLUDE_DIR)
    set(${OUT_TARGET} "" PARENT_SCOPE)
    set(${OUT_INCLUDE_DIR} "" PARENT_SCOPE)

    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(TS_PC QUIET IMPORTED_TARGET GLOBAL tree-sitter)
        if(TS_PC_FOUND)
            message(STATUS "tree-sitter core: system install ${TS_PC_VERSION}")
            find_path(_ts_inc tree_sitter/api.h HINTS ${TS_PC_INCLUDE_DIRS})
            set(${OUT_TARGET} PkgConfig::TS_PC PARENT_SCOPE)
            set(${OUT_INCLUDE_DIR} "${_ts_inc}" PARENT_SCOPE)
        endif()
    endif()
endfunction()

function(parser_abi_ok API_HEADER PARSER_C OUT)
    file(STRINGS "${API_HEADER}" _max REGEX "^#define TREE_SITTER_LANGUAGE_VERSION [0-9]+")
    file(STRINGS "${API_HEADER}" _min REGEX "^#define TREE_SITTER_MIN_COMPATIBLE_LANGUAGE_VERSION [0-9]+")
    file(STRINGS "${PARSER_C}"   _abi REGEX "^#define LANGUAGE_VERSION [0-9]+")
    string(REGEX MATCH "[0-9]+$" _max "${_max}")
    string(REGEX MATCH "[0-9]+$" _min "${_min}")
    string(REGEX MATCH "[0-9]+$" _abi "${_abi}")

    set(${OUT} TRUE PARENT_SCOPE)
    if(_abi AND _max AND _min AND (_abi GREATER _max OR _abi LESS _min))
        set(${OUT} FALSE PARENT_SCOPE)
    endif()
endfunction()

function(fetch_source NAME URL OUT_DIR)
    cmake_parse_arguments(ARG "" "TAG;LABEL" "" ${ARGN})

    if(NOT ARG_LABEL)
        set(ARG_LABEL "${NAME}")
    endif()

    set(_what "${ARG_LABEL}")
    if(ARG_TAG)
        string(APPEND _what " @ ${ARG_TAG}")
    endif()

    if(EXISTS "${OUT_DIR}/.git")
        message(STATUS "${_what}: already present, skipping")
        return()
    endif()

    set(_tag_args "")
    if(ARG_TAG)
        set(_tag_args GIT_TAG ${ARG_TAG})
    endif()

    message(CHECK_START "Fetching ${_what}")

    FetchContent_Declare(${NAME}
        GIT_REPOSITORY ${URL}
        ${_tag_args}
        GIT_SHALLOW    TRUE
        SOURCE_DIR     "${OUT_DIR}"
        SOURCE_SUBDIR  __no_cmake__
    )
    FetchContent_MakeAvailable(${NAME})

    message(CHECK_PASS "done")
endfunction()

function(get_language_symbol LANG OUT_SYMBOL)
    set(SYMBOL "tree_sitter_${LANG}")

    string(JSON GRAMMAR_DIR ERROR_VARIABLE GRAMMAR_ERROR
        GET "${CONFIG_JSON}" "grammar_dir" "${LANG}"
    )

    if(NOT GRAMMAR_ERROR)
        string(REPLACE "/" "_" GRAMMAR_SYMBOL "${GRAMMAR_DIR}")
        set(SYMBOL "tree_sitter_${GRAMMAR_SYMBOL}")
    endif()

    set(${OUT_SYMBOL} ${SYMBOL} PARENT_SCOPE)
endfunction()

function(setup_tree_sitter OUT_LIBS OUT_GENERATED_FILE)
    set(CONFIG_FILE "${CMAKE_SOURCE_DIR}/config.json")
    if(NOT EXISTS "${CONFIG_FILE}")
        message(FATAL_ERROR "tree-sitter: config file not found: ${CONFIG_FILE}")
    endif()

    set(TS_SYSTEM_TARGET "")
    set(TS_SYSTEM_INCLUDE "")
    set(TS_ABI_MISMATCH "")
    if(ARKANJO_USE_SYSTEM_TREE_SITTER)
        find_system_tree_sitter(TS_SYSTEM_TARGET TS_SYSTEM_INCLUDE)
    endif()

    set(PARSERS_DIR  "${THIRD_PARTY_BUILD_DIR}/parsers")
    file(MAKE_DIRECTORY ${PARSERS_DIR})

    file(READ "${CMAKE_SOURCE_DIR}/config.json" CONFIG_JSON)
    string(JSON LANGS_LENGTH LENGTH "${CONFIG_JSON}" languages)
    math(EXPR STOP_INDEX "${LANGS_LENGTH} - 1")

    set(PARSER_LIBS "")
    set(SKIPPED_LANGS "")

    string(TIMESTAMP t0 "%s")

    foreach(IDX RANGE ${STOP_INDEX})
        math(EXPR N "${IDX} + 1")
        string(JSON LANG GET "${CONFIG_JSON}" "languages" ${IDX})
        set(PARSER_DIR "${PARSERS_DIR}/tree-sitter-${LANG}")

        string(REPLACE "-" "_" LANG_ID "${LANG}")
        fetch_source(
            ts_parser_${LANG_ID}
            https://github.com/tree-sitter/tree-sitter-${LANG}.git
            "${PARSER_DIR}"
            LABEL "parser ${LANG} [${N}/${LANGS_LENGTH}]"
        )

        set(GRAMMAR_DIR "${PARSER_DIR}")

        # Check if language has a custom grammar directory
        string(JSON HAS_GRAMMAR_DIR ERROR_VARIABLE GRAMMAR_ERROR
            GET "${CONFIG_JSON}" "grammar_dir" "${LANG}"
        )

        if(NOT GRAMMAR_ERROR)
            set(GRAMMAR_DIR "${PARSER_DIR}/${HAS_GRAMMAR_DIR}")
        endif()

        if(EXISTS "${GRAMMAR_DIR}/src/parser.c")
            if(TS_SYSTEM_TARGET AND TS_SYSTEM_INCLUDE)
                parser_abi_ok(
                    "${TS_SYSTEM_INCLUDE}/tree_sitter/api.h"
                    "${GRAMMAR_DIR}/src/parser.c" _abi_ok)
                if(NOT _abi_ok)
                    list(APPEND TS_ABI_MISMATCH ${LANG})
                endif()
            endif()

            set(PARSER_SOURCES ${GRAMMAR_DIR}/src/parser.c)

            if(EXISTS "${GRAMMAR_DIR}/src/scanner.c")
                list(APPEND PARSER_SOURCES ${GRAMMAR_DIR}/src/scanner.c)
            endif()

            add_library(tree_sitter_${LANG} STATIC ${PARSER_SOURCES})
            target_include_directories(tree_sitter_${LANG} PRIVATE ${GRAMMAR_DIR}/src)

            list(APPEND PARSER_LIBS tree_sitter_${LANG})
        else()
            message(WARNING
                "tree-sitter: parser '${LANG}' skipped, "
                "'${GRAMMAR_DIR}/src/parser.c' not found. "
                "Check 'languages' and 'grammar_dir' in config.json.")
            list(APPEND SKIPPED_LANGS ${LANG})
        endif()
    endforeach()

    if(TS_SYSTEM_TARGET AND NOT TS_ABI_MISMATCH)
        message(STATUS "tree-sitter core: using system install")
        add_library(tree_sitter_core INTERFACE)
        target_link_libraries(tree_sitter_core INTERFACE ${TS_SYSTEM_TARGET})
    else()
        if(TS_ABI_MISMATCH)
            list(JOIN TS_ABI_MISMATCH ", " _mm)
            message(STATUS
                "tree-sitter core: system version does not support the ABI of: "
                "${_mm}; fetching ${ARKANJO_TREE_SITTER_VERSION}")
        else()
            message(STATUS "tree-sitter core: not found on system, fetching")
        endif()

        set(TS_DIR "${THIRD_PARTY_BUILD_DIR}/tree-sitter")
        fetch_source(
            tree_sitter_core_src
            https://github.com/tree-sitter/tree-sitter.git
            "${TS_DIR}"
            TAG "${ARKANJO_TREE_SITTER_VERSION}"
            LABEL "tree-sitter core"
        )
        add_library(tree_sitter_core STATIC "${TS_DIR}/lib/src/lib.c")
        target_include_directories(tree_sitter_core PUBLIC "${TS_DIR}/lib/include")
    endif()

    string(TIMESTAMP t1 "%s")
    math(EXPR elapsed "${t1} - ${t0}")

    list(LENGTH PARSER_LIBS OK_COUNT)
    message(STATUS "tree-sitter: ${OK_COUNT}/${LANGS_LENGTH} parsers configured (${elapsed}s)")

    if(SKIPPED_LANGS)
        list(JOIN SKIPPED_LANGS ", " _skipped)
        message(WARNING "tree-sitter: skipped languages: ${_skipped}")
    endif()

    set(GENERATED_FILE "${CMAKE_BINARY_DIR}/generated_parsers.cpp")

    file(WRITE ${GENERATED_FILE}
        "// Generated by CMake from config.json. Do not edit.\n"
        "#include <unordered_map>\n"
        "#include <string>\n"
        "#include <tree_sitter/api.h>\n\n")

    # externs
    foreach(IDX RANGE ${STOP_INDEX})
        string(JSON LANG GET "${CONFIG_JSON}" "languages" ${IDX})

        get_language_symbol(${LANG} LANG_SYMBOL)

        if(TARGET tree_sitter_${LANG})
            file(APPEND ${GENERATED_FILE}
                "extern \"C\" TSLanguage *${LANG_SYMBOL}();\n")
        endif()
    endforeach()

    file(APPEND ${GENERATED_FILE} "\nstd::unordered_map<std::string, TSLanguage* (*)()> get_language_map() {\n")
    file(APPEND ${GENERATED_FILE} "    return {\n")

    foreach(IDX RANGE ${STOP_INDEX})
        string(JSON LANG GET "${CONFIG_JSON}" "languages" ${IDX})

        get_language_symbol(${LANG} LANG_SYMBOL)

        if(TARGET tree_sitter_${LANG})
            file(APPEND ${GENERATED_FILE}
                "        {\"${LANG}\", ${LANG_SYMBOL}},\n")
        endif()
    endforeach()

    file(APPEND ${GENERATED_FILE} "    };\n}\n")

    file(APPEND ${GENERATED_FILE}
    "std::unordered_map<std::string, std::string> get_extension_map() {\n")
    file(APPEND ${GENERATED_FILE} "    return {\n")

    foreach(IDX RANGE ${STOP_INDEX})
        string(JSON LANG GET "${CONFIG_JSON}" "languages" ${IDX})

        string(JSON EXT_LEN ERROR_VARIABLE EXT_ERROR
            LENGTH "${CONFIG_JSON}" "extensions" ${LANG})
        if(EXT_ERROR)
            message(FATAL_ERROR
                "tree-sitter: no 'extensions' entry for '${LANG}' in config.json")
        endif()
        math(EXPR EXT_STOP "${EXT_LEN} - 1")

        foreach(EIDX RANGE ${EXT_STOP})
            string(JSON EXT GET "${CONFIG_JSON}" "extensions" ${LANG} ${EIDX})

            file(APPEND ${GENERATED_FILE}
                "        {\"${EXT}\", \"${LANG}\"},\n")
        endforeach()
    endforeach()

    file(APPEND ${GENERATED_FILE} "    };\n}\n")

    set(${OUT_LIBS} ${PARSER_LIBS} PARENT_SCOPE)
    set(${OUT_GENERATED_FILE} ${GENERATED_FILE} PARENT_SCOPE)
endfunction()

setup_tree_sitter(PARSER_LIBS GENERATED_FILE)