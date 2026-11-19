add_library(core_utils)

target_sources(core_utils PRIVATE
    utils.cpp
)

if(WIN32)
    target_sources(core_utils PRIVATE windows_utils.cpp)
elseif(APPLE)
    target_sources(core_utils PRIVATE posix_utils.cpp)
else(UNIX)
    target_sources(core_utils PRIVATE posix_utils.cpp)
endif()

target_include_directories(core_utils
    PUBLIC
        ${ARKANJO_PUBLIC_INCLUDE_DIRS}
    PRIVATE
        ${ARKANJO_PRIVATE_INCLUDE_DIRS}
)

target_link_libraries(core_utils PRIVATE core_base)
target_link_libraries(core_utils PRIVATE project_options)
