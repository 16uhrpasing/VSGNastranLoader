cmake_minimum_required(VERSION 3.7)
project(NastranLoaderVSGExample
        VERSION 0.0.0
        DESCRIPTION "Example Source file of how to use the nastran loader in VSG"
        LANGUAGES CXX C
)

# find the vsg
if (VULKAN_SDK)
    set(ENV{VULKAN_SDK} ${VULKAN_SDK})
endif()

find_package( vsg REQUIRED )
find_package( vsgXchange REQUIRED )

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp nastran.h nastran.cpp)

include_directories("${CMAKE_SOURCE_DIR}/")

target_link_libraries(${PROJECT_NAME} vsg::vsg)
target_link_libraries(${PROJECT_NAME} vsgXchange::vsgXchange)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/truck.nas
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

