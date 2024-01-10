cmake_minimum_required(VERSION 3.27)
project(dur C)

set(CMAKE_C_STANDARD 23)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(dur main.c
        durModel.c
        durModel.h
        durView.c
        durView.h
        durControl.c
        durControl.h)
