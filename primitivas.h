#pragma once

#include <array>
#include <vector>

struct Color {
    float r;
    float g;
    float b;
};

struct Vertice {
    float x;
    float y;
    float r;
    float g;
    float b;
};

struct Circulo {
    int centroX;
    int centroY;
    int radio;
    Color color;
};

using BufferVertices = std::vector<Vertice>;
using PuntoReal = std::array<float, 2>;

void agregarPunto(BufferVertices& vertices, int x, int y, Color color);
void lineaBresenham(BufferVertices& vertices, int x0, int y0, int x1, int y1, Color color);

void circuloDosVias(BufferVertices& vertices, int x0, int y0, int radio, Color color);
void circuloCuatroVias(BufferVertices& vertices, int x0, int y0, int radio, Color color);
void circuloOchoVias(BufferVertices& vertices, int x0, int y0, int radio, Color color);
void circuloPuntoMedio(BufferVertices& vertices, int x0, int y0, int radio, Color color);

std::vector<PuntoReal> calcularIntersecciones(const Circulo& primero, const Circulo& segundo);
void dibujarMarcador(BufferVertices& vertices, int x, int y, Color color);