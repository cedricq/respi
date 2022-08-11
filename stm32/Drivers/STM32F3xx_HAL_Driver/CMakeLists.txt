set(PROJECT stm32)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true Inc/*.h Src/*.c)
set(SOURCES ${SOURCES})
add_library(${PROJECT}_lib STATIC ${SOURCES})
target_include_directories(${PROJECT}_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc 
    ../../Core/Inc 
    ../CMSIS/Device/ST/STM32F3xx/Include
    ../CMSIS/Include
)

target_compile_definitions(${PROJECT}_lib PUBLIC STM32F302x8)
