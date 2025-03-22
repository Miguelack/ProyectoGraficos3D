#include "Graficos.hpp"
#include <iostream>

// transforma los vertices usando una matriz 4x4
void Graficos::transformar(std::vector<Vertice>& vertices, float matriz[4][4]) {
    for (auto& v : vertices) {
        float x = v.x * matriz[0][0] + v.y * matriz[1][0] + v.z * matriz[2][0] + matriz[3][0];
        float y = v.x * matriz[0][1] + v.y * matriz[1][1] + v.z * matriz[2][1] + matriz[3][1];
        float z = v.x * matriz[0][2] + v.y * matriz[1][2] + v.z * matriz[2][2] + matriz[3][2];
        v = {x, y, z};
    }
}

// proyecta los vertices 3D en 2D (proyeccion simple)
void Graficos::proyectar(std::vector<Vertice>& vertices) {
    for (auto& v : vertices) {
        v.x = v.x / v.z; // proyeccion simple
        v.y = v.y / v.z;
    }
}