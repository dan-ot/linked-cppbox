cmake_minimum_required(VERSION 3.28.3)
project(hello)
file(GLOB HELLO "*.cpp")
add_executable(hello ${HELLO})
target_link_libraries(hello PUBLIC Crow::Crow Threads::Threads)
add_custom_command(
    TARGET hello POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/public/
        ${CMAKE_CURRENT_BINARY_DIR}/public)