#include "Graficos.hpp"
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>

bool Graficos::recortarLinea(Vertice& v1, Vertice& v2, float nearPlane) {
    if (v1.z <= nearPlane && v2.z <= nearPlane) return false;
    
    if (v1.z <= nearPlane) {
        float t = (nearPlane - v2.z) / (v1.z - v2.z);
        t = Graficos::clamp(t, 0.0f, 1.0f);
        v1.x = v2.x + t * (v1.x - v2.x);
        v1.y = v2.y + t * (v1.y - v2.y);
        v1.z = nearPlane;
    } 
    else if (v2.z <= nearPlane) {
        float t = (nearPlane - v1.z) / (v2.z - v1.z);
        t = Graficos::clamp(t, 0.0f, 1.0f);
        v2.x = v1.x + t * (v2.x - v1.x);
        v2.y = v1.y + t * (v2.y - v1.y);
        v2.z = nearPlane;
    }
    
    return !std::isnan(v1.x) && !std::isnan(v1.y) && !std::isnan(v1.z) &&
           !std::isnan(v2.x) && !std::isnan(v2.y) && !std::isnan(v2.z);
}

sf::Vector2f Graficos::proyectarPunto(const Vertice& v, float scale) {
    if (v.z > 0.5f) { // Aumentamos el near plane
        float factor = 500.0f / (v.z + 500.0f);
        float x = 512.f + v.x * factor * scale;
        float y = 384.f + v.y * factor * scale;
        
        if (!std::isnan(x) && !std::isnan(y)) {
            return sf::Vector2f(x, y);
        }
    }
    return sf::Vector2f(-1000, -1000);
}

void Graficos::transformarVertice(Vertice& v, const Camara& cam) {
    // Convertir a coordenadas relativas a la cámara
    float dx = v.x - cam.x;
    float dy = v.y - cam.y;
    float dz = v.z - cam.z;
    
    // Rotación en Y (horizontal)
    float cosY = cos(cam.rotY), sinY = sin(cam.rotY);
    float x1 = dx * cosY + dz * sinY;
    float z1 = -dx * sinY + dz * cosY;
    
    // Rotación en X (vertical)
    float cosX = cos(cam.rotX), sinX = sin(cam.rotX);
    v.y = dy * cosX - z1 * sinX;
    v.z = dy * sinX + z1 * cosX;
    v.x = x1;
    
    // Limitar valores extremos
    const float MAX_VAL = 1e6f;
    v.x = clamp(v.x, -MAX_VAL, MAX_VAL);
    v.y = clamp(v.y, -MAX_VAL, MAX_VAL);
    v.z = clamp(v.z, -MAX_VAL, MAX_VAL);
}