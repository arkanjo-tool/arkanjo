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

    set(TS_DIR "${THIRD_PARTY_BUILD_DIR}/tree-sitter")

    if(NOT EXISTS "${TS_DIR}")
        message(STATUS "Cloning tree-sitter core...")
        execute_process(
            COMMAND git clone https://github.com/tree-sitter/tree-sitter.git ${TS_DIR}
        )
    endif()

    set(PARSERS_DIR 
        "${THIRD_PARTY_BUILD_DIR}/parsers"
    )
    file(MAKE_DIRECTORY ${PARSERS_DIR})

    file(READ "${CMAKE_SOURCE_DIR}/config.json" CONFIG_JSON)
    string(JSON LANGS_LENGTH LENGTH "${CONFIG_JSON}" 
        languages)
    math(EXPR STOP_INDEX "${LANGS_LENGTH} - 1")

    string(TIMESTAMP t0 "%s")

    foreach(IDX RANGE ${STOP_INDEX})
        string(JSON LANG GET "${CONFIG_JSON}" "languages" ${IDX})
        set(PARSER_DIR "${PARSERS_DIR}/tree-sitter-${LANG}")

        if(NOT EXISTS "${PARSER_DIR}")
            message(STATUS "Cloning parser ${LANG}...")
            execute_process(COMMAND git clone https://github.com/tree-sitter/tree-sitter-${LANG}.git ${PARSER_DIR})
        endif()

        set(GRAMMAR_DIR "${PARSER_DIR}")

        # Check if language has a custom grammar directory
        string(JSON HAS_GRAMMAR_DIR ERROR_VARIABLE GRAMMAR_ERROR
            GET "${CONFIG_JSON}" "grammar_dir" "${LANG}"
        )

        if(NOT GRAMMAR_ERROR)
            set(GRAMMAR_DIR "${PARSER_DIR}/${HAS_GRAMMAR_DIR}")
        endif()

        if(EXISTS "${GRAMMAR_DIR}/src/parser.c")
            set(PARSER_SOURCES ${GRAMMAR_DIR}/src/parser.c)

            if(EXISTS "${GRAMMAR_DIR}/src/scanner.c")
                list(APPEND PARSER_SOURCES ${GRAMMAR_DIR}/src/scanner.c)
            endif()

            add_library(tree_sitter_${LANG} STATIC ${PARSER_SOURCES})

            target_include_directories(tree_sitter_${LANG} PRIVATE ${GRAMMAR_DIR}/src)
            list(APPEND PARSER_LIBS tree_sitter_${LANG})
        else()
            message(WARNING "Language parser ${LANG} not found or incomplete")
        endif()
    endforeach()

    string(TIMESTAMP t1 "%s")
    math(EXPR elapsed "${t1} - ${t0}")

    message(STATUS "Cloning parsers done (${elapsed}s)")

    set(GENERATED_FILE "${CMAKE_BINARY_DIR}/generated_parsers.cpp")

    file(WRITE ${GENERATED_FILE} "#include <unordered_map>\n")
    file(APPEND ${GENERATED_FILE} "#include <string>\n")
    file(APPEND ${GENERATED_FILE} "#include <tree_sitter/api.h>\n\n")

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

    add_library(tree_sitter_core STATIC
        "${THIRD_PARTY_BUILD_DIR}/tree-sitter/lib/src/lib.c"
    )

    target_include_directories(tree_sitter_core PUBLIC
        "${THIRD_PARTY_BUILD_DIR}/tree-sitter/lib/include"
    )

    file(APPEND ${GENERATED_FILE}
    "std::unordered_map<std::string, std::string> get_extension_map() {\n")
    file(APPEND ${GENERATED_FILE} "    return {\n")

    foreach(IDX RANGE ${STOP_INDEX})
        string(JSON LANG GET "${CONFIG_JSON}" "languages" ${IDX})

        string(JSON EXTS GET "${CONFIG_JSON}" "extensions" ${LANG})

        string(JSON EXT_LEN LENGTH "${CONFIG_JSON}" "extensions" ${LANG})
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