cmake_minimum_required(VERSION 3.15)
project(arkanjo 
    LANGUAGES C CXX
)

option(ENABLE_WARNINGS "Enable compiler warnings" ON)
option(ENABLE_TESTS "Build tests" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

include(GNUInstallDirs)

set(THIRD_PARTY_BUILD_DIR "${CMAKE_BINARY_DIR}/third-party")

include(cmake/TreeSitter.cmake)
include(cmake/CompilerOptions.cmake)
include(cmake/Targets.cmake)

if(ENABLE_TESTS)
    enable_testing()

    add_executable(arkanjo-test
        tests/e2e/test.cpp
    )
    target_link_libraries(arkanjo-test PRIVATE 
        core_base core_utils core_commands core_cli project_options
    )

    add_test(NAME run_tests COMMAND arkanjo-test)
    set_tests_properties(run_tests PROPERTIES
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

install(TARGETS arkanjo arkanjo-preprocessor
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
#install(DIRECTORY include/
#    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#)
install(DIRECTORY third-party/
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/arkanjo/third-party
)

include(cmake/Formatter.cmake)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")