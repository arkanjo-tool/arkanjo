include(FetchContent)

function(setup_libgit2)
    message(STATUS "Configuring libgit2 via FetchContent...")

    FetchContent_Declare(
        libgit2
        GIT_REPOSITORY https://github.com/libgit2/libgit2.git
        GIT_TAG        v1.9.1
    )

    set(BUILD_TESTS OFF CACHE BOOL "Disable libgit2 tests" FORCE)
    set(BUILD_CLI OFF CACHE BOOL "Disable libgit2 CLI" FORCE)
    set(USE_SSH OFF CACHE BOOL "Disable SSH support in libgit2" FORCE)
    set(USE_HTTP_PARSER builtin CACHE STRING "Use builtin HTTP parser" FORCE)
    set(USE_HTTPS OFF CACHE STRING "Disable HTTPS support" FORCE)
    set(USE_BUNDLED_ZLIB ON CACHE BOOL "Use bundled zlib" FORCE)

    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build libgit2 as static" FORCE)

    FetchContent_MakeAvailable(libgit2)

    if(TARGET libgit2)
        message(STATUS "libgit2 target successfully configured!")
    else()
        message(FATAL_ERROR "Failed to configure libgit2: 'libgit2' target NOT found.")
    endif()
endfunction()

setup_libgit2()


# macOS needs explicit iconv linkage for libgit2
if(APPLE)
    find_library(ICONV_LIBRARY iconv)

    if(ICONV_LIBRARY)
        message(STATUS "Found iconv: ${ICONV_LIBRARY}")
    else()
        message(FATAL_ERROR "iconv library not found")
    endif()
endif()