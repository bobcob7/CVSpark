cmake_minimum_required (VERSION 3.0)
project(CVSpark)

set(CVSpark_VERSION_MAJOR 1)
set(CVSpark_VERSION_MINOR 0)
include(ExternalProject)

include_directories("${PROJECT_BINARY_DIR}")

#Set up all dependency URLs
set(SDL_GIT_URL https://www.libsdl.org/release/SDL2-2.0.4.tar.gz)
set(OpenCV_GIT_URL https://github.com/opencv/opencv.git)
	
	#Get SDL
	set(SDL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/SDL")
	set(SDL_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/SDL")
	set(SDL_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${SDL_INSTALL_DIR})
	ExternalProject_Add(
		SDL
		PREFIX ${SDL_PREFIX}
		URL ${SDL_GIT_URL}
		INSTALL_DIR ${SDL_INSTALL_DIR}
		CMAKE_ARGS ${SDL_CMAKE_ARGS}
	)

	#Get OpenCV
	set(OpenCV_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/OpenCV")
	set(OpenCV_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/OpenCV")
	set(OpenCV_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${OpenCV_INSTALL_DIR})
	ExternalProject_Add(
		OpenCV
		PREFIX ${OpenCV_PREFIX}
		GIT_REPOSITORY ${OpenCV_GIT_URL}
		GIT_TAG "3.1.0"
		INSTALL_DIR ${OpenCV_INSTALL_DIR}
		CMAKE_ARGS ${OpenCV_CMAKE_ARGS}
	)
	
	set(SDL2_INCLUDE_DIR ${SDL_INSTALL_DIR}/include)
	set(OpenCV_INCLUDE_DIR ${OpenCV_INSTALL_DIR}/include)
if(WIN32)
	message(�WINDOWS�)
	set(SDL2_LIBRARY ${SDL_INSTALL_DIR}/lib/SDL2.lib)
	set(SDL2_MAIN_LIBRARY ${SDL_INSTALL_DIR}/lib/SDL2main.lib)
	set(SDL2_LIBRARIES ${SDL2_LIBRARY} ${SDL2_MAIN_LIBRARY} opengl32.lib winmm.lib version.lib Imm32.lib)

elseif(APPLE)
	message(�OSX�)
	set(SDL2_LIBRARY ${SDL_INSTALL_DIR}/lib/libSDL2.a)
	set(SDL2_LIBRARIES ${SDL2_LIBRARY} ${SDL2_MAIN_LIBRARY})

elseif(LINUX)
	INCLUDE (FindPkgConfig)
	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
endif()

configure_file (
	"${PROJECT_SOURCE_DIR}/Info.h.in"
	"${PROJECT_BINARY_DIR}/Info.h"
)

file(GLOB INCS
	*.hpp
	*.h
)

file(GLOB SRC
	*.cpp
	*.c
)

add_executable(CVSpark ${SRC} ${INCS})
add_dependencies(CVSpark SDL OpenCV)
include_directories(${SDL2_INCLUDE_DIR} ${OpenCV_INCLUDE_DIR})
target_link_libraries(CVSpark ${OpenCV_LIBRARIES} ${SDL2_LIBRARIES})