# Práctica 03: Algoritmos de Generación de Circunferencias

Este proyecto implementa los algoritmos fundamentales para la generación de circunferencias en **C++** utilizando **OpenGL moderno (3.3+)**. [cite_start]El objetivo es visualizar y comparar la eficiencia de diferentes métodos de rasterización de curvas[cite: 5, 8].

## Requisitos del Sistema

Para compilar y ejecutar este proyecto, necesitas tener instalados:

* **Compilador C++**: Soporte para C++17 (GCC, Clang o MSVC).
* **CMake**: Versión 3.10 o superior.
* **Librerías de Desarrollo**:
    * **GLEW**: Extensiones de OpenGL.
    * **GLFW**: Gestión de ventanas y eventos.
    * **GLM**: Librería matemática para gráficos (matrices de proyección).

## Estructura de Archivos

* [cite_start]`main.cpp`: Contiene las implementaciones de los algoritmos (2-vías, 4-vías, 8-vías y Punto Medio)[cite: 10, 55, 74, 102].
* `CMakeLists.txt`: Archivo de configuración para la generación del proyecto.
* `shader.vert` / `shader.frag`: Código de los Shaders para procesamiento en GPU.

## Instrucciones de Compilación

Sigue estos pasos desde una terminal en la carpeta raíz del proyecto:

1. **Crear el directorio de construcción**:
   ```bash
   mkdir build
   cd build

2. **Configurar el proyecto con CMake:
Esto preparará los archivos necesarios para tu compilador específico.
   ```bash
   cmake ..

3. Compilar el ejecutable:
   ```bash
   cmake --build .


