set(PROJECT stm32)

if(NOT ${TEST})

file(GLOB STM32_SOURCES 
	Drivers/STM32F3xx_HAL_Driver/Src/*.c
)
set(STM32_SOURCES ${STM32_SOURCES})
add_library (${PROJECT}_driver ${STM32_SOURCES})
target_include_directories (${PROJECT}_driver PUBLIC 
	Drivers/STM32F3xx_HAL_Driver/Inc
	Drivers/CMSIS/Device/ST/STM32F3xx/Include
	Drivers/CMSIS/Include
	Core/Inc
	)
target_compile_definitions(${PROJECT}_driver PRIVATE
    ${MCU_MODEL}
    USE_HAL_DRIVER
    DEBUG 
    )
set(MAIN_GENERATED_FILES
    Core/Src/main.c
    Core/Src/crc.cpp
    Core/Src/stm32f3xx_hal_msp.c
    Core/Src/stm32f3xx_it.c
    Core/Src/system_stm32f3xx.c
	# STARTUP SCRIPT
    Core/Startup/startup_stm32f302r8tx.s
	)
set(EXECUTABLE ${PROJECT_NAME}.elf)
add_executable(${EXECUTABLE} ${MAIN_GENERATED_FILES})
target_link_libraries (${EXECUTABLE} LINK_PUBLIC stm32_driver application) 
# arch application)
#target_link_options(${EXECUTABLE} PRIVATE
#    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F302R8TX_FLASH.ld
#)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${SIZE_CMD} ${EXECUTABLE}
)
# Create hex file
set (BIN_PATH ${CMAKE_SOURCE_DIR}/build)
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND cp ${EXECUTABLE} ${BIN_PATH}/
        COMMAND ${OBJCOPY_CMD} -O ihex   ${BIN_PATH}/${EXECUTABLE} ${BIN_PATH}/${PROJECT_NAME}.hex
        COMMAND ${OBJCOPY_CMD} -O binary ${BIN_PATH}/${EXECUTABLE} ${BIN_PATH}/${PROJECT_NAME}.bin
)

endif()
