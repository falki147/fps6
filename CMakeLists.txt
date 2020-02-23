cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(fps6 VERSION 1.0.0 LANGUAGES C CXX)

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(OPENGL REQUIRED)

include_directories("src" "glad/include" ${SDL_INCLUDE_DIR} ${SDL_MIXER_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

file(GLOB_RECURSE GLADSRC "glad/src/*.c")
add_library(GLAD STATIC ${GLADSRC})

file(GLOB_RECURSE FPS6SRC "src/*.cpp" "src/*.hpp")
add_executable(fps6 ${FPS6SRC})
target_link_libraries(fps6 GLAD ${SDL_LIBRARY} ${SDL_MIXER_LIBRARIES} ${OPENGL_LIBRARIES})

add_definitions(-DNOMINMAX)
