set(ARKANJO_PUBLIC_INCLUDE_DIRS
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set(ARKANJO_PRIVATE_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/third-party
)

if(ARKANJO_LIBGIT2_SYSTEM)
    set(ARKANJO_LIBGIT2_LIBS git2)
else()
    set(ARKANJO_LIBGIT2_LIBS
        libgit2
        util
        xdiff
        llhttp
        pcre2
        zlib
    )
endif()

add_executable(arkanjo)

target_include_directories(arkanjo
    PUBLIC
        ${ARKANJO_PUBLIC_INCLUDE_DIRS}
    PRIVATE
        ${ARKANJO_PRIVATE_INCLUDE_DIRS}
)

target_link_libraries(arkanjo PRIVATE core_base core_utils core_commands core_cli core_git)

add_executable(arkanjo-preprocessor
    src/commands/pre/preprocessor_main.cpp
)
target_include_directories(arkanjo-preprocessor
    PUBLIC
        ${ARKANJO_PUBLIC_INCLUDE_DIRS}
    PRIVATE
        ${ARKANJO_PRIVATE_INCLUDE_DIRS}
)
target_link_libraries(arkanjo-preprocessor PRIVATE core_base core_utils core_commands core_cli core_git)

target_link_libraries(arkanjo PRIVATE project_options)
target_link_libraries(arkanjo-preprocessor PRIVATE project_options)

add_custom_target(copy_third_party ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/third-party
        ${THIRD_PARTY_BUILD_DIR}
)

add_dependencies(arkanjo copy_third_party)
add_dependencies(arkanjo-preprocessor copy_third_party)