add_library(core_commands)

file(GLOB_RECURSE CORE_COMMANDS_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

target_sources(core_commands PRIVATE
    command.cpp
    ${CORE_COMMANDS_SOURCES}
)

target_sources(core_commands PRIVATE ${GENERATED_FILE})

target_include_directories(core_commands
    PUBLIC
        ${ARKANJO_PUBLIC_INCLUDE_DIRS}
    PRIVATE
        ${ARKANJO_PRIVATE_INCLUDE_DIRS}
)

target_link_libraries(core_commands PRIVATE 
    core_base 
    core_utils 
    core_parser 
    core_methods
    core_git
)
target_link_libraries(core_commands PRIVATE ${PARSER_LIBS} PUBLIC tree_sitter_core)
target_link_libraries(core_commands PRIVATE project_options)
