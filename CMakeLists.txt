cmake_minimum_required(VERSION 3.4)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Libraries/depthai-core)

project(OakD)

message([STATUS] " BUILDING " ${PROJECT_NAME})

find_package(OpenCV REQUIRED)

add_library(OakD Source/OakD.cpp)

# include_directories(Include/)
# target_link_directories(OakD PUBLIC Include)

target_link_libraries(OakD ${DepthAI_LIBRARIES})
target_link_libraries(OakD PUBLIC depthai::opencv ${OpenCV_LIBS})
target_include_directories(OakD PUBLIC Source)
