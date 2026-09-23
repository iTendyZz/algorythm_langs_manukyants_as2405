cmake_minimum_required(VERSION 4.0)
project(3)

set(CMAKE_CXX_STANDARD 20)

add_executable(3 main.cpp
        Pipe.cpp
        Pipe.h
        station.cpp
        station.h
        manager.cpp
        manager.h
        Utils.cpp
        Utils.h)
