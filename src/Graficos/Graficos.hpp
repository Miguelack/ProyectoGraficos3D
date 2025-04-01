#ifndef GRAFICOS_HPP
#define GRAFICOS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Common/Vertice.hpp"
#include "CameraController.hpp"  // Incluir la definición de Camara desde aquí

class Graficos {
public:
    // Implementación propia de clamp para C++11
    template <typename T>
    static const T& clamp(const T& value, const T& min, const T& max) {
        return (value < min) ? min : (max < value) ? max : value;
    }

    static bool recortarLinea(Vertice& v1, Vertice& v2, float nearPlane = 0.5f);
    static sf::Vector2f proyectarPunto(const Vertice& v, float scale = 50.0f);
    static void transformarVertice(Vertice& v, const Camara& cam);
    
    // Funciones originales
    static void transformar(std::vector<Vertice>& vertices, float matriz[4][4]);
    static void proyectar(std::vector<Vertice>& vertices);
};

#endif // GRAFICOS_HPP