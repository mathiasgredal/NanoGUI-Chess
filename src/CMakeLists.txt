cmake_minimum_required (VERSION 2.8)

project (CMAKETEST)

include_directories(${CMAKE_SOURCE_DIR}/include)


if (WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mms-bitfields -DWIN32 -std=c++14 -g")
    link_directories(${CMAKE_SOURCE_DIR}/win32)
endif (WIN32)

if (APPLE)
    set (CMAKE_CXX_STANDARD 17)
    link_directories(${CMAKE_SOURCE_DIR}/macos)
endif (APPLE)

add_executable(chess main.cpp chess_GUI.cpp chess_GUI.h)

if (WIN32)
    target_link_libraries(chess ole32 uuid comctl32 wsock32 m gdi32 user32 kernel32)
endif (WIN32)

if(APPLE)
  find_library(COCOA Cocoa)
  target_link_libraries(chess ${FLTKLIB} ${COCOA})
endif()


target_link_libraries(chess fltk)
