cmake_minimum_required(VERSION 3.27)
project(dur C)

set(CMAKE_C_STANDARD 23)

add_executable(dur main.c
        durModel.c
        durModel.h
        durView.c
        durView.h
        durControl.c
        durControl.h)
