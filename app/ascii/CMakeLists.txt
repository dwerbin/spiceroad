SET(APP_EXE spiceroadascii)
add_executable(${APP_EXE} main.cpp)
target_link_libraries(${APP_EXE} PRIVATE ${SPICELIB_LIBRARY})
target_include_directories(${APP_EXE} PRIVATE 
    ${SPICELIB_INCLUDE_DIR}
)
target_compile_definitions(${APP_EXE} PRIVATE DEBUG WIN32_LEAN_AND_MEAN)
