set(PROJECT application)

if(NOT ${TEST})
set(APP_SOURCES
    src/breath_machine.cpp
    src/commands.cpp
    src/measurements.cpp
    src/print_output.cpp
    src/threads.cpp
    src/trigger.cpp
	)
add_library (${PROJECT} ${APP_SOURCES})
target_include_directories (${PROJECT} PUBLIC inc ${CMAKE_SOURCE_DIR}/stm32/Core/Inc)
target_link_libraries (${PROJECT} LINK_PUBLIC stm32_driver arch)
endif()