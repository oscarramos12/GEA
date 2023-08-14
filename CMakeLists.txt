cmake_minimum_required(VERSION 3.22)

project(GEA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_BUILD_TYPE Debug)

file(
  GLOB_RECURSE
  SOURCES
  CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

#sdl
find_package(SDL2 REQUIRED)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  ${SOURCES}
)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/src
  PUBLIC ${PROJECT_SOURCE_DIR}/src/Game
  PUBLIC ${PROJECT_SOURCE_DIR}/src/Scene
  PUBLIC ${PROJECT_SOURCE_DIR}/src/ECS
  PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
  ${PROJECT_NAME}
  ${SDL2_LIBRARIES}
)
