cmake_minimum_required(VERSION 3.15.3)

project(respi C CXX ASM)

set (CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake)

set(MCU_MODEL STM32F302x8)
set(STARTUP_SCRIPT ${CMAKE_SOURCE_DIR}/stm32/Core/Startup/startup_stm32f302r8tx.s)
set(MCU_LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32/STM32F302R8TX_FLASH.ld)

file(GLOB SOURCES 
	${CMAKE_SOURCE_DIR}/stm32/Drivers/STM32F3xx_HAL_Driver/Src/*.c
)
set(SOURCES ${SOURCES})

set(STM32CUBEMX_GENERATED_FILES
    ${CMAKE_SOURCE_DIR}/stm32/Core/Src/main.c
    ${CMAKE_SOURCE_DIR}/stm32/Core/Src/crc.cpp
    ${CMAKE_SOURCE_DIR}/stm32/Core/Src/stm32f3xx_hal_msp.c
    ${CMAKE_SOURCE_DIR}/stm32/Core/Src/stm32f3xx_it.c
    ${CMAKE_SOURCE_DIR}/stm32/Core/Src/system_stm32f3xx.c

    ${STARTUP_SCRIPT}               
    
    ${CMAKE_SOURCE_DIR}/Application/src/breath_machine.cpp
    ${CMAKE_SOURCE_DIR}/Application/src/commands.cpp
    ${CMAKE_SOURCE_DIR}/Application/src/measurements.cpp
    ${CMAKE_SOURCE_DIR}/Application/src/print_output.cpp
    ${CMAKE_SOURCE_DIR}/Application/src/threads.cpp
    ${CMAKE_SOURCE_DIR}/Application/src/trigger.cpp
    
    ${CMAKE_SOURCE_DIR}/Arch/src/DataAccessor.cpp
    ${CMAKE_SOURCE_DIR}/Arch/src/Fibre.cpp
    
    ${SOURCES}
	)

set(EXECUTABLE ${PROJECT_NAME}.out)
add_executable(${EXECUTABLE} ${STM32CUBEMX_GENERATED_FILES})

target_include_directories(${EXECUTABLE} PRIVATE
    ${CMAKE_SOURCE_DIR}/Application/inc
    ${CMAKE_SOURCE_DIR}/Arch/inc        
    ${CMAKE_SOURCE_DIR}/stm32/Core/Inc
    ${CMAKE_SOURCE_DIR}/stm32/Drivers/STM32F3xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/stm32/Drivers/CMSIS/Device/ST/STM32F3xx/Include
    ${CMAKE_SOURCE_DIR}/stm32/Drivers/CMSIS/Include
    )

target_compile_definitions(${EXECUTABLE} PRIVATE
    ${MCU_MODEL}
    USE_HAL_DRIVER
    DEBUG 
    )
    
target_link_options(${EXECUTABLE} PRIVATE
    -T${MCU_LINKER_SCRIPT}
)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${SIZE_CMD} ${EXECUTABLE}
)

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${OBJCOPY_CMD} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND ${OBJCOPY_CMD} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
)
