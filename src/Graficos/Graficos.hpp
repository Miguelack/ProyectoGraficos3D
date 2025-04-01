#ifndef GRAFICOS_HPP
#define GRAFICOS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "Common/Vertice.hpp"
#include "CameraController.hpp"

// Estructura para representar un vector 3D sin GLM
struct Vec3 {
    float x, y, z;
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    
    // Normaliza el vector
    Vec3 normalize() const {
        float len = std::sqrt(x*x + y*y + z*z);
        if (len > 0.0001f) {
            return Vec3(x/len, y/len, z/len);
        }
        return *this;
    }
};

// Calcula el producto cruz entre dos vectores
inline Vec3 crossProduct(const Vec3& a, const Vec3& b) {
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

// Versión sin GLM de calcularNormal
inline Vec3 calcularNormal(const Vertice& v1, const Vertice& v2, const Vertice& v3) {
    Vec3 edge1 = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
    Vec3 edge2 = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};
    return crossProduct(edge1, edge2).normalize();
}

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