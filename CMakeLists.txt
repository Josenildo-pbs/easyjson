cmake_minimum_required(VERSION 3.10)

# Define the project name and languages
project(easyjson C)

# Set the C standard
set(CMAKE_C_STANDARD 99)  
set(CMAKE_C_STANDARD_REQUIRED True)

# Source files
set(SRC_FILES "src/main.c" "src/core.c" "src/lexer.c" "src/parser.c" "src/interface.c")

# Create the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Specify include directories for the target
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Specify compiler options for the target
target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 -Wall -Wextra)
