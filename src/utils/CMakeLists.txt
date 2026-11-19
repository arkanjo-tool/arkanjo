target_sources(core_base PRIVATE
    utils.cpp
)

if(WIN32)
    target_sources(core_base PRIVATE windows_utils.cpp)
elseif(APPLE)
    target_sources(core_base PRIVATE posix_utils.cpp)
else(UNIX)
    target_sources(core_base PRIVATE posix_utils.cpp)
endif()

