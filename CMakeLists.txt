cmake_minimum_required(VERSION 3.26.4)

project(Chess)

file(GLOB source_files "src/headers/*.h")
file(GLOB source_files "src/*.cpp")

add_executable(${PROJECT_NAME} ${source_files})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lSDL2 -lGL -lSDL2_image -lSDL2_mixer -lSDL2_ttf")