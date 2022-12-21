set(PROJECT app)

set(APP_SOURCES
    src/breath_machine.cpp
    src/commands.cpp
    src/measurements.cpp
    src/print_output.cpp
    src/trigger.cpp
	)

add_library (${PROJECT} OBJECT ${APP_SOURCES})
target_include_directories (${PROJECT} PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/inc 
	)
target_link_libraries (${PROJECT} LINK_PUBLIC bsp_stm32f302r8_nucleo infra)
	
IF(TEST)
	#set(TEST_SOURCES
	#    unit/test_t.cpp
	#	)	
	#add_executable (app_test ${TEST_SOURCES} ${APP_SOURCES})
	#target_link_libraries(app_test gtest_main ${PROJECT} arch)
	#add_test(app_test app_test) 
ENDIF()