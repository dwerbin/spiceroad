#.rst:
# FindSDL2_image
# ---------
#
# Find the SDL_image (version 2.x).
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module sets the following variables:
#
# ``SDL2_image_FOUND``
#  True, if the system has SDL2_image.
# ``SDL2_image_INCLUDE_DIR``
#  Path to the SDL2_image include directory.
# ``SDL2_image_LIBRARY``
#  SDL2_image library.
# ``SDL2_image_SHARED_LIBRARIES``
#  Paths to the SDL2_image shared libraries.


#=============================================================================
# Copyright 2015 Jacek Migacz (jacekmigacz@gmail.com)
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

if (WIN32)
    if (DEFINED ENV{SDL2_IMAGE_ROOT_DIR})
        if (MINGW)
            if (CMAKE_SIZEOF_VOID_P EQUAL 4)
                find_path(SDL2_image_INCLUDE_DIR "SDL_image.h" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "i686-w64-mingw32/include/SDL2" DOC "Path to the SDL2_image include directory.")
                find_library(SDL2_image_LIBRARY SDL2_image HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "i686-w64-mingw32/lib" DOC "SDL2_image library.")
                find_file(SDL2_image_SHARED_LIBRARY "SDL2_image.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "i686-w64-mingw32/bin" DOC "Path to the SDL2_image shared library.")
                find_file(SDL2_image_jpeg_SHARED_LIBRARY "libjpeg-9.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "i686-w64-mingw32/bin" DOC "Path to the (SDL2_image boundle) JPEG shared library.")
                find_file(SDL2_image_png_SHARED_LIBRARY "libpng16-16.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "i686-w64-mingw32/bin" DOC "Path to the (SDL2_image boundle) PNG shared library.")
                find_file(SDL2_image_tiff_SHARED_LIBRARY "libtiff-5.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "i686-w64-mingw32/bin" DOC "Path to the (SDL2_image boundle) TIFF shared library.")
                find_file(SDL2_image_webp_SHARED_LIBRARY "libwebp-4.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "i686-w64-mingw32/bin" DOC "Path to the (SDL2_image boundle) WebP shared library.")
                find_file(SDL2_image_zlib_SHARED_LIBRARY "zlib1.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "i686-w64-mingw32/bin" DOC "Path to the (SDL2_image boundle) zlib shared library.")
            elseif (CMAKE_SIZEOF_VOID_P EQUAL 8)
                find_path(SDL2_image_INCLUDE_DIR "SDL_image.h" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "x86_64-w64-mingw32/include/SDL2" DOC "Path to the SDL2_image include directory.")
                find_library(SDL2_image_LIBRARY SDL2_image HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "x86_64-w64-mingw32/lib" DOC "SDL2_image library.")
                find_file(SDL2_image_SHARED_LIBRARY "SDL2_image.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "x86_64-w64-mingw32/bin" DOC "Path to the SDL2_image shared library.")
                find_file(SDL2_image_jpeg_SHARED_LIBRARY "libjpeg-9.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "x86_64-w64-mingw32/bin" DOC "Path to the (SDL2_image boundle) JPEG shared library.")
                find_file(SDL2_image_png_SHARED_LIBRARY "libpng16-16.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "x86_64-w64-mingw32/bin" DOC "Path to the (SDL2_image boundle) PNG shared library.")
                find_file(SDL2_image_tiff_SHARED_LIBRARY "libtiff-5.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "x86_64-w64-mingw32/bin" DOC "Path to the (SDL2_image boundle) TIFF shared library.")
                find_file(SDL2_image_webp_SHARED_LIBRARY "libwebp-4.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "x86_64-w64-mingw32/bin" DOC "Path to the (SDL2_image boundle) WebP shared library.")
                find_file(SDL2_image_zlib_SHARED_LIBRARY "zlib1.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "x86_64-w64-mingw32/bin" DOC "Path to the (SDL2_image boundle) zlib shared library.")
            endif ()
        elseif (MSVC)
            find_path(SDL2_image_INCLUDE_DIR "SDL_image.h" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "include" DOC "Path to the SDL2_image include directory.")
            if (CMAKE_SIZEOF_VOID_P EQUAL 4)
                find_library(SDL2_image_LIBRARY SDL2_image HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "lib/x86" DOC "SDL2_image library.")
                find_file(SDL2_image_SHARED_LIBRARY "SDL2_image.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "lib/x86" DOC "Path to the SDL2_image shared library.")
                find_file(SDL2_image_jpeg_SHARED_LIBRARY "libjpeg-9.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "lib/x86" DOC "Path to the (SDL2_image boundle) JPEG shared library.")
                find_file(SDL2_image_png_SHARED_LIBRARY "libpng16-16.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "lib/x86" DOC "Path to the (SDL2_image boundle) PNG shared library.")
                find_file(SDL2_image_tiff_SHARED_LIBRARY "libtiff-5.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "lib/x86" DOC "Path to the (SDL2_image boundle) TIFF shared library.")
                find_file(SDL2_image_webp_SHARED_LIBRARY "libwebp-4.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "lib/x86" DOC "Path to the (SDL2_image boundle) WebP shared library.")
                find_file(SDL2_image_zlib_SHARED_LIBRARY "zlib1.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "lib/x86" DOC "Path to the (SDL2_image boundle) zlib shared library.")
            elseif (CMAKE_SIZEOF_VOID_P EQUAL 8)
                find_library(SDL2_image_LIBRARY SDL2_image HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "lib/x64" DOC "SDL2_image library.")
                find_file(SDL2_image_SHARED_LIBRARY "SDL2_image.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "lib/x64" DOC "Path to the SDL2_image shared library.")
                find_file(SDL2_image_jpeg_SHARED_LIBRARY "libjpeg-9.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "lib/x64" DOC "Path to the (SDL2_image boundle) JPEG shared library.")
                find_file(SDL2_image_png_SHARED_LIBRARY "libpng16-16.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "lib/x64" DOC "Path to the (SDL2_image boundle) PNG shared library.")
                find_file(SDL2_image_tiff_SHARED_LIBRARY "libtiff-5.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "lib/x64" DOC "Path to the (SDL2_image boundle) TIFF shared library.")
                find_file(SDL2_image_webp_SHARED_LIBRARY "libwebp-4.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "lib/x64" DOC "Path to the (SDL2_image boundle) WebP shared library.")
                find_file(SDL2_image_zlib_SHARED_LIBRARY "zlib1.dll" HINTS $ENV{SDL2_IMAGE_ROOT_DIR} PATH_SUFFIXES "lib/x64" DOC "Path to the (SDL2_image boundle) zlib shared library.")
            endif ()
        endif ()
        list(APPEND SDL2_image_SHARED_LIBRARIES ${SDL2_image_SHARED_LIBRARY})
        list(APPEND SDL2_image_SHARED_LIBRARIES ${SDL2_image_jpeg_SHARED_LIBRARY})
        list(APPEND SDL2_image_SHARED_LIBRARIES ${SDL2_image_png_SHARED_LIBRARY})
        list(APPEND SDL2_image_SHARED_LIBRARIES ${SDL2_image_tiff_SHARED_LIBRARY})
        list(APPEND SDL2_image_SHARED_LIBRARIES ${SDL2_image_webp_SHARED_LIBRARY})
        list(APPEND SDL2_image_SHARED_LIBRARIES ${SDL2_image_zlib_SHARED_LIBRARY})
        set(SDL2_image_SHARED_LIBRARIES ${SDL2_image_SHARED_LIBRARIES} CACHE STRING "Paths to the SDL2_image shared libraries.")
    else ()
        message(WARNING "SDL2_IMAGE_ROOT_DIR is not set.")
    endif ()
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2_image REQUIRED_VARS SDL2_image_INCLUDE_DIR SDL2_image_LIBRARY)

mark_as_advanced(SDL2_image_INCLUDE_DIR SDL2_image_LIBRARY)
mark_as_advanced(SDL2_image_SHARED_LIBRARY)
mark_as_advanced(SDL2_image_jpeg_SHARED_LIBRARY)
mark_as_advanced(SDL2_image_png_SHARED_LIBRARY)
mark_as_advanced(SDL2_image_tiff_SHARED_LIBRARY)
mark_as_advanced(SDL2_image_webp_SHARED_LIBRARY)
mark_as_advanced(SDL2_image_zlib_SHARED_LIBRARY)
mark_as_advanced(SDL2_image_SHARED_LIBRARIES)
