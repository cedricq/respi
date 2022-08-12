cmake_minimum_required(VERSION 3.15.3)


project(respi C CXX ASM)

if(${TEST})
enable_testing()
endif()

add_subdirectory (stm32)
add_subdirectory (Arch)
add_subdirectory (Application)

if(${TEST})
# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(CTest)
endif()

