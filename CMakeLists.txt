cmake_minimum_required(VERSION 3.10)
project(Practica3Grafica)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar paquetes necesarios
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Agregar el ejecutable
add_executable(circunferencias
    main.cpp
    primitivas.cpp
    escenas.cpp
)

# Enlazar librerías
target_link_libraries(circunferencias 
    PRIVATE 
    OpenGL::GL 
    GLEW::GLEW 
    glfw 
    glm::glm
)

target_include_directories(circunferencias PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
