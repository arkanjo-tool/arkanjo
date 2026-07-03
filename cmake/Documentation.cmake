find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(DOXYGEN_INPUT ${CMAKE_BINARY_DIR}/Doxyfile)

    configure_file(
        ${CMAKE_SOURCE_DIR}/Doxyfile
        ${DOXYGEN_INPUT}
        @ONLY
    )

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()
