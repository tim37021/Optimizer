cmake_minimum_required(VERSION 3.4)

project(HW2)

file(GLOB HEADER *.h)
file(GLOB SOURCE *.cpp)

add_executable(HW2 ${SOURCE} ${HEADER})