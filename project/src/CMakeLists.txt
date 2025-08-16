cmake_minimum_required(VERSION 3.10)  
project(QtWord)

set(CMAKE_CXX_STANDARD 11) 

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui LinguistTools Network REQUIRED)

add_subdirectory(editdoc)
add_subdirectory(mainwindow)
add_subdirectory(topmenu)