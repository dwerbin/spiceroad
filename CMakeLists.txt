cmake_minimum_required(VERSION 3.20.0)
project(spiceroad)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB CPP_SOURCES 
    ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB C_SOURCES
    ${PROJECT_SOURCE_DIR}/*.c)
file(GLOB HEADERS
    ${PROJECT_SOURCE_DIR}/*.h)
    
add_executable(${PROJECT_NAME} ${C_SOURCES} ${CPP_SOURCES} ${HEADERS})

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -g -Wall -Wno-long-long -pedantic)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE 
    . 
)
#target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG WIN32_LEAN_AND_MEAN)
