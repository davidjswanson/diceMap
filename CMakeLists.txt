project(diceMap)

cmake_minimum_required(VERSION 3.6)	#Need 3.6 to set Visual Studio Startup Project

add_executable(main main.cpp)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)

include_directories(${CMAKE_SOURCE_DIR})

# Libraries
add_subdirectory(external/NanoGUI/)
option(NANOGUI_BUILD_EXAMPLE "Build NanoGUI example application?" OFF)
option(NANOGUI_BUILD_PYTHON  "Build a Python plugin for NanoGUI?" OFF)
