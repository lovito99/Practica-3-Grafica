#include "primitivas.h"

#include <algorithm>
#include <cmath>

namespace {

void dibujarOchoPuntos(BufferVertices& vertices, int x0, int y0, int x, int y, Color color) {
    const int magnitudY = std::abs(y);

    agregarPunto(vertices, x0 + x, y0 + magnitudY, color);
    agregarPunto(vertices, x0 + x, y0 - magnitudY, color);
    agregarPunto(vertices, x0 - x, y0 + magnitudY, color);
    agregarPunto(vertices, x0 - x, y0 - magnitudY, color);
    agregarPunto(vertices, x0 + magnitudY, y0 + x, color);
    agregarPunto(vertices, x0 + magnitudY, y0 - x, color);
    agregarPunto(vertices, x0 - magnitudY, y0 + x, color);
    agregarPunto(vertices, x0 - magnitudY, y0 - x, color);
}

int redondeoRaiz(int radio, int x) {
    const double valor = static_cast<double>(radio) * static_cast<double>(radio) -
                         static_cast<double>(x) * static_cast<double>(x);
    return static_cast<int>(std::floor(std::sqrt(std::max(0.0, valor)) + 0.5));
}

}

void agregarPunto(BufferVertices& vertices, int x, int y, Color color) {
    vertices.push_back({
        static_cast<float>(x),
        static_cast<float>(y),
        color.r,
        color.g,
        color.b
    });
}

void lineaBresenham(BufferVertices& vertices, int x0, int y0, int x1, int y1, Color color) {
    int xActual = x0;
    int yActual = y0;
    const int deltaX = std::abs(x1 - x0);
    const int deltaY = std::abs(y1 - y0);
    const int pasoX = (x0 < x1) ? 1 : -1;
    const int pasoY = (y0 < y1) ? 1 : -1;
    int error = deltaX - deltaY;

    while (true) {
        agregarPunto(vertices, xActual, yActual, color);
        if (xActual == x1 && yActual == y1) {
            break;
        }

        const int dobleError = error * 2;
        if (dobleError > -deltaY) {
            error -= deltaY;
            xActual += pasoX;
        }
        if (dobleError < deltaX) {
            error += deltaX;
            yActual += pasoY;
        }
    }
}

void circuloDosVias(BufferVertices& vertices, int x0, int y0, int radio, Color color) {
    agregarPunto(vertices, x0 + radio, y0, color);
    agregarPunto(vertices, x0 - radio, y0, color);

    for (int x = -radio + 1; x <= radio - 1; ++x) {
        const int y = redondeoRaiz(radio, x);
        agregarPunto(vertices, x0 + x, y0 + y, color);
        agregarPunto(vertices, x0 + x, y0 - y, color);
    }
}

void circuloCuatroVias(BufferVertices& vertices, int x0, int y0, int radio, Color color) {
    agregarPunto(vertices, x0, y0 + radio, color);
    agregarPunto(vertices, x0, y0 - radio, color);
    agregarPunto(vertices, x0 + radio, y0, color);
    agregarPunto(vertices, x0 - radio, y0, color);

    for (int x = 1; x <= radio - 1; ++x) {
        const int y = redondeoRaiz(radio, x);
        agregarPunto(vertices, x0 + x, y0 + y, color);
        agregarPunto(vertices, x0 + x, y0 - y, color);
        agregarPunto(vertices, x0 - x, y0 + y, color);
        agregarPunto(vertices, x0 - x, y0 - y, color);
    }
}

