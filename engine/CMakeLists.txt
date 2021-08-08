cmake_minimum_required(VERSION 3.5)
project(kent_engine VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external/glm)
add_subdirectory(external/glfw)
add_subdirectory(external/vk-bootstrap)

find_package(Vulkan REQUIRED)

file(GLOB SOURCES "src/*.cpp", "src/*/*.cpp", "src/*/*/*.cpp", "src/*/*/*/*.cpp")

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        glm
        glfw
        ${Vulkan_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE
        vk-bootstrap
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC
        glm
        glfw
        ${Vulkan_LIBRARIES}
    PRIVATE    
        vk-bootstrap
)


file (GLOB SHADERS shaders/*.frag shaders/*.vert)


add_custom_command(TARGET ${PROJECT_NAME} 
    PRE_BUILD 
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/sandbox/shaders" "${CMAKE_BINARY_DIR}/editor/shaders"
    )

foreach(shader ${SHADERS})
    get_filename_component(SHADER_NAME ${shader} NAME)
    add_custom_command(TARGET ${PROJECT_NAME}
     PRE_BUILD  
     COMMAND glslc.exe -c ${shader} -o "${CMAKE_BINARY_DIR}/sandbox/shaders/${SHADER_NAME}.spv")
    add_custom_command(TARGET ${PROJECT_NAME}
     PRE_BUILD  
     COMMAND glslc.exe -c ${shader} -o "${CMAKE_BINARY_DIR}/editor/shaders/${SHADER_NAME}.spv")
endforeach()

