cmake_minimum_required(VERSION 4.0)
project(_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(_2 main.cpp
        Pipe.cpp
        Pipe.h
        station.cpp
        station.h
        Utils.cpp
        Utils.h
        manager.cpp
        manager.h)
