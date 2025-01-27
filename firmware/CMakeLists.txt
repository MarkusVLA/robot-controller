cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Export compile commands to json for vim
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(my_esp32_project)
