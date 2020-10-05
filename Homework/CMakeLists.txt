cmake_minimum_required( VERSION 3.16 )
project( Homework )

set( CMAKE_CXX_STANDARD 17 )
find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( Homework FindApple.cpp)
target_link_libraries( Homework ${OpenCV_LIBS} )