set(PROJECT arch)

set(APP_SOURCES
    src/DataAccessor.cpp
    src/Fibre.cpp
	)
add_library (${PROJECT} ${APP_SOURCES})
target_include_directories (${PROJECT} PUBLIC inc )


if(${TEST})
set(APP_SOURCES
    unit/Fibre_t.cpp
	)	
add_executable (arch_test unit/Fibre_t.cpp src/Fibre.cpp)
target_link_libraries(arch_test gtest_main arch)
add_test(arch_test arch_test) 
endif()
