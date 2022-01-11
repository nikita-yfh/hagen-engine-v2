CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

set(CMAKE_CXX_STANDART 11)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "-fno-exceptions -fno-rtti -flto") #disable exceptions, not needed for game engine
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-s -flto")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_definitions("-DLOCAL_SAVES")

include("FindSDL2.cmake")
include("FindSDL2_mixer.cmake")
include("FindLuaJIT.cmake")

add_subdirectory("src")
