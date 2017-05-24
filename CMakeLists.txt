cmake_minimum_required(VERSION 3.6)
project(PatchMatch)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED system)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(SOURCE_FILES main.cpp Bitmap.cpp Bitmap.h PatchMatch.cpp PatchMatch.h NNF.cpp NNF.h)

add_executable(PatchMatch ${SOURCE_FILES})
target_link_libraries(PatchMatch ${OpenCV_LIBS})
target_link_libraries(PatchMatch ${Boost_LIBRARIES})

target_include_directories(PatchMatch PUBLIC ${Boost_INCLUDE_DIRS})