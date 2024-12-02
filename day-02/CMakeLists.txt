cmake_minimum_required(VERSION 3.15)

project(aoc-2024-day-02)

# C++ version
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


add_executable(day-02 day-02.cpp)

# add_custom_command(
#     TARGET ALL POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy
#             ${CMAKE_CURRENT_SOURCE_DIR}/data
#             ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# message(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
# message(TARGET_FILE_DIR:{PROJECT_NAME}="$<TARGET_FILE_DIR:${PROJECT_NAME}>")

# add_custom_target(zoo ALL)

# add_custom_command(
#   TARGET zoo POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy_directory 
#     ${CMAKE_CURRENT_SOURCE_DIR}/data
#     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # add_custom_command(
    #   TARGET zoo POST_BUILD
    #   COMMAND ${CMAKE_COMMAND} -E copy_directory 
    #     ${CMAKE_CURRENT_SOURCE_DIR}/data
    #     $<TARGET_FILE_DIR:${zoo}>)
