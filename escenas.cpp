#include "escenas.h"

#include <array>
#include <cmath>
#include <cstdint>
#include <random>
#include <vector>

namespace {

using AlgoritmoCirculo = void (*)(BufferVertices&, int, int, int, Color);

const Color colorCarbon {0.10f, 0.15f, 0.22f};
const Color colorAzul {0.12f, 0.39f, 0.82f};
const Color colorCeleste {0.24f, 0.67f, 0.93f};
const Color colorArena {0.88f, 0.76f, 0.52f};
const Color colorSol {0.94f, 0.72f, 0.15f};
const Color colorRojo {0.87f, 0.23f, 0.18f};
const Color colorVerde {0.18f, 0.60f, 0.34f};
const Color colorMagenta {0.70f, 0.23f, 0.54f};

void segmentoHorizontal(BufferVertices& vertices, int x, int y, int longitud, Color color) {
    lineaBresenham(vertices, x, y, x + longitud, y, color);
}

void rectangulo(BufferVertices& vertices, int x1, int y1, int x2, int y2, Color color) {
    lineaBresenham(vertices, x1, y1, x2, y1, color);
    lineaBresenham(vertices, x2, y1, x2, y2, color);
    lineaBresenham(vertices, x2, y2, x1, y2, color);
    lineaBresenham(vertices, x1, y2, x1, y1, color);
}

void techo(BufferVertices& vertices, Color color) {
    lineaBresenham(vertices, 250, 340, 360, 460, color);
    lineaBresenham(vertices, 360, 460, 560, 460, color);
    lineaBresenham(vertices, 560, 460, 670, 340, color);
}

std::vector<Circulo> generarCirculos(unsigned int semilla) {
    std::mt19937 generador(semilla);
    std::uniform_int_distribution<int> distribucionX(220, 740);
    std::uniform_int_distribution<int> distribucionY(200, 520);
    std::uniform_int_distribution<int> distribucionRadio(70, 130);

    const std::array<Color, 5> paleta = {
        colorAzul,
        colorVerde,
        colorMagenta,
        colorArena,
        colorCeleste
    };

    std::vector<Circulo> circulos;
    for (int intento = 0; intento < 64; ++intento) {
        circulos.clear();
        for (int indice = 0; indice < 5; ++indice) {
            circulos.push_back({
                distribucionX(generador),
                distribucionY(generador),
                distribucionRadio(generador),
                paleta[static_cast<std::size_t>(indice)]
            });
        }

        int cruces = 0;
        for (std::size_t i = 0; i < circulos.size(); ++i) {
            for (std::size_t j = i + 1; j < circulos.size(); ++j) {
                cruces += static_cast<int>(calcularIntersecciones(circulos[i], circulos[j]).size());
            }
        }

        if (cruces >= 2) {
            return circulos;
        }

        generador.seed(semilla + static_cast<unsigned int>((intento + 1) * 37));
    }

    return circulos;
}

}

