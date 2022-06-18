cmake_minimum_required(VERSION 3.18)

project(atoc)

# sources
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp" "src/*.tcc")
file(GLOB_RECURSE SOURCES "src/*.c" "src/*.cpp")

# settings
set(CMAKE_CXX_STANDARD 17)

# libs
add_subdirectory(liballegrex)
# its 2022 and cmake still does not have include aliases
include_directories(liballegrex/src)

# includes
include_directories(${CMAKE_SOURCE_DIR}/src)

# target lib
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} allegrex)

add_subdirectory(tests)
