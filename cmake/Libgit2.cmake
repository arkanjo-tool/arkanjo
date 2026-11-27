include(FetchContent)

option(ARKANJO_USE_SYSTEM_LIBGIT2
    "Use system-installed libgit2 if available" ON)

set(ARKANJO_LIBGIT2_VERSION "v1.9.7")

function(setup_libgit2)
    if(ARKANJO_USE_SYSTEM_LIBGIT2)
        find_package(libgit2 CONFIG QUIET)
        if(libgit2_FOUND AND TARGET git2)
            message(STATUS "libgit2: system install (CMake config)")

            set(ARKANJO_LIBGIT2_TARGET git2 PARENT_SCOPE)
            set(ARKANJO_LIBGIT2_SYSTEM TRUE PARENT_SCOPE)
            return()
        endif()

        find_package(PkgConfig QUIET)
        if(PkgConfig_FOUND)
            pkg_check_modules(LIBGIT2_PC QUIET IMPORTED_TARGET GLOBAL libgit2)
            if(LIBGIT2_PC_FOUND)
                message(STATUS "libgit2: system install ${LIBGIT2_PC_VERSION} (pkg-config)")

                set(ARKANJO_LIBGIT2_TARGET PkgConfig::LIBGIT2_PC PARENT_SCOPE)
                set(ARKANJO_LIBGIT2_SYSTEM TRUE PARENT_SCOPE)
                return()
            endif()
        endif()
    endif()

    message(STATUS "libgit2: not found on system, fetching")

    FetchContent_Declare(
        libgit2
        GIT_REPOSITORY https://github.com/libgit2/libgit2.git
        GIT_TAG        "${ARKANJO_LIBGIT2_VERSION}"
        GIT_SHALLOW     TRUE
    )

    # libgit2 uses an old cmake_minimum_required: these policies make
    # option()/set(CACHE) honor the normal variables set below
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0126 NEW)

    set(BUILD_TESTS      OFF)
    set(BUILD_CLI        OFF)
    set(USE_SSH          OFF)
    set(USE_HTTP_PARSER  builtin)
    set(USE_HTTPS        OFF)
    set(USE_BUNDLED_ZLIB ON)
    set(REGEX_BACKEND    builtin)

    set(BUILD_SHARED_LIBS OFF)

    message(CHECK_START "Fetching libgit2 @ ${ARKANJO_LIBGIT2_VERSION}")
    FetchContent_MakeAvailable(libgit2)

    if(NOT TARGET libgit2package)
        message(CHECK_FAIL "failed")
        message(FATAL_ERROR
            "libgit2: target 'libgit2package' not found after fetching "
            "${ARKANJO_LIBGIT2_VERSION}. The target name may have changed "
            "in this libgit2 version.")
    endif()
    message(CHECK_PASS "done")

    set(ARKANJO_LIBGIT2_TARGET libgit2package PARENT_SCOPE)
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