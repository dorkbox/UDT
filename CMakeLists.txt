PROJECT( udt )
cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-D_DEBUG)
endif()

add_definitions(
	-Wall
	-D__UNIX
	-DPOSIX_C_SOURCE
	-DGNU_SOURCE
	-Wno-unused-function
	-std=c++11
)

SET(LIBRARIES
	pthread
	m
	${ARCAN_SHMIF_LIBRARY}
)

set(SOURCES
	src/api.cpp
	src/buffer.cpp
	src/cache.cpp
	src/ccc.cpp
	src/channel.cpp
	src/core.cpp
	src/epoll.cpp
	src/list.cpp
	src/md5.cpp
	src/packet.cpp
	src/queue.cpp
	src/udtCommon.cpp
	src/window.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
