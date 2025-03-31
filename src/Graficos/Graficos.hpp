#ifndef GRAFICOS_HPP
#define GRAFICOS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Common/Vertice.hpp"

// Definición completa de Camara para evitar problemas
struct Camara {
    float x, y, z;
    float rotX, rotY;
    float velocidadMovimiento;
    float velocidadRotacion;
    float targetX, targetY, targetZ;
    float lerpFactor;
    
    Camara() : x(0), y(0), z(5.0f), rotX(0), rotY(0), 
              velocidadMovimiento(0.2f), velocidadRotacion(0.05f),
              targetX(0), targetY(0), targetZ(5.0f), lerpFactor(0.2f) {}
};

class Graficos {
public:
    // Implementación propia de clamp para C++11
    template <typename T>
    static const T& clamp(const T& value, const T& min, const T& max) {
        return (value < min) ? min : (max < value) ? max : value;
    }

    static bool clipLine(Vertice& v1, Vertice& v2, float nearPlane = 0.5f);
    static sf::Vector2f proyectarPunto(const Vertice& v, float scale = 50.0f);
    static void transformarVertice(Vertice& v, const Camara& cam);
    
    // Funciones originales
    static void transformar(std::vector<Vertice>& vertices, float matriz[4][4]);
    static void proyectar(std::vector<Vertice>& vertices);
};

#endif // GRAFICOS_HPP