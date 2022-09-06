set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

#set(ARM_TOOLCHAIN_DIR /opt/gcc-arm-none-eabi-10.3-2021.10/bin)
set(TOOLCHAIN_PREFIX  arm-none-eabi)

set(CMAKE_C_COMPILER 			"${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}-gcc")
set(CMAKE_ASM_COMPILER 			"${CMAKE_C_COMPILER}")
set(CMAKE_CXX_COMPILER 			"${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}-g++")

add_definitions(     
    -DSTM32F302x8
    -DUSE_HAL_DRIVER
    -DDEBUG  
    )

set(GCC_CROSS_BASE_FLAGS
        "-mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -specs=nosys.specs -specs=nano.specs -fpic -ffunction-sections -fdata-sections -fno-lto -Wl,--gc-sections -Wall -Og -g3")
set(CMAKE_C_FLAGS ${GCC_CROSS_BASE_FLAGS})
set(CMAKE_CXX_FLAGS ${GCC_CROSS_BASE_FLAGS})
set(CMAKE_ASM_FLAGS ${GCC_CROSS_BASE_FLAGS})

set(CMAKE_CROSSCOMPILING_EMULATOR "${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}-gdb")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(OBJCOPY_CMD ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}-objcopy)
set(SIZE_CMD ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}-size)