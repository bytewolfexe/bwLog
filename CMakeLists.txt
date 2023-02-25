
add_library(bwLog STATIC)

set(
	BWTK_LOG_SOURCE_FILES
	src/bwLog.cpp
	src/time_point.cpp
)

set(
	BWTK_LOG_HEADER_FILES
	include/bwLog/bwLog.hpp
	src/string_builder.hpp
)

target_sources(bwLog PRIVATE ${BWTK_LOG_SOURCE_FILES} ${BWTK_LOG_HEADER_FILES})

target_include_directories(bwLog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)