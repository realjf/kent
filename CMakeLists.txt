cmake_minimum_required(VERSION 3.5)

project(kent_app VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(engine)
add_subdirectory(editor)
add_subdirectory(sandbox)




