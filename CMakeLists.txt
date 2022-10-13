cmake_minimum_required(VERSION 3.18)

project(
	Blocktris
	DESCRIPTION "A block puzzle game written in C++ using SFML."
	LANGUAGES CXX
)

if(CMAKE_BUILD-TYPE MATCHES Debug)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++17")
endif()

find_package(SFML 2.5 COMPONENTS audio graphics window system REQUIRED)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(Source)
