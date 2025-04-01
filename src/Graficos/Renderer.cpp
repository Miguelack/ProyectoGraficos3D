#include "Renderer.hpp"
#include "Graficos.hpp"
#include "CameraController.hpp"
#include <algorithm>
#include <cmath>

// Configuración visual estilo Blender
const sf::Color FONDO(45, 45, 60); // Gris azulado oscuro
const sf::Color COLOR_CARA_OPACA(80, 160, 200, 180); // Azul claro semitransparente
const sf::Color COLOR_ARISTAS(255, 200, 50, 220); // Amarillo-anaranjado
const sf::Color COLOR_VERTICES(240, 240, 240, 255); // Blanco brillante

// Parámetros de renderizado
constexpr float Z_NEAR = 0.05f;
constexpr float Z_FAR = 100.0f;
constexpr float FOV = 60.0f * 3.14159f / 180.0f; // 60° en radianes
constexpr float ASPECT_RATIO = 1024.0f / 768.0f;
constexpr float VIEWPORT_SCALE = 400.0f;

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
    else if (tipoModelo == 5) { // Pirámide
        caras = {
            {0,3,2,1},  // Base cuadrada (orden antihorario)
            {0,1,4},    // Caras laterales
            {1,2,4},
            {2,3,4},
            {3,0,4}
        };
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

void Renderer::renderizarModelo(sf::RenderWindow& ventana, 
                              const std::vector<Vertice>& vertices,
                              const std::vector<std::pair<int, int>>& conexiones,
                              const Camara& camara,
                              ModoRenderizado modo) {
    if (vertices.empty() || !ventana.isOpen()) return;

    ventana.clear(FONDO);

    // 1. Pre-transformación y proyección de vértices
    std::vector<sf::Vector2f> verticesProyectados;
    verticesProyectados.reserve(vertices.size());
    
    for (const auto& v : vertices) {
        Vertice vt = v;
        CameraController::transformarVertice(vt, camara);

        // Proyección perspectiva profesional
        if (vt.z < -Z_NEAR && vt.z > -Z_FAR) {
            float factor = 1.0f / (-vt.z * tan(FOV/2));
            float x = vt.x * factor * ASPECT_RATIO * VIEWPORT_SCALE + 512.0f;
            float y = -vt.y * factor * VIEWPORT_SCALE + 384.0f;
            
            verticesProyectados.emplace_back(x, y);
        } else {
            verticesProyectados.emplace_back(-10000, -10000);
        }
    }

    // 2. Renderizado de caras con ordenación por profundidad
    if (modo == MODO_SOLIDO || modo == MODO_MIXTO) {
        std::vector<std::vector<int>> caras;
        generarCaras(vertices, caras, vertices.size());

        // Ordenar caras por profundidad
        std::vector<std::pair<float, size_t>> carasOrdenadas;
        for (size_t i = 0; i < caras.size(); ++i) {
            float zSum = 0;
            bool valida = true;
            for (int idx : caras[i]) {
                if (idx < 0 || static_cast<size_t>(idx) >= vertices.size()) {
                    valida = false;
                    break;
                }
                zSum += vertices[idx].z;
            }
            if (valida && !caras[i].empty()) {
                carasOrdenadas.emplace_back(zSum / caras[i].size(), i);
            }
        }

        std::sort(carasOrdenadas.begin(), carasOrdenadas.end(), 
            [](const auto& a, const auto& b) { return a.first < b.first; });

        // Renderizar caras ordenadas
        for (const auto& [z, i] : carasOrdenadas) {
            const auto& cara = caras[i];
            sf::VertexArray poligono(cara.size() == 4 ? sf::Quads : sf::Triangles);
            bool visible = true;

            for (int idx : cara) {
                if (idx < 0 || static_cast<size_t>(idx) >= verticesProyectados.size()) {
                    visible = false;
                    break;
                }
                
                const auto& pos = verticesProyectados[idx];
                if (pos.x < -500 || pos.x > 1500 || pos.y < -500 || pos.y > 1500) {
                    visible = false;
                    break;
                }
                
                poligono.append(sf::Vertex(pos, COLOR_CARA_OPACA));
            }

            if (visible && poligono.getVertexCount() >= 3) {
                ventana.draw(poligono);
            }
        }
    }

    // 3. Renderizado de aristas
    if (modo == MODO_LINEAS || modo == MODO_MIXTO) {
        sf::VertexArray lineas(sf::Lines);
        
        for (const auto& conn : conexiones) {
            if (conn.first < 0 || static_cast<size_t>(conn.first) >= verticesProyectados.size() ||
                conn.second < 0 || static_cast<size_t>(conn.second) >= verticesProyectados.size()) {
                continue;
            }

            const auto& p1 = verticesProyectados[conn.first];
            const auto& p2 = verticesProyectados[conn.second];
            
            float dx = p1.x - p2.x;
            float dy = p1.y - p2.y;
            float distancia2 = dx*dx + dy*dy;
            
            if (distancia2 > 0 && distancia2 < 500000 &&
                p1.x > -300 && p1.x < 1300 && p2.x > -300 && p2.x < 1300) {
                lineas.append(sf::Vertex(p1, COLOR_ARISTAS));
                lineas.append(sf::Vertex(p2, COLOR_ARISTAS));
            }
        }
        
        ventana.draw(lineas);
    }

    // 4. Renderizado de vértices
    if (modo != MODO_SOLIDO) {
        sf::VertexArray puntos(sf::Points);
        
        for (const auto& pos : verticesProyectados) {
            if (pos.x > 0 && pos.x < 1024 && pos.y > 0 && pos.y < 768) {
                puntos.append(sf::Vertex(pos, COLOR_VERTICES));
            }
        }
        
        ventana.draw(puntos);
    }
}

void Renderer::dibujarPuntero(sf::RenderWindow& ventana, const sf::Color& color) {
    // Crear una simple cruz (punto central + 4 líneas)
    float centroX = ventana.getSize().x / 2.0f;
    float centroY = ventana.getSize().y / 2.0f;
    float tamaño = 15.0f;
    float grosor = 2.0f;
    float espacio = 5.0f;

    // Punto central
    sf::CircleShape punto(2.0f);
    punto.setFillColor(color);
    punto.setPosition(centroX - 2.0f, centroY - 2.0f);
    ventana.draw(punto);

    // Línea horizontal izquierda
    sf::RectangleShape lineaH(sf::Vector2f(tamaño, grosor));
    lineaH.setFillColor(color);
    lineaH.setPosition(centroX - tamaño - espacio, centroY - grosor/2.0f);
    ventana.draw(lineaH);

    // Línea horizontal derecha
    lineaH.setPosition(centroX + espacio, centroY - grosor/2.0f);
    ventana.draw(lineaH);

    // Línea vertical superior
    sf::RectangleShape lineaV(sf::Vector2f(grosor, tamaño));
    lineaV.setFillColor(color);
    lineaV.setPosition(centroX - grosor/2.0f, centroY - tamaño - espacio);
    ventana.draw(lineaV);

    // Línea vertical inferior
    lineaV.setPosition(centroX - grosor/2.0f, centroY + espacio);
    ventana.draw(lineaV);
}