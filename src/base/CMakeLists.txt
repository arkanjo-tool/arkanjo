add_library(core_base)

target_sources(core_base PRIVATE
    path.cpp
    preprocess_state.cpp
    similarity_table.cpp

    config/config.cpp
)

add_subdirectory(function)
add_subdirectory(storage)

target_include_directories(core_base
    PUBLIC
        ${ARKANJO_PUBLIC_INCLUDE_DIRS}
    PRIVATE
        ${ARKANJO_PRIVATE_INCLUDE_DIRS}
)

target_link_libraries(core_base PRIVATE project_options)
