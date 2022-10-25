cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(fps6 VERSION 1.0.0 LANGUAGES C CXX)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(OPENGL REQUIRED)

include_directories("src" "glad/include" ${OPENGL_INCLUDE_DIR})

file(GLOB_RECURSE GLADSRC "glad/src/*.c")
add_library(GLAD STATIC ${GLADSRC})

file(GLOB_RECURSE FPS6SRC "src/*.cpp" "src/*.hpp")
add_executable(fps6 ${FPS6SRC})
target_link_libraries(fps6 PRIVATE GLAD SDL2_mixer::SDL2_mixer-static SDL2::SDL2main SDL2::SDL2-static ${OPENGL_LIBRARIES})

add_definitions(-DNOMINMAX)
