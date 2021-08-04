cmake_minimum_required(VERSION 3.5)

project(kent_sandbox VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# 
add_executable(${PROJECT_NAME} src/sandbox_app.cpp)

target_link_libraries(${PROJECT_NAME} kent_engine)
target_include_directories(${PROJECT_NAME} PRIVATE kent_engine)

