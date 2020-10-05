cmake_minimum_required(VERSION 3.16)
project(HelloWorld)

set(CMAKE_CXX_STANDARD 14)
add_executable(HelloWorld main.cpp)
find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})


target_link_libraries(  HelloWorld ${OpenCV_LIBS}  )
