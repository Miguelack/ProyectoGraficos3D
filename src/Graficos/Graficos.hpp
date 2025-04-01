// directiva para evitar inclusiones múltiples
#ifndef GRAFICOS_HPP
#define GRAFICOS_HPP

// incluye librería gráfica SFML
#include <SFML/Graphics.hpp>
// incluye contenedor vector
#include <vector>
// incluye funciones matemáticas
#include <cmath>
// incluye definición de vértice
#include "Common/Vertice.hpp"
// incluye controlador de cámara
#include "CameraController.hpp"

// estructura para representar vector 3D sin usar GLM
struct Vec3 {
    // componentes del vector
    float x, y, z;
    // constructor con valores por defecto
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    
    // método para normalizar el vector
    Vec3 normalize() const {
        // calcula longitud del vector
        float len = std::sqrt(x*x + y*y + z*z);
        // evita división por cero
        if (len > 0.0001f) {
            // devuelve vector normalizado
            return Vec3(x/len, y/len, z/len);
        }
        // devuelve vector original si es muy pequeño
        return *this;
    }
};

// función para calcular producto cruz entre vectores
inline Vec3 crossProduct(const Vec3& a, const Vec3& b) {
    // calcula cada componente del producto cruz
    return Vec3(
        a.y * b.z - a.z * b.y,  // componente x
        a.z * b.x - a.x * b.z,  // componente y
        a.x * b.y - a.y * b.x   // componente z
    );
}

// calcula normal de triángulo sin usar GLM
inline Vec3 calcularNormal(const Vertice& v1, const Vertice& v2, const Vertice& v3) {
    // calcula primer borde del triángulo
    Vec3 edge1 = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
    // calcula segundo borde del triángulo
    Vec3 edge2 = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};
    // devuelve producto cruz normalizado
    return crossProduct(edge1, edge2).normalize();
}

// clase principal de gráficos
class Graficos {
public:
    // implementación de clamp compatible con C++11
    template <typename T>
    static const T& clamp(const T& value, const T& min, const T& max) {
        // devuelve valor acotado entre mínimo y máximo
        return (value < min) ? min : (max < value) ? max : value;
    }

    // función para recortar líneas contra plano cercano
    static bool recortarLinea(Vertice& v1, Vertice& v2, float nearPlane = 0.5f);
    // función para proyectar punto 3D a 2D
    static sf::Vector2f proyectarPunto(const Vertice& v, float scale = 50.0f);
    // transforma vértice al espacio de la cámara
    static void transformarVertice(Vertice& v, const Camara& cam);
    
    // funciones originales de transformación
    static void transformar(std::vector<Vertice>& vertices, float matriz[4][4]);
    // función para proyectar lista de vértices
    static void proyectar(std::vector<Vertice>& vertices);
};

// fin de la directiva ifndef
#endif // GRAFICOS_HPP