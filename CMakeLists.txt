# Version
cmake_minimum_required(VERSION 2.8)

# Project name
project(prord)

# OpenCV
find_package(OpenCV REQUIRED)
if(!OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Please install OpenCV library.")
endif(!OpenCV_FOUND)

# Working tree configuration
set(EXECUTABLE_OUTPUT_PATH bin/)
include_directories(inc/prord inc/skeleton)
add_definitions(-Wall -Wextra -Wunused -O2)

# Programs
file(GLOB prord_src src/prord/* inc/prord/*)
add_executable(prord ${prord_src})
target_link_libraries(prord ${OpenCV_LIBS})

file(GLOB skeleton_src src/skeleton/* inc/skeleton/*)
add_executable(skeleton ${skeleton_src})
target_link_libraries(skeleton ${OpenCV_LIBS})
