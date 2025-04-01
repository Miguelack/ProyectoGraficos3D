// archivo de cabecera de la clase Renderer
#include "Renderer.hpp"
// controlador de cámara para transformaciones
#include "CameraController.hpp"
// funciones auxiliares de gráficos 3D
#include "Graficos/Graficos.hpp"
// biblioteca para gráficos de SFML
#include <SFML/Graphics.hpp>
// biblioteca con algoritmos útiles
#include <algorithm>

// configura las conexiones entre vértices para diferentes modelos 3D
void Renderer::configurarConexiones(const std::vector<Vertice>& vertices, 
                                  std::vector<std::pair<int, int>>& conexiones) {
    // limpia las conexiones existentes
    conexiones.clear();
    
    // configura conexiones para un cubo (8 vértices)
    if (vertices.size() == 8) {
        // 24 aristas en total para el cubo
        conexiones = {
            // cara frontal (4 aristas)
            {0,1}, {1,2}, {2,3}, {3,0},
            // cara trasera (4 aristas)
            {4,5}, {5,6}, {6,7}, {7,4},
            // aristas laterales (4 aristas)
            {0,4}, {1,5}, {2,6}, {3,7},
            // diagonales de cara (4 aristas)
            {0,5}, {1,4}, {2,7}, {3,6},
            // diagonales espaciales (4 aristas adicionales)
            {0,6}, {1,7}, {2,4}, {3,5}
        };
    } 
    // configura conexiones para una pirámide (5 vértices)
    else if (vertices.size() == 5) {
        // 8 aristas en total para la pirámide
        conexiones = {
            // base cuadrada (4 aristas)
            {0,1}, {1,2}, {2,3}, {3,0},
            // aristas laterales (4 aristas)
            {0,4}, {1,4}, {2,4}, {3,4}
        };
    }
}

// renderiza un modelo 3D en la ventana especificada
void Renderer::renderizarModelo(sf::RenderWindow& ventana, 
                              const std::vector<Vertice>& vertices,
                              const std::vector<std::pair<int, int>>& conexiones,
                              const Camara& camara) {
    // validación de parámetros de entrada
    if (vertices.empty() || conexiones.empty() || !ventana.isOpen()) return;

    // dibuja todas las conexiones (aristas) del modelo
    for (const auto& conexion : conexiones) {
        // verifica que los índices de vértices sean válidos
        if (static_cast<size_t>(conexion.first) >= vertices.size() || 
            static_cast<size_t>(conexion.second) >= vertices.size()) {
            continue;
        }

        // obtiene los vértices de la conexión actual
        Vertice v1 = vertices[conexion.first];
        Vertice v2 = vertices[conexion.second];
        
        // transforma los vértices al espacio de la cámara
        CameraController::transformarVertice(v1, camara);
        CameraController::transformarVertice(v2, camara);
        
        // aplica recorte de línea (clipping) y proyección
        if (Graficos::recortarLinea(v1, v2)) {
            const sf::Vector2f p1 = Graficos::proyectarPunto(v1);
            const sf::Vector2f p2 = Graficos::proyectarPunto(v2);
            
            // dibuja la línea si los puntos son válidos
            if (p1.x > -1000.0f && p2.x > -1000.0f) {
                const sf::Vertex linea[] = {
                    sf::Vertex(p1, sf::Color(180, 140, 255)),
                    sf::Vertex(p2, sf::Color(180, 140, 255))
                };
                ventana.draw(linea, 2, sf::Lines);
            }
        }
    }
    
    // dibuja los vértices como puntos
    for (const auto& vertice : vertices) {
        Vertice v = vertice;
        CameraController::transformarVertice(v, camara);
        
        // solo dibuja si está delante del plano cercano
        if (v.z > 0.1f) {
            const sf::Vector2f pos = Graficos::proyectarPunto(v);
            if (pos.x > -1000.0f) {
                sf::CircleShape punto(2.0f);
                punto.setFillColor(sf::Color(200, 160, 255));
                punto.setPosition(pos.x - 2.0f, pos.y - 2.0f);
                ventana.draw(punto);
            }
        }
    }
}