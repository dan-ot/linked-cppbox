cmake_minimum_required(VERSION 3.28.3)
project(hello)
file(GLOB HELLO "*.cpp")
add_executable(hello ${HELLO})
target_link_libraries(hello PUBLIC Crow::Crow Threads::Threads ${MONGO_LIBRARIES})
add_custom_command(
    TARGET hello POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/public/
        ${CMAKE_CURRENT_BINARY_DIR}/public)
add_custom_command(
    TARGET hello POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/templates/
        ${CMAKE_CURRENT_BINARY_DIR}/templates)