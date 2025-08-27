cmake_minimum_required(VERSION 4.0)
project(GestionArchivos C)

set(CMAKE_C_STANDARD 17)

add_executable(GestionArchivos main.c
        Options.h
        Options.c
        menu.h
        menu.c)
