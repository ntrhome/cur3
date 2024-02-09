#My CMakeLists.txt
#$ cc -xc -std=c17 -o cmake-build-debug/dur main.c dur.c

#cmake_minimum_required(VERSION 3.27)
cmake_minimum_required(VERSION 3.18)
project(dur C)

#set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD 17)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -v")

add_executable(dur main.c
#        durModel.c
#        durModel.h
        durView.c
        durView.h
        durControl.c
        durControl.h
        dur.c
        dur.h
)
