project(OakD)

message([STATUS] " BUILDING " ${PROJECT_NAME})

add_library(${PROJECT_NAME} Source/OakD.cpp)

include_directories(Include/)

target_link_libraries(${PROJECT_NAME} ${DepthAI_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE depthai::opencv)
