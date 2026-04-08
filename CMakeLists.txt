cmake_minimum_required(VERSION 3.15)
project(arkanjo 
    LANGUAGES CXX
)

option(ENABLE_WARNINGS "Enable compiler warnings" ON)
option(ENABLE_TESTS "Build tests" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()
set(THIRD_PARTY_DIR "" CACHE PATH "Override for third party directory")

include(GNUInstallDirs)


set(ARKANJO_INCLUDE_DIRS
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third-party>
    $<INSTALL_INTERFACE:include>
)

file(GLOB_RECURSE CORE_BASE_SOURCES
    src/base/*.cpp
)
add_library(core_base ${CORE_BASE_SOURCES})
target_include_directories(core_base
    PUBLIC ${ARKANJO_INCLUDE_DIRS}
)

add_library(core_utils
    src/utils/utils.cpp
)

if(WIN32)
    target_sources(core_utils PRIVATE src/utils/windows_utils.cpp)
elseif(APPLE)
    target_sources(core_utils PRIVATE src/utils/posix_utils.cpp)
else(UNIX)
    target_sources(core_utils PRIVATE src/utils/posix_utils.cpp)
endif()

target_include_directories(core_utils PUBLIC ${ARKANJO_INCLUDE_DIRS})
target_link_libraries(core_utils PUBLIC core_base)

file(GLOB_RECURSE CORE_COMMANDS_SOURCES
    src/commands/*.cpp
)
add_library(core_commands ${CORE_COMMANDS_SOURCES})
target_include_directories(core_commands PUBLIC ${ARKANJO_INCLUDE_DIRS})
target_link_libraries(core_commands PUBLIC core_base core_utils)

file(GLOB_RECURSE CORE_CLI_SOURCES
    src/cli/*.cpp
)
add_library(core_cli ${CORE_CLI_SOURCES})
target_include_directories(core_cli PUBLIC ${ARKANJO_INCLUDE_DIRS})
target_link_libraries(core_cli PUBLIC core_base core_utils)

add_executable(arkanjo
    src/main.cpp
)
target_link_libraries(arkanjo PRIVATE core_base core_utils core_commands core_cli)

add_executable(arkanjo-preprocessor
    src/commands/pre/preprocessor_main.cpp
)
target_link_libraries(arkanjo-preprocessor PRIVATE core_base core_utils core_commands core_cli)

add_library(project_options INTERFACE)

target_include_directories(project_options
    SYSTEM INTERFACE
    third-party
)

set(THIRD_PARTY_BUILD_DIR
    "${CMAKE_BINARY_DIR}/third-party"
)

add_custom_target(copy_third_party ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/third-party
        ${THIRD_PARTY_BUILD_DIR}
)

target_compile_definitions(project_options INTERFACE 
    THIRD_PARTY_DIR="${CMAKE_INSTALL_FULL_LIBDIR}/arkanjo/third-party"
)

target_compile_definitions(project_options INTERFACE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)
target_compile_options(project_options INTERFACE
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Zi /Od>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/O2>

    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Debug>>:-g3 -O0>
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Release>>:-O3>
)
if(ENABLE_WARNINGS)
    target_compile_options(project_options INTERFACE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
    )
endif()

target_link_libraries(core_base PUBLIC project_options)
target_link_libraries(core_utils PUBLIC project_options)
target_link_libraries(core_commands PUBLIC project_options)
target_link_libraries(core_cli PUBLIC project_options)
target_link_libraries(arkanjo PRIVATE project_options)
target_link_libraries(arkanjo-preprocessor PRIVATE project_options)

if(ENABLE_TESTS)
    enable_testing()

    add_executable(arkanjo-test
        tests/e2e/test.cpp
    )
    target_link_libraries(arkanjo-test PRIVATE 
        core_base core_utils core_commands core_cli project_options
    )

    add_test(NAME run_tests COMMAND arkanjo-test)
    set_tests_properties(run_tests PROPERTIES
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

install(TARGETS arkanjo arkanjo-preprocessor
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
#install(DIRECTORY include/
#    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#)
install(DIRECTORY third-party/
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/arkanjo/third-party
)

# ============
# clang-format

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

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")