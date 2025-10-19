cmake_minimum_required(VERSION 3.15)

project(mn1 VERSION 1.0 LANGUAGES CXX)

set(
    CMAKE_INSTALL_PREFIX
    "${CMAKE_BINARY_DIR}/exeobj_cmake"
    CACHE PATH "Installation directory" FORCE
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    trabalho1
    trabalho1/main.cpp
)
install(TARGETS trabalho1 RUNTIME DESTINATION ".")
