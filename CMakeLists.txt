cmake_minimum_required(VERSION 3.10)
project(PhysicsSimulation)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Ensure consistent runtime libraries
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")


# Include directories for headers
include_directories(include)

# Add source files
add_executable(PhysicsSimulation src/main.cpp src/glad.c)

# Add library search paths
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Link GLFW and OpenGL libraries
target_link_libraries(PhysicsSimulation "C:/Users/jfire/Desktop/Physics Simulation (C++)/lib/glfw3.lib" opengl32)
