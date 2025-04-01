#include "Renderer.hpp"
#include "Graficos.hpp"
#include "CameraController.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp>

// Constantes de color
const sf::Color COLOR_CARA_OPACA(255, 0, 0, 255);    // Rojo completamente opaco
const sf::Color COLOR_ARISTAS(180, 140, 255, 255);   // Color aristas (morado claro)
const sf::Color COLOR_VERTICES(200, 160, 255, 255);  // Color vértices

void Renderer::generarCaras(const std::vector<Vertice>& /*vertices*/,
                          std::vector<std::vector<int>>& caras,
                          size_t tipoModelo) {
    caras.clear();
    
    if (tipoModelo == 8) { // Cubo
        caras = {
            {0,1,2,3}, {4,5,6,7}, {0,1,5,4},
            {2,3,7,6}, {0,3,7,4}, {1,2,6,5}
        };
    } 
    else if (tipoModelo == 5) { // Pirámide (orden corregido)
        caras = {
            {0,3,2,1},  // Base cuadrada (orden antihorario)
            {0,1,4},    // Caras laterales
            {1,2,4},
            {2,3,4},
            {3,0,4}
        };
    }
}

bool esCaraVisible(const Vertice& v1, const Vertice& v2, const Vertice& v3, const Vertice& camPos) {
    // Calcula vectores de la cara
    Vec3 edge1 = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
    Vec3 edge2 = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};
    
    // Calcula la normal de la cara
    Vec3 normal = crossProduct(edge1, edge2).normalize();
    
    // Vector de vista
    Vec3 view = {camPos.x - v1.x, camPos.y - v1.y, camPos.z - v1.z};
    
    // Producto punto
    float dot = normal.x * view.x + normal.y * view.y + normal.z * view.z;
    
    return dot > 0;
}

void Renderer::renderizarModelo(sf::RenderWindow& ventana, 
                              const std::vector<Vertice>& vertices,
                              const std::vector<std::pair<int, int>>& conexiones,
                              const Camara& camara,
                              ModoRenderizado modo) {
    if (vertices.empty() || !ventana.isOpen()) return;

    // Configuración de estados de renderizado
    sf::RenderStates states;
    states.blendMode = sf::BlendNone;

    // 1. Renderizar caras sólidas (opacas)
    if (modo == MODO_SOLIDO || modo == MODO_MIXTO) {
        std::vector<std::vector<int>> caras;
        generarCaras(vertices, caras, vertices.size());
        
        // Posición de la cámara para backface culling
        Vertice camPos = {camara.x, camara.y, camara.z};
        
        for (const auto& cara : caras) {
            if (cara.size() < 3) continue;

            // Verificar visibilidad de la cara
            Vertice v1 = vertices[cara[0]];
            Vertice v2 = vertices[cara[1]];
            Vertice v3 = vertices[cara.size() > 2 ? cara[2] : cara[1]]; // Para triángulos y quads
            
            CameraController::transformarVertice(v1, camara);
            CameraController::transformarVertice(v2, camara);
            CameraController::transformarVertice(v3, camara);
            
            if (!esCaraVisible(v1, v2, v3, camPos)) continue;

            sf::VertexArray poligono(cara.size() == 4 ? sf::Quads : sf::Triangles, cara.size());
            bool caraValida = true;
            
            for (size_t i = 0; i < cara.size(); ++i) {
                if (static_cast<size_t>(cara[i]) >= vertices.size()) {
                    caraValida = false;
                    break;
                }
                
                Vertice v = vertices[cara[i]];
                CameraController::transformarVertice(v, camara);
                
                if (v.z <= 0.1f) {
                    caraValida = false;
                    break;
                }
                
                poligono[i].position = Graficos::proyectarPunto(v);
                poligono[i].color = COLOR_CARA_OPACA;
            }
            
            if (caraValida) {
                ventana.draw(poligono, states);
            }
        }
    }

    // 2. Renderizar aristas (versión corregida para SFML)
    if (modo == MODO_LINEAS || modo == MODO_MIXTO) {
        std::vector<sf::Vertex> lineasVertices;
        
        for (const auto& conn : conexiones) {
            if (static_cast<size_t>(conn.first) >= vertices.size() || 
                static_cast<size_t>(conn.second) >= vertices.size()) {
                continue;
            }

            Vertice v1 = vertices[conn.first];
            Vertice v2 = vertices[conn.second];
            
            CameraController::transformarVertice(v1, camara);
            CameraController::transformarVertice(v2, camara);
            
            if (v1.z > 0.1f && v2.z > 0.1f && Graficos::recortarLinea(v1, v2)) {
                lineasVertices.emplace_back(Graficos::proyectarPunto(v1), COLOR_ARISTAS);
                lineasVertices.emplace_back(Graficos::proyectarPunto(v2), COLOR_ARISTAS);
            }
        }
        
        if (!lineasVertices.empty()) {
            ventana.draw(&lineasVertices[0], lineasVertices.size(), sf::Lines, states);
        }
    }
    
    // 3. Renderizar vértices (versión corregida para SFML)
    if (modo != MODO_SOLIDO) {
        std::vector<sf::Vertex> puntosVertices;
        
        for (size_t i = 0; i < vertices.size(); ++i) {
            Vertice v = vertices[i];
            CameraController::transformarVertice(v, camara);
            
            if (v.z > 0.1f) {
                sf::Vector2f pos = Graficos::proyectarPunto(v);
                if (pos.x > -1000.0f) {
                    puntosVertices.emplace_back(pos, COLOR_VERTICES);
                }
            }
        }
        
        if (!puntosVertices.empty()) {
            ventana.draw(&puntosVertices[0], puntosVertices.size(), sf::Points, states);
        }
    }
}

void Renderer::configurarConexiones(const std::vector<Vertice>& vertices, 
                                  std::vector<std::pair<int, int>>& conexiones) {
    conexiones.clear();

    if (vertices.size() == 8) { // Cubo
        conexiones = {
            {0,1}, {1,2}, {2,3}, {3,0},  // Cara frontal
            {4,5}, {5,6}, {6,7}, {7,4},  // Cara trasera
            {0,4}, {1,5}, {2,6}, {3,7},  // Aristas laterales
            {0,5}, {1,4}, {2,7}, {3,6}   // Diagonales de cara
        };
    } 
    else if (vertices.size() == 5) { // Pirámide
        conexiones = {
            {0,1}, {1,2}, {2,3}, {3,0},  // Base cuadrada
            {0,4}, {1,4}, {2,4}, {3,4}   // Aristas laterales
        };
    }
}