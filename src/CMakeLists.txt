include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(common)
add_subdirectory(console)
if (GUI)
    add_subdirectory(gui)
endif ()
