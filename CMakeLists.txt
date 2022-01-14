cmake_minimum_required(VERSION 3.20.0)
project(IFT-3100 VERSION 0.1.0)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include(CTest)
enable_testing()

include_directories(${OPENGL_INCLUDE_DIR})

set(SOURCE_FILES src/main.cpp)

add_executable(IFT-3100 ${SOURCE_FILES})

target_link_libraries(IFT-3100 OpenGL::GL glfw)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
