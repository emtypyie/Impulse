cmake_minimum_required(VERSION 3.16)
project(Impulse LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Update these to match the Sandbox subfolder layout
include_directories(include)
include_directories(ThirdParty/raylib/include)
link_directories(ThirdParty/raylib/lib)

set(SOURCE_FILES
    src/Vector3d.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm gdi32 opengl32)
endif()
