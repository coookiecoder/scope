cmake_minimum_required(VERSION 3.22)
project(Scope)

set(CMAKE_CXX_STANDARD 20)

add_executable(Scope main.cpp class/Obj.cpp class/Vector.cpp include/Obj.hpp)
