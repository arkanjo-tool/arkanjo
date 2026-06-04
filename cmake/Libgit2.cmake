include(FetchContent)
option(ARKANJO_USE_SYSTEM_LIBGIT2
    "Use system-installed libgit2 if available"
    ON
)

function(setup_libgit2)

    if(ARKANJO_USE_SYSTEM_LIBGIT2)
        find_package(libgit2 CONFIG QUIET)

        if(libgit2_FOUND)
            message(STATUS "Using system-installed libgit2")

            set(ARKANJO_LIBGIT2_TARGET git2 PARENT_SCOPE)
            set(ARKANJO_LIBGIT2_SYSTEM TRUE PARENT_SCOPE)

            return()
        endif()
    endif()

    message(STATUS "System libgit2 not found, using FetchContent")

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

    if(NOT TARGET libgit2)
        message(FATAL_ERROR "Failed to configure libgit2")
    endif()

    set(ARKANJO_LIBGIT2_TARGET libgit2 PARENT_SCOPE)
    set(ARKANJO_LIBGIT2_SYSTEM FALSE PARENT_SCOPE)

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