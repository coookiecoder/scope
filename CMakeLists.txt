cmake_minimum_required(VERSION 3.28)
project(Scope)

set(CMAKE_CXX_STANDARD 23)

set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
set(SFML_BUILD_GRAPHICS FALSE)

message(STATUS "Fetching SFML")

include(FetchContent)
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 3.0.0 GIT_SHALLOW ON EXCLUDE_FROM_ALL SYSTEM)
FetchContent_MakeAvailable(SFML)

message(STATUS "Fetching SFML done")
message(STATUS "SFML include directory :" ${sfml_SOURCE_DIR})

add_executable(Scope main.cpp class/Obj.cpp class/Vector.cpp include/Obj.hpp
        class/VulkanApplication.cpp
        include/VulkanApplication.hpp
        template/Matrix.tpp)

find_package(Vulkan REQUIRED)
find_package(X11 REQUIRED)

target_include_directories(Scope PRIVATE ${glm_SOURCE_DIR} ${sfml_SOURCE_DIR})

target_link_libraries(Scope PRIVATE SFML::Window Vulkan::Vulkan X11)
