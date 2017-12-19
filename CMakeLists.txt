project(diceMap)

cmake_minimum_required(VERSION 3.6)	#Need 3.6 to set Visual Studio Startup Project


## Add NanoGUI to Project
# Disable building extras we won't need (pure C++ project)
set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE)
set(NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE)

# When using Visual Studio, building as a shared library caused problem
# with nanogui.dll not being found, turning this option off resolved issue
set(NANOGUI_BUILD_SHARED  OFF CACHE BOOL " " FORCE)

# Add the configurations from nanogui
# https://github.com/darrenmothersele/nanogui-test/blob/master/CMakeLists.txt
add_subdirectory(external/nanogui/)
include_directories(external/nanogui/include)
include_directories(${NANOGUI_EXTRA_INCS})
add_definitions(${NANOGUI_EXTRA_DEFS})

# For reliability of parallel build, make the NanoGUI targets dependencies
set_property(TARGET glfw glfw_objects nanogui PROPERTY FOLDER "dependencies")

# Project specific library
include_directories(lib/)


add_executable(main main.cpp)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
target_link_libraries(main nanogui ${NANOGUI_EXTRA_LIBS})
