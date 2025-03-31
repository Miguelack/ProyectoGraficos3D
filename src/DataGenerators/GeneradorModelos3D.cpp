#include "GeneradorModelos3D.hpp"
#include <cmath>
#include <iostream>  // Incluir esta cabecera para std::cout

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

void GeneradorModelos3D::imprimirVertices(const std::vector<Vertice>& vertices) {
    std::cout << "\n=== Vertices del Modelo ===\n";
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << "V" << i << ": (" << vertices[i].x << ", " 
                  << vertices[i].y << ", " << vertices[i].z << ")\n";
    }
    std::cout << "=========================\n";
}