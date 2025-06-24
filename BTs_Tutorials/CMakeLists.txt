cmake_minimum_required(VERSION 3.5)
project(BTS_Tutorials)

find_package(behaviortree_cpp REQUIRED)

add_executable(tutorial01
    src/tutorial01.cpp
)

target_link_libraries(tutorial01 PRIVATE BT::behaviortree_cpp)
target_include_directories(tutorial01 PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(tutorial02
    src/tutorial02.cpp
)

target_link_libraries(tutorial02 PRIVATE BT::behaviortree_cpp)
target_include_directories(tutorial02 PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(tutorial03
    src/tutorial03.cpp
)

target_link_libraries(tutorial03 PRIVATE BT::behaviortree_cpp)
target_include_directories(tutorial03 PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

file(GLOB XML_FILES "xml/*.xml")
file(COPY ${XML_FILES} DESTINATION ${CMAKE_BINARY_DIR})