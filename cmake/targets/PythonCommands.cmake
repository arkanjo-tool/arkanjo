# Python command auto-discovery
# Gera executáveis: arkanjo-<command>

set(COMMANDS_DIR "${CMAKE_SOURCE_DIR}/src/commands")

file(GLOB children RELATIVE ${COMMANDS_DIR} ${COMMANDS_DIR}/*)

foreach(child IN LISTS children)

    set(init_file "${COMMANDS_DIR}/${child}/__init__.py")

    if(EXISTS "${init_file}")

        set(wrapper "${CMAKE_BINARY_DIR}/arkanjo-${child}")

        file(WRITE "${wrapper}"
"#!/usr/bin/env python3
import sys
import runpy
from pathlib import Path

BASE = Path(__file__).resolve().parent / \"../share/arkanjo/commands\"

sys.path.insert(0, str(BASE))

runpy.run_module('${child}.__init__', run_name='__main__')
")

        file(CHMOD "${wrapper}"
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                        GROUP_READ GROUP_EXECUTE
                        WORLD_READ WORLD_EXECUTE)

        install(PROGRAMS "${wrapper}" DESTINATION bin)

        install(
            DIRECTORY "${COMMANDS_DIR}/${child}"
            DESTINATION "share/arkanjo/commands"
        )

    endif()

endforeach()
