cmake_minimum_required(VERSION 3.24)
project(suite_teste_unitario)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(
    ${CMAKE_SOURCE_DIR}/3rd_party/gtest
    ${CMAKE_CURRENT_BINARY_DIR}/gtest
)

add_executable(
    suite_teste_unitario
    "trabalho1_test.cpp"
)

target_link_libraries(suite_teste_unitario gtest_main)
target_include_directories(suite_teste_unitario PRIVATE ${FW_SOURCE_DIR})

install(
    TARGETS suite_teste_unitario
    RUNTIME DESTINATION "."
)
