cmake_minimum_required(VERSION 3.15.3)

project(respi C CXX ASM)

IF(TEST)
	enable_testing()

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
	
	add_subdirectory (Arch)
	
ELSE()
	add_definitions(     
	    -DSTM32F302x8
	    -DUSE_HAL_DRIVER
	    -DDEBUG
	    -DUSE_FULL_ASSERT 
	    -DHSE_VALUE=8000000      
	    )
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${CMAKE_CURRENT_SOURCE_DIR}/stm32/STM32F302R8TX_FLASH.ld")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_NAME}.map")
	
	add_subdirectory (stm32)
	add_subdirectory (Arch)
	add_subdirectory (Application)
ENDIF()


