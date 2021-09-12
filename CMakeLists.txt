cmake_minimum_required(VERSION 3.0)

project(HackComputer)
#target_include_directories(test PRIVATE .)

include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp")
#set(SOURCES Main.cpp Wiring.cpp Chip.cpp Gates.cpp Bitstream.cpp
add_executable(HackComputer ${SOURCES})

