cmake_minimum_required(VERSION 3.20.0)
project(spiceroad)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(THIRD_PARTY_DIR "${CMAKE_CURRENT_LIST_DIR}/3rdparty")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---- DOWNLOAD AND SETUP SDL2 ----
SET(SDL2_DIR "${THIRD_PARTY_DIR}/SDL2-2.26.1" CACHE PATH "SDL2 directory" FORCE)
set(ENV{SDL2_ROOT_DIR} ${SDL2_DIR})
if (NOT EXISTS "${SDL2_DIR}")
    SET(SDL2_ZIP_FILE "SDL2-devel-2.26.1-mingw.tar.gz")
    SET(SDL2_URL "https://github.com/libsdl-org/SDL/releases/download/release-2.26.1/${SDL2_ZIP_FILE}")

#     message(STATUS "--- SDL2_DIR ---")
#     message(STATUS ${THIRD_PARTY_DIR})
#     message(STATUS ${SDL2_DIR})
#     message(STATUS ${SDL2_URL})
#     message(STATUS ${SDL2_ZIP_FILE})
#     message(STATUS ${CMAKE_COMMAND})

    if (NOT EXISTS "${THIRD_PARTY_DIR}/${SDL2_ZIP_FILE}")
        file(DOWNLOAD "${SDL2_URL}" "${THIRD_PARTY_DIR}/${SDL2_ZIP_FILE}" SHOW_PROGRESS)
    endif()
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${SDL2_ZIP_FILE}
        WORKING_DIRECTORY "${THIRD_PARTY_DIR}"
    )
endif()

# ---- DOWNLOAD AND SETUP SDL2 IMAGE ----
SET(SDL2_IMAGE_DIR "${THIRD_PARTY_DIR}/SDL2_image-2.6.2" CACHE PATH "SDL2 image directory" FORCE)
set(ENV{SDL2_IMAGE_ROOT_DIR} ${SDL2_IMAGE_DIR})
if (NOT EXISTS "${SDL2_IMAGE_DIR}")
    SET(SDL2_IMAGE_ZIP_FILE "SDL2_image-devel-2.6.2-mingw.tar.gz")
    SET(SDL2_IMAGE_URL "https://github.com/libsdl-org/SDL_image/releases/download/release-2.6.2/${SDL2_IMAGE_ZIP_FILE}")

    if (NOT EXISTS "${THIRD_PARTY_DIR}/${SDL2_IMAGE_ZIP_FILE}")
        file(DOWNLOAD "${SDL2_IMAGE_URL}" "${THIRD_PARTY_DIR}/${SDL2_IMAGE_ZIP_FILE}" SHOW_PROGRESS)
    endif()
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${SDL2_IMAGE_ZIP_FILE}
        WORKING_DIRECTORY "${THIRD_PARTY_DIR}"
    )
endif()

# ---- DOWNLOAD AND SETUP SDL2 TTF ----
SET(SDL2_TTF_DIR "${THIRD_PARTY_DIR}/SDL2_ttf-2.20.1" CACHE PATH "SDL2 ttf directory" FORCE)
set(ENV{SDL2_TTF_ROOT_DIR} ${SDL2_TTF_DIR})
if (NOT EXISTS "${SDL2_TTF_DIR}")
    SET(SDL2_TTF_ZIP_FILE "SDL2_ttf-devel-2.20.1-mingw.tar.gz")
    SET(SDL2_TTF_URL "https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.20.1/${SDL2_TTF_ZIP_FILE}")

    if (NOT EXISTS "${THIRD_PARTY_DIR}/${SDL2_TTF_ZIP_FILE}")
        file(DOWNLOAD "${SDL2_TTF_URL}" "${THIRD_PARTY_DIR}/${SDL2_TTF_ZIP_FILE}" SHOW_PROGRESS)
    endif()
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${SDL2_TTF_ZIP_FILE}
        WORKING_DIRECTORY "${THIRD_PARTY_DIR}"
    )
endif()
# ----

add_subdirectory(spicelib)
add_subdirectory(app)