BufferVertices crearEscenaFigura() {
    BufferVertices vertices;

    lineaBresenham(vertices, 40, 150, 920, 150, colorCarbon);
    lineaBresenham(vertices, 60, 143, 900, 143, colorCarbon);
    lineaBresenham(vertices, 270, 340, 650, 340, colorAzul);

    rectangulo(vertices, 180, 220, 720, 340, colorAzul);
    techo(vertices, colorAzul);
    lineaBresenham(vertices, 180, 220, 720, 220, colorAzul);
    lineaBresenham(vertices, 420, 340, 420, 460, colorAzul);
    lineaBresenham(vertices, 310, 340, 310, 430, colorAzul);
    lineaBresenham(vertices, 530, 340, 530, 430, colorAzul);
    lineaBresenham(vertices, 338, 430, 392, 430, colorCeleste);
    lineaBresenham(vertices, 448, 430, 502, 430, colorCeleste);
    lineaBresenham(vertices, 600, 220, 610, 220, colorCarbon);
    lineaBresenham(vertices, 300, 220, 320, 220, colorCarbon);

    rectangulo(vertices, 330, 360, 400, 430, colorCeleste);
    rectangulo(vertices, 440, 360, 510, 430, colorCeleste);
    segmentoHorizontal(vertices, 392, 322, 18, colorCarbon);
    segmentoHorizontal(vertices, 502, 322, 18, colorCarbon);

    lineaBresenham(vertices, 180, 270, 140, 270, colorRojo);
    lineaBresenham(vertices, 180, 290, 140, 290, colorRojo);
    lineaBresenham(vertices, 720, 270, 760, 270, colorRojo);
    lineaBresenham(vertices, 720, 290, 760, 290, colorRojo);
    lineaBresenham(vertices, 720, 235, 736, 235, colorRojo);

    circuloCuatroVias(vertices, 310, 220, 70, colorCarbon);
    circuloPuntoMedio(vertices, 310, 220, 28, colorArena);
    circuloOchoVias(vertices, 590, 220, 70, colorCarbon);
    circuloDosVias(vertices, 590, 220, 28, colorArena);
    circuloDosVias(vertices, 780, 560, 54, colorSol);
    circuloCuatroVias(vertices, 780, 560, 18, colorSol);
    circuloOchoVias(vertices, 120, 585, 38, colorVerde);
    circuloPuntoMedio(vertices, 840, 255, 20, colorRojo);

    for (int angulo = 0; angulo < 360; angulo += 30) {
        const double radianes = static_cast<double>(angulo) * 3.14159265358979323846 / 180.0;
        const int x = 780 + static_cast<int>(std::round(std::cos(radianes) * 92.0));
        const int y = 560 + static_cast<int>(std::round(std::sin(radianes) * 92.0));
        lineaBresenham(vertices, 780, 560, x, y, colorSol);
    }

    lineaBresenham(vertices, 120, 550, 120, 630, colorVerde);
    lineaBresenham(vertices, 120, 630, 95, 662, colorVerde);
    lineaBresenham(vertices, 120, 630, 145, 662, colorVerde);
    lineaBresenham(vertices, 120, 630, 120, 668, colorVerde);
    lineaBresenham(vertices, 120, 595, 120, 620, colorVerde);
    lineaBresenham(vertices, 820, 255, 860, 255, colorRojo);

    return vertices;
}

BufferVertices crearEscenaIntersecciones() {
    BufferVertices vertices;
    std::random_device dispositivo;
    const unsigned int semilla =
        (static_cast<unsigned int>(dispositivo()) << 1u) ^
        static_cast<unsigned int>(std::uintptr_t(&vertices));
    const std::vector<Circulo> circulos = generarCirculos(semilla);
    const std::array<AlgoritmoCirculo, 4> algoritmos = {
        circuloDosVias,
        circuloCuatroVias,
        circuloOchoVias,
        circuloPuntoMedio
    };

    rectangulo(vertices, 80, 80, 880, 640, colorCarbon);

    for (std::size_t indice = 0; indice < circulos.size(); ++indice) {
        const Circulo& circulo = circulos[indice];
        const AlgoritmoCirculo algoritmo = algoritmos[indice % algoritmos.size()];
        algoritmo(vertices, circulo.centroX, circulo.centroY, circulo.radio, circulo.color);
        dibujarMarcador(vertices, circulo.centroX, circulo.centroY, colorCarbon);
    }

    for (std::size_t i = 0; i < circulos.size(); ++i) {
        for (std::size_t j = i + 1; j < circulos.size(); ++j) {
            const auto puntos = calcularIntersecciones(circulos[i], circulos[j]);
            for (const PuntoReal& punto : puntos) {
                dibujarMarcador(
                    vertices,
                    static_cast<int>(std::lround(punto[0])),
                    static_cast<int>(std::lround(punto[1])),
                    colorRojo);
            }
        }
    }

    return vertices;
}