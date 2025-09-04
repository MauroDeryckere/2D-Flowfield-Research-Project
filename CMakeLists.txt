cmake_minimum_required(VERSION 3.21)
project(FlowFields LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Engine)
add_subdirectory(Flowfields)