cmake_minimum_required(VERSION 3.4)

project(p1)

find_package(PythonLibs 3.6 REQUIRED)

set (CMAKE_CXX_STANDARD 17)
file(GLOB HEADER optimizer.h)
file(GLOB SOURCE optimizer.cpp)

option(ENABLE_PLOT "Plot or not" OFF)
if(ENABLE_PLOT)
add_definitions(-DPLOT)
set(SOURCE ${SOURCE} cppplt.cpp)
set(HEADER ${SOURCE} cppplt.h)
endif()

add_executable(p1 p1.cpp ${SOURCE} ${HEADER})
target_include_directories(p1 PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(p1 ${PYTHON_LIBRARIES})

project(p2)
add_executable(p2 p2.cpp ${SOURCE} ${HEADER})
message(INFO ${PYTHON_LIBRARIES})
target_include_directories(p2 PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(p2 ${PYTHON_LIBRARIES})