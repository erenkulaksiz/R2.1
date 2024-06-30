cmake_minimum_required(VERSION 3.8)

project(R2)

set(ASSIMP_DLL "${CMAKE_SOURCE_DIR}/lib/assimp-vc143-mtd.dll")
set(ASSIMP_LIB "${CMAKE_SOURCE_DIR}/lib/assimp-vc143-mtd.lib")

set(ASSIMP_BUILD_STATIC_LIB OFF)
set(ASSIMP_INSTALL OFF)

link_directories(lib)
include_directories(
    include 
    imgui
)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_definitions(-D__R2_DEBUG)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
file(GLOB IMGUI_SOURCES 
    ${IMGUI_DIR}/*.cpp 
    ${IMGUI_DIR}/*.h 
    ${IMGUI_DIR}/backends/imgui_impl_glfw.h 
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.h 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

file(GLOB_RECURSE SRC_SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

set(SOURCES 
    ${SRC_SOURCES}
    ${IMGUI_SOURCES}
)

find_package(OpenGL REQUIRED)

add_executable(R2 ${SOURCES})

target_link_libraries(R2 ${OPENGL_LIBRARIES} glfw3 ${ASSIMP_LIB})

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET R2 PROPERTY CXX_STANDARD 20)
endif()

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

add_custom_command(TARGET R2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/app.xml"
    $<TARGET_FILE_DIR:R2>
    COMMAND ${CMAKE_COMMAND} -E copy
    "${ASSIMP_DLL}"
    $<TARGET_FILE_DIR:R2>
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/resources"
    "$<TARGET_FILE_DIR:R2>/resources"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/shaders"
    "$<TARGET_FILE_DIR:R2>/shaders"
)