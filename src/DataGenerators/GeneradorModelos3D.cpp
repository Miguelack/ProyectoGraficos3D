#include "GeneradorModelos3D.hpp"
#include <cmath>

// genera un cubo simple
std::vector<Vertice> GeneradorModelos3D::generarCubo(float tamano) {
    std::vector<Vertice> vertices;
    float halfTamano = tamano / 2.0f;

    // vertices del cubo
    vertices = {
        {-halfTamano, -halfTamano, -halfTamano},
        {halfTamano, -halfTamano, -halfTamano},
        {halfTamano, halfTamano, -halfTamano},
        {-halfTamano, halfTamano, -halfTamano},
        {-halfTamano, -halfTamano, halfTamano},
        {halfTamano, -halfTamano, halfTamano},
        {halfTamano, halfTamano, halfTamano},
        {-halfTamano, halfTamano, halfTamano}
    };

    return vertices;
}

// genera una piramide simple
std::vector<Vertice> GeneradorModelos3D::generarPiramide(float base, float altura) {
    std::vector<Vertice> vertices;
    float halfBase = base / 2.0f;

    // base de la piramide
    vertices = {
        {-halfBase, -halfBase, 0},
        {halfBase, -halfBase, 0},
        {halfBase, halfBase, 0},
        {-halfBase, halfBase, 0},
        {0, 0, altura} // vertice superior
    };

    return vertices;
}