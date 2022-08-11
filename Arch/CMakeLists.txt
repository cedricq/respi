set(PROJECT arch)

set(CMAKE_CXX_STANDARD 14)

set(APP_SOURCES src/Fibre.cpp src/DataAccessor.cpp)
add_library(${PROJECT}_lib STATIC ${APP_SOURCES})
target_include_directories(${PROJECT}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

set(TEST_SOURCES unit/Fibre_t.cpp)
add_executable (${PROJECT}_test ${TEST_SOURCES})
add_test(NAME ${PROJECT}_test COMMAND ${PROJECT}_test)
target_link_libraries(${PROJECT}_test PUBLIC gtest_main ${PROJECT}_lib)