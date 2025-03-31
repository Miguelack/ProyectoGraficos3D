#ifndef GENERADORMODELOS3D_HPP
#define GENERADORMODELOS3D_HPP

#include <vector>
#include "Common/Vertice.hpp"

class GeneradorModelos3D {
public:
    static std::vector<Vertice> generarCubo(float tamano);
    static std::vector<Vertice> generarPiramide(float base, float altura);
    
    // Nuevo método para visualización de debug
    static void imprimirVertices(const std::vector<Vertice>& vertices);
};

#endif