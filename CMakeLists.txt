cmake_minimum_required(VERSION 3.28)
project(riscvemulator)
set(CMAKE_BUILD_TYPE Debug)
add_executable(riscvemulator main.c bus.c cpu32.c dram.c machine.c rv32i.c rv32m.c)