cmake_minimum_required(VERSION 3.28)
project(imprint)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

add_executable(imprint
        container.h
        imprint.h
        main.cpp
        node.h
        scope.h
        stream.h
        xml.h)
