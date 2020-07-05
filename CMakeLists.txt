cmake_minimum_required(VERSION 3.15)

project(DrunkardsWalk)

add_subdirectory(src)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "DrunkardsWalk")
set_property(TARGET DrunkardsWalk PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
