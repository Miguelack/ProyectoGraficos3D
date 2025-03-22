#ifndef GENERADORMODELOS3D_HPP
#define GENERADORMODELOS3D_HPP

#include <vector>
#include "Common/Vertice.hpp"  // Incluir la definicion de Vertice

class GeneradorModelos3D {
public:
    // genera un cubo simple
    static std::vector<Vertice> generarCubo(float tamano);

    // genera una piramide simple
    static std::vector<Vertice> generarPiramide(float base, float altura);
};

#endif // GENERADORMODELOS3D_HPP