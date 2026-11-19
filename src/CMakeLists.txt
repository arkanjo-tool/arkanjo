target_sources(arkanjo PRIVATE
    main.cpp
)

add_subdirectory(base)
add_subdirectory(cli)
add_subdirectory(commands)
add_subdirectory(git)
add_subdirectory(methods)
add_subdirectory(parser)
add_subdirectory(utils)
