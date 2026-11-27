add_library(core_git)

target_sources(core_git PRIVATE
    repository.cpp
    text_diff.cpp
)

target_include_directories(core_git
    PUBLIC
        ${ARKANJO_PUBLIC_INCLUDE_DIRS}
        ${libgit2_SOURCE_DIR}/include
    PRIVATE
        ${ARKANJO_PRIVATE_INCLUDE_DIRS}
)

target_link_libraries(core_git PRIVATE core_cli)
target_link_libraries(core_git PUBLIC ${ARKANJO_LIBGIT2_TARGET})
target_link_libraries(core_git PRIVATE project_options)

if(APPLE)
    target_link_libraries(core_git PRIVATE ${ICONV_LIBRARY})
endif()
