add_library(core_methods)

file(GLOB_RECURSE CORE_METHODS_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

target_sources(core_methods PRIVATE
    method.cpp
    ${CORE_METHODS_SOURCES}
)

target_include_directories(core_methods
    PUBLIC
        ${ARKANJO_PUBLIC_INCLUDE_DIRS}
    PRIVATE
        ${ARKANJO_PRIVATE_INCLUDE_DIRS}
)

target_sources(core_methods PRIVATE ${GENERATED_FILE})
target_link_libraries(core_methods PRIVATE core_base core_cli)
target_link_libraries(core_methods PRIVATE ${PARSER_LIBS} PUBLIC tree_sitter_core)
target_link_libraries(core_methods PRIVATE project_options)