file(GLOB_RECURSE CLANG_FORMAT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.hpp
    ${CMAKE_SOURCE_DIR}/include/*.hpp
)

find_program(CLANG_FORMAT_EXECUTABLE clang-format)

if(CLANG_FORMAT_EXECUTABLE)
    add_custom_target(clang_format_check
        COMMAND ${CMAKE_COMMAND} -E echo "Checking code format..."
        COMMAND ${CMAKE_COMMAND} -E env 
                ${CLANG_FORMAT_EXECUTABLE} --dry-run --Werror -style=file ${CLANG_FORMAT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-format check (no files will be modified)"
        VERBATIM
    )
else()
    message(STATUS "clang-format not found, skipping clang_format_check target")
endif()

find_program(CLANG_TIDY_EXECUTABLE clang-tidy)

if(CLANG_TIDY_EXECUTABLE)
    add_custom_target(clang_tidy_check
        COMMAND ${CMAKE_COMMAND} -E echo "Running clang-tidy..."

        COMMAND ${CLANG_TIDY_EXECUTABLE}
            -p=${CMAKE_BINARY_DIR}
            ${CLANG_FORMAT_SOURCES}

        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy (unused includes check)"
        VERBATIM
    )
else()
    message(STATUS "clang-tidy not found, skipping clang_tidy_check target")
endif()