cmake_minimum_required(VERSION 3.0)

project(HackComputer)

include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(HackComputer ${SOURCES})

target_link_options(HackComputer PRIVATE -pthread)
target_compile_options(HackComputer PRIVATE -g)
