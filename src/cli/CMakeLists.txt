add_library(core_cli)

target_sources(core_cli PRIVATE
    parser_options.cpp

    formatter/theme.cpp
)

target_include_directories(core_cli
    PUBLIC
        ${ARKANJO_PUBLIC_INCLUDE_DIRS}
    PRIVATE
        ${ARKANJO_PRIVATE_INCLUDE_DIRS}
)

target_link_libraries(core_cli PRIVATE core_base)
target_link_libraries(core_cli PRIVATE project_options)
