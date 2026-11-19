add_library(core_parser)

target_sources(core_parser PRIVATE
    feature_extractor.cpp
    tree_sitter_parser.cpp
)

target_include_directories(core_parser
    PUBLIC
        ${ARKANJO_PUBLIC_INCLUDE_DIRS}
    PRIVATE
        ${ARKANJO_PRIVATE_INCLUDE_DIRS}
)

target_sources(core_parser PRIVATE ${GENERATED_FILE})
target_link_libraries(core_parser PRIVATE ${PARSER_LIBS} PUBLIC tree_sitter_core)
target_link_libraries(core_parser PRIVATE project_options)