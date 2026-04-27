# Practica 3: Lineas, Circunferencias e Intersecciones

El proyecto ahora contiene una aplicacion OpenGL en C++17 con dos escenas:

1. Una figura compuesta usando un algoritmo de lineas y los cuatro algoritmos de circunferencia solicitados.
2. Una escena con 5 circulos aleatorios y el marcado de sus intersecciones.

## Algoritmos implementados

- Simetria de dos vias
- Simetria de cuatro vias
- Simetria de ocho vias
- Punto medio
- Bresenham para lineas

## Archivos principales

- main.cpp: inicializacion de OpenGL, carga de shaders y cambio de escena.
- primitivas.h / primitivas.cpp: algoritmos de lineas, circunferencias e interseccion entre circulos.
- escenas.h / escenas.cpp: construccion de la figura compuesta y de la escena aleatoria.
- shader.vert / shader.frag: shaders con color por vertice.

## Controles

- Tecla 1: figura compuesta.
- Tecla 2: 5 circulos aleatorios con intersecciones.
- Tecla R: regenera la escena de circulos aleatorios.

## Compilacion

```bash
mkdir build
cd build
cmake ..
cmake --build .
```