void circuloOchoVias(BufferVertices& vertices, int x0, int y0, int radio, Color color) {
    agregarPunto(vertices, x0, y0 + radio, color);
    agregarPunto(vertices, x0, y0 - radio, color);
    agregarPunto(vertices, x0 + radio, y0, color);
    agregarPunto(vertices, x0 - radio, y0, color);

    int x = 1;
    int y = redondeoRaiz(radio, x);

    while (x < y) {
        agregarPunto(vertices, x0 + x, y0 + y, color);
        agregarPunto(vertices, x0 + x, y0 - y, color);
        agregarPunto(vertices, x0 - x, y0 + y, color);
        agregarPunto(vertices, x0 - x, y0 - y, color);
        agregarPunto(vertices, x0 + y, y0 + x, color);
        agregarPunto(vertices, x0 + y, y0 - x, color);
        agregarPunto(vertices, x0 - y, y0 + x, color);
        agregarPunto(vertices, x0 - y, y0 - x, color);

        ++x;
        y = redondeoRaiz(radio, x);
    }

    if (x == y) {
        agregarPunto(vertices, x0 + x, y0 + y, color);
        agregarPunto(vertices, x0 + x, y0 - y, color);
        agregarPunto(vertices, x0 - x, y0 + y, color);
        agregarPunto(vertices, x0 - x, y0 - y, color);
    }
}

void circuloPuntoMedio(BufferVertices& vertices, int x0, int y0, int radio, Color color) {
    double hm = 1.25 - static_cast<double>(radio);
    int x = 0;
    int y = -radio;

    agregarPunto(vertices, x0, y0 + radio, color);
    agregarPunto(vertices, x0, y0 - radio, color);
    agregarPunto(vertices, x0 + radio, y0, color);
    agregarPunto(vertices, x0 - radio, y0, color);

    while (x < -(y + 1)) {
        if (hm < 0.0) {
            hm += (2.0 * static_cast<double>(x)) + 3.0;
        } else {
            hm += (2.0 * static_cast<double>(x)) + (2.0 * static_cast<double>(y)) + 5.0;
            ++y;
        }

        ++x;
        dibujarOchoPuntos(vertices, x0, y0, x, y, color);
    }
}

std::vector<PuntoReal> calcularIntersecciones(const Circulo& primero, const Circulo& segundo) {
    constexpr double epsilon = 1e-6;

    const double dx = static_cast<double>(segundo.centroX - primero.centroX);
    const double dy = static_cast<double>(segundo.centroY - primero.centroY);
    const double distancia = std::hypot(dx, dy);

    if (distancia < epsilon) {
        return {};
    }

    if (distancia > static_cast<double>(primero.radio + segundo.radio) + epsilon) {
        return {};
    }

    if (distancia < std::abs(static_cast<double>(primero.radio - segundo.radio)) - epsilon) {
        return {};
    }

    const double radioA = static_cast<double>(primero.radio);
    const double radioB = static_cast<double>(segundo.radio);
    const double a = ((radioA * radioA) - (radioB * radioB) + (distancia * distancia)) / (2.0 * distancia);
    const double h2 = (radioA * radioA) - (a * a);
    if (h2 < -epsilon) {
        return {};
    }

    const double h = std::sqrt(std::max(0.0, h2));
    const double baseX = static_cast<double>(primero.centroX) + (a * dx / distancia);
    const double baseY = static_cast<double>(primero.centroY) + (a * dy / distancia);
    const double offsetX = -dy * h / distancia;
    const double offsetY = dx * h / distancia;

    std::vector<PuntoReal> puntos;
    puntos.push_back({
        static_cast<float>(baseX + offsetX),
        static_cast<float>(baseY + offsetY)
    });

    if (h > epsilon) {
        puntos.push_back({
            static_cast<float>(baseX - offsetX),
            static_cast<float>(baseY - offsetY)
        });
    }

    return puntos;
}

void dibujarMarcador(BufferVertices& vertices, int x, int y, Color color) {
    lineaBresenham(vertices, x - 6, y, x + 6, y, color);
    lineaBresenham(vertices, x, y - 6, x, y + 6, color);
    lineaBresenham(vertices, x - 4, y - 4, x + 4, y + 4, color);
    lineaBresenham(vertices, x - 4, y + 4, x + 4, y - 4, color);
}