
cmake_minimum_required(VERSION 3.20)

# Configurar vcpkg antes de inicializar el proyecto
set(CMAKE_TOOLCHAIN_FILE
    "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING ""
)

project(OpenGLPlayground LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar bibliotecas instaladas por vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(OpenGLPlayground
    main.cpp
)

target_link_libraries(OpenGLPlayground PRIVATE
    glfw
    glad::glad
    opengl32
)

# Copiar los shaders junto al ejecutable
if(EXISTS "${CMAKE_SOURCE_DIR}/shaders")
    add_custom_command(TARGET OpenGLPlayground POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:OpenGLPlayground>/shaders"
    )
endif()