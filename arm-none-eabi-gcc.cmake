set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

#set(ARM_TOOLCHAIN_DIR /opt/gcc-arm-none-eabi-10.3-2021.10/bin)
set(TOOLCHAIN_PREFIX  arm-none-eabi)

set(CMAKE_C_COMPILER 			"${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}-gcc")
set(CMAKE_CXX_COMPILER 			"${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}-g++")
set(CMAKE_ASM_COMPILER 			"${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}-g++")

add_definitions(     
    -DSTM32F302x8
    -DUSE_HAL_DRIVER
    -DDEBUG
    -DUSE_FULL_ASSERT 
    -DHSE_VALUE=8000000      
    )
      
set(GCC_CROSS_BASE_FLAGS_DEFAULT
        "-mcpu=cortex-m4 -g -Og -ffunction-sections -fdata-sections -Wall -Wfatal-errors -fstack-usage --specs=nano.specs -mfloat-abi=hard -mthumb "
        # -Werror
)

set(GCC_CROSS_BASE_FLAGS_CPP
        "${GCC_CROSS_BASE_FLAGS_DEFAULT} -std=c++11 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fpermissive "
)

set(GCC_CROSS_BASE_FLAGS_C
        "${GCC_CROSS_BASE_FLAGS_DEFAULT} " 
)

set(GCC_CROSS_BASE_FLAGS_ASM
        "-mcpu=cortex-m4 -g1 -x assembler-with-cpp --specs=nano.specs -mfloat-abi=hard -mthumb "         
)

set(GCC_CROSS_BASE_FLAGS_LINKER
        "-mcpu=cortex-m4 -T${CMAKE_CURRENT_SOURCE_DIR}/stm32/STM32F302R8TX_FLASH.ld --specs=nosys.specs -Wl,-Map=respi.map -Wl,--gc-sections -static -mfloat-abi=hard -mthumb -u _printf_float "
)
                
set(CMAKE_C_FLAGS               ${GCC_CROSS_BASE_FLAGS_C})
set(CMAKE_CXX_FLAGS             ${GCC_CROSS_BASE_FLAGS_CPP})
set(CMAKE_ASM_FLAGS             ${GCC_CROSS_BASE_FLAGS_ASM})
set(CMAKE_EXE_LINKER_FLAGS      ${GCC_CROSS_BASE_FLAGS_LINKER})

set(CMAKE_CROSSCOMPILING_EMULATOR "${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}-gdb")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(OBJCOPY_CMD ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}-objcopy)
set(SIZE_CMD ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}-size)