set(ARKANJO_INCLUDE_DIRS
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third-party>
    $<INSTALL_INTERFACE:include>
)

#
file(GLOB_RECURSE CORE_BASE_SOURCES
    src/base/*.cpp
)
add_library(core_base ${CORE_BASE_SOURCES})
target_include_directories(core_base
    PUBLIC ${ARKANJO_INCLUDE_DIRS}
)

#
add_library(core_utils src/utils/utils.cpp)
if(WIN32)
    target_sources(core_utils PRIVATE src/utils/windows_utils.cpp)
elseif(APPLE)
    target_sources(core_utils PRIVATE src/utils/posix_utils.cpp)
else(UNIX)
    target_sources(core_utils PRIVATE src/utils/posix_utils.cpp)
endif()
target_include_directories(core_utils PUBLIC ${ARKANJO_INCLUDE_DIRS})
target_link_libraries(core_utils PUBLIC core_base)

#
file(GLOB_RECURSE CORE_COMMANDS_SOURCES
    src/commands/*.cpp
)
add_library(core_commands ${CORE_COMMANDS_SOURCES})
target_sources(core_commands PRIVATE ${GENERATED_FILE})
target_include_directories(core_commands PUBLIC ${ARKANJO_INCLUDE_DIRS})
target_link_libraries(core_commands PUBLIC core_base core_utils ${PARSER_LIBS} tree_sitter_core)

#
file(GLOB_RECURSE CORE_CLI_SOURCES
    src/cli/*.cpp
)
add_library(core_cli ${CORE_CLI_SOURCES})
target_include_directories(core_cli PUBLIC ${ARKANJO_INCLUDE_DIRS})
target_link_libraries(core_cli PUBLIC core_base core_utils)

#
add_executable(arkanjo
    src/main.cpp
)
target_link_libraries(arkanjo PRIVATE core_base core_utils core_commands core_cli)

add_executable(arkanjo-preprocessor
    src/commands/pre/preprocessor_main.cpp
)
target_link_libraries(arkanjo-preprocessor PRIVATE core_base core_utils core_commands core_cli)

target_link_libraries(core_base PUBLIC project_options)
target_link_libraries(core_utils PUBLIC project_options)
target_link_libraries(core_commands PUBLIC project_options)
target_link_libraries(core_cli PUBLIC project_options)
target_link_libraries(arkanjo PRIVATE project_options)
target_link_libraries(arkanjo-preprocessor PRIVATE project_options)

add_custom_target(copy_third_party ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/third-party
        ${THIRD_PARTY_BUILD_DIR}
)

add_dependencies(arkanjo copy_third_party)
add_dependencies(arkanjo-preprocessor copy_third_party)