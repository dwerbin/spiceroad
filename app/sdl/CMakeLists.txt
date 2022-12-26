SET(APP_EXE spiceroadgame)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(${APP_EXE} main.cpp)
target_link_libraries(${APP_EXE} PRIVATE 
    ${SDL2_LIBRARIES}
    ${SDL2_image_LIBRARY}
    ${SPICELIB_LIBRARY}
)
target_include_directories(${APP_EXE} PRIVATE 
    ${SDL2_INCLUDE_DIR}
    ${SDL2_image_INCLUDE_DIR}
    ${SPICELIB_INCLUDE_DIR}
)
target_compile_definitions(${APP_EXE} PRIVATE DEBUG WIN32_LEAN_AND_MEAN)

# create a list of files to copy
set(THIRD_PARTY_DLLS
   ${SDL2_SHARED_LIBRARY_PATH}
   ${SDL2_image_SHARED_LIBRARY}
   ${SDL2_ttf_SHARED_LIBRARY}
)

# do the copying
foreach(THIRD_PARTY_DLL ${THIRD_PARTY_DLLS})
    add_custom_command(
        TARGET ${APP_EXE}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${THIRD_PARTY_DLL} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
endforeach(THIRD_PARTY_DLL)
