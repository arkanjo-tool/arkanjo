set(THIRD_PARTY_DIR "" CACHE PATH "Override for third party directory")

add_library(project_options INTERFACE)

target_include_directories(project_options
    SYSTEM INTERFACE
    third-party
)

target_compile_definitions(project_options INTERFACE 
    THIRD_PARTY_DIR="${CMAKE_INSTALL_FULL_LIBDIR}/arkanjo/third-party"
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

target_compile_options(project_options INTERFACE
    # MSVC
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Zi /Od>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/O2>

    # GCC / Clang
    $<$<AND:$<NOT:$<C_COMPILER_ID:MSVC>>,$<COMPILE_LANGUAGE:C>,$<CONFIG:Debug>>:-O3>
    $<$<AND:$<NOT:$<C_COMPILER_ID:MSVC>>,$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Release>>:-O3>
)
if(ENABLE_WARNINGS)
    target_compile_options(project_options INTERFACE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
    )
endif()