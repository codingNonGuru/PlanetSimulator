cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE None)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lGL -lGLU -lGLEW -lpthread -lSDL2 -lSDL2_ttf -lSDL2_image -std=c++11 -fpermissive")

file(GLOB_RECURSE SOURCE_FILES
	"Source/*"
)

add_definitions(-DTHREAD_NUMBER=4 -DPLATFORM_LINUX)
add_definitions(-DGRAVITY -DGRAVITY_VALUE=3)

include_directories(/home/andrei/Documents/Programming/Libraries/glm/glm)
include_directories(/usr/local/include)

project(PlanetSimulator)

add_executable(
	PlanetSimulator
	${SOURCE_FILES}
)