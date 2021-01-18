cmake_minimum_required(VERSION 3.15)
project(PipeIdentify)

set(CMAKE_CXX_STANDARD 14)

set(OpenCV_DIR  D:\\opencv\\mingwBuild\\install)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
aux_source_directory(code CODESRC)
add_executable(opencvTest ${CODESRC})


set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
target_link_libraries(opencvTest ${OpenCV_LIBS})