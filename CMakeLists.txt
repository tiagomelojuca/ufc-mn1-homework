cmake_minimum_required(VERSION 3.15)

project(mn1 VERSION 1.0 LANGUAGES CXX)

option(BUILD_UNIT_TESTS "Construir testes unitarios com framework do gtest" ON)
set(FW_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(
    CMAKE_INSTALL_PREFIX
    "${CMAKE_BINARY_DIR}/exeobj_cmake"
    CACHE PATH "Installation directory" FORCE
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (BUILD_UNIT_TESTS)
    add_subdirectory(test)
endif()

add_executable(
    trabalho1
    src/trabalho1/main.cpp
)
install(TARGETS trabalho1 RUNTIME DESTINATION ".")
