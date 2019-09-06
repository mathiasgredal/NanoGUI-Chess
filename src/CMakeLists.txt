cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)

project(Skakspil)

file(GLOB SOURCES
    main.cpp
    chess_GUI.h
    chess_GUI.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(skak ${SOURCES})

if(APPLE)
  find_library(FLTKLIB
        NAMES libfltk.a
        HINTS ${CMAKE_CURRENT_SOURCE_DIR}/macos/lib/)

  find_library(COCOA Cocoa)

  target_link_libraries(skak ${FLTKLIB} ${COCOA})
endif()

if(WIN32)
    find_library(FLTKLIB
          NAMES libfltk.a
          HINTS ${CMAKE_CURRENT_SOURCE_DIR}/win32/lib/)

   # Added because of this blog: http://ivory.idyll.org/blog/using-cmake.html
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")

   target_link_libraries(skak ${FLTKLIB})
endif()

install(TARGETS skak DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build)




