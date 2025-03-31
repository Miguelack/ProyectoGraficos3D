#include "Renderer.hpp"
#include "CameraController.hpp"
#include "Graficos/Graficos.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>

void Renderer::setupConnections(const std::vector<Vertice>& vertices, 
                              std::vector<std::pair<int, int>>& connections) {
    connections.clear();
    
    if (vertices.size() == 8) {
        // Conexiones para cubo (24 aristas)
        connections = {
            // Cara frontal (4 aristas)
            {0,1}, {1,2}, {2,3}, {3,0},
            // Cara trasera (4 aristas)
            {4,5}, {5,6}, {6,7}, {7,4},
            // Aristas laterales (4 aristas)
            {0,4}, {1,5}, {2,6}, {3,7},
            // Diagonales de cara (4 aristas)
            {0,5}, {1,4}, {2,7}, {3,6},
            // Diagonales espaciales (4 aristas adicionales)
            {0,6}, {1,7}, {2,4}, {3,5}
        };
    } else if (vertices.size() == 5) {
        // Conexiones para pirámide (8 aristas)
        connections = {
            // Base cuadrada (4 aristas)
            {0,1}, {1,2}, {2,3}, {3,0},
            // Aristas laterales (4 aristas)
            {0,4}, {1,4}, {2,4}, {3,4}
        };
    }
}

void Renderer::renderModel(sf::RenderWindow& window, 
                         const std::vector<Vertice>& vertices,
                         const std::vector<std::pair<int, int>>& connections,
                         const Camera& camera) {
    // Validación de parámetros
    if (vertices.empty() || connections.empty() || !window.isOpen()) return;

    // Dibujar conexiones entre vértices
    for (const auto& conn : connections) {
        // Verificar índices válidos con cast seguro
        if (static_cast<size_t>(conn.first) >= vertices.size() || 
            static_cast<size_t>(conn.second) >= vertices.size()) {
            continue;
        }

        // Obtener vértices de la conexión
        Vertice v1 = vertices[conn.first];
        Vertice v2 = vertices[conn.second];
        
        // Transformar vértices al espacio de la cámara
        CameraController::transformVertex(v1, camera);
        CameraController::transformVertex(v2, camera);
        
        // Aplicar clipping y proyección
        if (Graficos::clipLine(v1, v2)) {
            const sf::Vector2f p1 = Graficos::proyectarPunto(v1);
            const sf::Vector2f p2 = Graficos::proyectarPunto(v2);
            
            // Dibujar línea si los puntos son válidos
            if (p1.x > -1000.0f && p2.x > -1000.0f) {
                const sf::Vertex line[] = {
                    sf::Vertex(p1, sf::Color(180, 140, 255)),
                    sf::Vertex(p2, sf::Color(180, 140, 255))
                };
                window.draw(line, 2, sf::Lines);
            }
        }
    }
    
    // Dibujar vértices como puntos
    for (const auto& vertex : vertices) {
        Vertice v = vertex;
        CameraController::transformVertex(v, camera);
        
        // Solo dibujar si está delante del plano cercano
        if (v.z > 0.1f) {
            const sf::Vector2f pos = Graficos::proyectarPunto(v);
            if (pos.x > -1000.0f) {
                sf::CircleShape point(2.0f);
                point.setFillColor(sf::Color(200, 160, 255));
                point.setPosition(pos.x - 2.0f, pos.y - 2.0f);
                window.draw(point);
            }
        }
    }
}