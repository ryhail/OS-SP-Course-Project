cmake_minimum_required(VERSION 3.27)
project(sfml)

set(CMAKE_CXX_STANDARD 17)
find_package(SFML 2.6.1 COMPONENTS graphics audio REQUIRED)
add_executable(game main.cpp)
target_link_libraries(game sfml-graphics sfml-audio)