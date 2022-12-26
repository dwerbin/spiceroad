#.rst:
# FindSDL2_ttf
# ---------
#
# Find the SDL_ttf (version 2.x).
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module sets the following variables:
#
# ``SDL2_ttf_FOUND``
#  True, if the system has SDL2_ttf.
# ``SDL2_ttf_INCLUDE_DIR``
#  Path to the SDL2_ttf include directory.
# ``SDL2_ttf_LIBRARY``
#  SDL2_ttf library.
# ``SDL2_ttf_SHARED_LIBRARIES``
#  Paths to the SDL2_ttf shared libraries.


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
    if (DEFINED ENV{SDL2_TTF_ROOT_DIR})
        if (MINGW)
            if (CMAKE_SIZEOF_VOID_P EQUAL 4)
                find_path(SDL2_ttf_INCLUDE_DIR "SDL_ttf.h" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "i686-w64-mingw32/include/SDL2" DOC "Path to the SDL2_ttf include directory.")
                find_library(SDL2_ttf_LIBRARY SDL2_ttf HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "i686-w64-mingw32/lib" DOC "SDL2_ttf library.")
                find_file(SDL2_ttf_SHARED_LIBRARY "SDL2_ttf.dll" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "i686-w64-mingw32/bin" DOC "Paths to the SDL2_ttf shared library.")
                find_file(SDL2_ttf_freetype_SHARED_LIBRARY "libfreetype-6.dll" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "i686-w64-mingw32/bin" DOC "Path to the (SDL2_ttf boundle) FreeType shared library.")
                find_file(SDL2_ttf_zlib_SHARED_LIBRARY "zlib1.dll" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "i686-w64-mingw32/bin" DOC "Path to the (SDL2_ttf boundle) zlib shared library.")
            elseif (CMAKE_SIZEOF_VOID_P EQUAL 8)
                find_path(SDL2_ttf_INCLUDE_DIR "SDL_ttf.h" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "x86_64-w64-mingw32/include/SDL2" DOC "Path to the SDL2_ttf include directory.")
                find_library(SDL2_ttf_LIBRARY SDL2_ttf HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "x86_64-w64-mingw32/lib" DOC "SDL2_ttf library.")
                find_file(SDL2_ttf_SHARED_LIBRARY "SDL2_ttf.dll" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "x86_64-w64-mingw32/bin" DOC "Paths to the SDL2_ttf shared library.")
                find_file(SDL2_ttf_freetype_SHARED_LIBRARY "libfreetype-6.dll" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "x86_64-w64-mingw32/bin" DOC "Path to the (SDL2_ttf boundle) FreeType shared library.")
                find_file(SDL2_ttf_zlib_SHARED_LIBRARY "zlib1.dll" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "x86_64-w64-mingw32/bin" DOC "Path to the (SDL2_ttf boundle) zlib shared library.")
            endif ()
        elseif (MSVC)
            find_path(SDL2_ttf_INCLUDE_DIR "SDL_ttf.h" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "include" DOC "Path to the SDL2_ttf include directory.")
            if (CMAKE_SIZEOF_VOID_P EQUAL 4)
                find_library(SDL2_ttf_LIBRARY SDL2_ttf HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "lib/x86" DOC "SDL2_ttf library.")
                find_file(SDL2_ttf_SHARED_LIBRARY "SDL2_ttf.dll" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "lib/x86" DOC "Paths to the SDL2_ttf shared library.")
                find_file(SDL2_ttf_freetype_SHARED_LIBRARY "libfreetype-6.dll" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "lib/x86" DOC "Path to the (SDL2_ttf boundle) FreeType shared library.")
                find_file(SDL2_ttf_zlib_SHARED_LIBRARY "zlib1.dll" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "lib/x86" DOC "Path to the (SDL2_ttf boundle) zlib shared library.")
            elseif (CMAKE_SIZEOF_VOID_P EQUAL 8)
                find_library(SDL2_ttf_LIBRARY SDL2_ttf HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "lib/x64" DOC "SDL2_ttf library.")
                find_file(SDL2_ttf_SHARED_LIBRARY "SDL2_ttf.dll" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "lib/x64" DOC "Paths to the SDL2_ttf shared library.")
                find_file(SDL2_ttf_freetype_SHARED_LIBRARY "libfreetype-6.dll" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "lib/x64" DOC "Path to the (SDL2_ttf boundle) FreeType shared library.")
                find_file(SDL2_ttf_zlib_SHARED_LIBRARY "zlib1.dll" HINTS $ENV{SDL2_TTF_ROOT_DIR} PATH_SUFFIXES "lib/x64" DOC "Path to the (SDL2_ttf boundle) zlib shared library.")
            endif ()
        endif ()
        list(APPEND SDL2_ttf_SHARED_LIBRARIES ${SDL2_ttf_SHARED_LIBRARY})
        list(APPEND SDL2_ttf_SHARED_LIBRARIES ${SDL2_ttf_freetype_SHARED_LIBRARY})
        list(APPEND SDL2_ttf_SHARED_LIBRARIES ${SDL2_ttf_zlib_SHARED_LIBRARY})
        set(SDL2_ttf_SHARED_LIBRARIES ${SDL2_ttf_SHARED_LIBRARIES} CACHE STRING "Paths to the SDL2_ttf shared libraries.")
    else ()
        message(WARNING "SDL2_TTF_ROOT_DIR is not set.")
    endif ()
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2_ttf REQUIRED_VARS SDL2_ttf_INCLUDE_DIR SDL2_ttf_LIBRARY)

mark_as_advanced(SDL2_ttf_INCLUDE_DIR SDL2_ttf_LIBRARY)
mark_as_advanced(SDL2_ttf_SHARED_LIBRARY)
mark_as_advanced(SDL2_ttf_freetype_SHARED_LIBRARY)
mark_as_advanced(SDL2_ttf_zlib_SHARED_LIBRARY)
mark_as_advanced(SDL2_ttf_SHARED_LIBRARIES)
