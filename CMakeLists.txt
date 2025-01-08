cmake_minimum_required(VERSION 3.10)
project(PhysicsSimulation)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Link directories
link_directories(lib)

# Add source files
add_executable(PhysicsSimulation src/main.cpp src/glad.c)

# Link libraries
target_link_libraries(PhysicsSimulation glfw3 opengl32)

# Specify console application
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-subsystem,console")
