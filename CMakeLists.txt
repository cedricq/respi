cmake_minimum_required(VERSION 3.15.3)

project(respi C CXX ASM)

set(MCU_MODEL STM32F302x8)

add_subdirectory (stm32)
add_subdirectory (Arch)
add_subdirectory (Application)
