#include "Graficos.hpp"
#include <cmath>
#include <iostream>

bool Graficos::clipLine(Vertice& v1, Vertice& v2, float nearPlane) {
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
    float rotY = fmod(cam.rotY, 2*M_PI);
    float rotX = fmod(cam.rotX, 2*M_PI);
    
    float dx = v.x - cam.x;
    float dy = v.y - cam.y;
    float dz = v.z - cam.z;
    
    float cosY = cos(rotY), sinY = sin(rotY);
    float x1 = dx * cosY + dz * sinY;
    float z1 = -dx * sinY + dz * cosY;
    
    float cosX = cos(rotX), sinX = sin(rotX);
    v.y = dy * cosX - z1 * sinX;
    v.z = dy * sinX + z1 * cosX;
    v.x = x1;
    
    const float MAX_VAL = 1e6f;
    v.x = clamp(v.x, -MAX_VAL, MAX_VAL);
    v.y = clamp(v.y, -MAX_VAL, MAX_VAL);
    v.z = clamp(v.z, -MAX_VAL, MAX_VAL);
}

// Funciones originales
void Graficos::transformar(std::vector<Vertice>& vertices, float matriz[4][4]) {
    for (auto& v : vertices) {
        float x = v.x * matriz[0][0] + v.y * matriz[1][0] + v.z * matriz[2][0] + matriz[3][0];
        float y = v.x * matriz[0][1] + v.y * matriz[1][1] + v.z * matriz[2][1] + matriz[3][1];
        float z = v.x * matriz[0][2] + v.y * matriz[1][2] + v.z * matriz[2][2] + matriz[3][2];
        v = {x, y, z};
    }
}

void Graficos::proyectar(std::vector<Vertice>& vertices) {
    for (auto& v : vertices) {
        if (v.z != 0) {  // Protección contra división por cero
            v.x = v.x / v.z;
            v.y = v.y / v.z;
        }
    }
}