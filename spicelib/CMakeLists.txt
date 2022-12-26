# export 
#   SPICELIB_INCLUDE_DIR
#   SPICELIB_LIBRARY

SET(LIB_NAME spicelib)

file(GLOB CPP_SOURCES 
    *.cpp)
file(GLOB C_SOURCES
    *.c)
file(GLOB HEADERS
    *.h)
    
add_library(${LIB_NAME} STATIC ${C_SOURCES} ${CPP_SOURCES} ${HEADERS})

if(MSVC)
  target_compile_options(${LIB_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${LIB_NAME} PRIVATE -g -Wall -Wno-long-long -pedantic)
endif()

target_include_directories(${LIB_NAME} PRIVATE 
    . 
)
target_compile_definitions(${LIB_NAME} PRIVATE DEBUG WIN32_LEAN_AND_MEAN)

get_filename_component(SPICELIB_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)
set(SPICELIB_INCLUDE_DIR
  ${SPICELIB_INCLUDE_DIR}
  PARENT_SCOPE
)
set(SPICELIB_LIBRARY ${LIB_NAME} PARENT_SCOPE)