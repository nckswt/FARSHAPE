cmake_minimum_required( VERSION 2.8 )
project( objectRecognition )
find_package( OpenCV REQUIRED )
add_library( camera camera/camera.cpp camera/camera.h )
add_executable( objectRecognition main.cpp )
include_directories(${CMAKE_SOURCE_DIR}/camera )
target_link_libraries( camera ${OpenCV_LIBS} )
target_link_libraries( objectRecognition camera)
