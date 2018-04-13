cmake_minimum_required(VERSION 3.4)

project(p1)

file(GLOB HEADER optimizer.h)
file(GLOB SOURCE optimizer.cpp)

add_executable(p1 p1.cpp ${SOURCE} ${HEADER})

project(p2)
add_executable(p2 p2.cpp ${SOURCE} ${HEADER})
