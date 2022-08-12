set(PROJECT arch)

set(APP_SOURCES
    src/DataAccessor.cpp
    src/Fibre.cpp
	)

add_library (${PROJECT} ${APP_SOURCES})
target_include_directories (${PROJECT} PUBLIC inc )