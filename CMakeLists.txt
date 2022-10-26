cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(fps6 VERSION 1.0.0 LANGUAGES C CXX)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG)
find_package(OPENGL REQUIRED)

include_directories("src" ${OPENGL_INCLUDE_DIR})

file(GLOB_RECURSE FPS6SRC "src/*.cpp" "src/*.hpp")
add_executable(fps6 ${FPS6SRC})
target_link_libraries(fps6 PRIVATE SDL2::SDL2main SDL2::SDL2-static ${OPENGL_LIBRARIES})

if(NOT EMSCRIPTEN)
  include_directories("glad/include")
  file(GLOB_RECURSE GLADSRC "glad/src/*.c")
  add_library(GLAD STATIC ${GLADSRC})
  target_link_libraries(fps6 PRIVATE GLAD)
else()
  target_compile_options(fps6 PRIVATE -fexceptions)
  target_link_options(fps6 PRIVATE -fexceptions -s MIN_WEBGL_VERSION=2 -sASYNCIFY)
endif()

if(SDL2_mixer_FOUND)
  target_link_libraries(fps6 PRIVATE SDL2_mixer::SDL2_mixer-static)
else()
  target_compile_definitions(fps6 PRIVATE -DFPS6_NO_AUDIO)
endif()

add_definitions(-DNOMINMAX)
