#ifndef GRAFICOS_HPP
#define GRAFICOS_HPP

#include <vector>
#include "Common/Vertice.hpp"  // Incluir la definicion de Vertice

class Graficos {
public:
    // transforma los vertices usando una matriz 4x4
    static void transformar(std::vector<Vertice>& vertices, float matriz[4][4]);

    // proyecta los vertices 3D en 2D (proyeccion simple)
    static void proyectar(std::vector<Vertice>& vertices);
};

#endif // GRAFICOS_HPP