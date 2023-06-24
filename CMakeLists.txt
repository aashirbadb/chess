cmake_minimum_required(VERSION 3.26.4)

project(Chess)


file (GLOB source_files "src/*.cpp")
add_executable(${PROJECT_NAME} ${source_files})