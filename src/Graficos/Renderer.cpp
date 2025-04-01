// incluye archivo de cabecera del renderizador
#include "Renderer.hpp"

// incluye funciones gráficas básicas
#include "Graficos.hpp"

// incluye controlador de cámara
#include "CameraController.hpp"

// incluye algoritmos como sort
#include <algorithm>

// incluye funciones matemáticas
#include <cmath>

// color de fondo estilo blender (gris azulado)
const sf::Color FONDO(45, 45, 60);

// color para caras opacas (azul claro semitransparente)
const sf::Color COLOR_CARA_OPACA(80, 160, 200, 180);

// color para aristas (amarillo-anaranjado)
const sf::Color COLOR_ARISTAS(255, 200, 50, 220);

// color para vértices (blanco brillante)
const sf::Color COLOR_VERTICES(240, 240, 240, 255);

// distancia mínima de visualización
constexpr float Z_NEAR = 0.05f;

// distancia máxima de visualización
constexpr float Z_FAR = 100.0f;

// campo de visión en radianes (60 grados)
constexpr float FOV = 60.0f * 3.14159f / 180.0f;

// relación de aspecto (1024x768)
constexpr float ASPECT_RATIO = 1024.0f / 768.0f;

// factor de escala de la vista
constexpr float VIEWPORT_SCALE = 400.0f;

// genera las caras de los modelos predefinidos
void Renderer::generarCaras(const std::vector<Vertice>& /*vertices*/,
                          std::vector<std::vector<int>>& caras,
                          size_t tipoModelo) {
    // limpia el vector de caras
    caras.clear();
    
    // configura caras para cubo (8 vértices)
    if (tipoModelo == 8) {
        caras = {
            {0,1,2,3}, {4,5,6,7}, {0,1,5,4},
            {2,3,7,6}, {0,3,7,4}, {1,2,6,5}
        };
    } 
    // configura caras para pirámide (5 vértices)
    else if (tipoModelo == 5) {
        caras = {
            {0,3,2,1},  // base cuadrada
            {0,1,4},    // caras laterales triangulares
            {1,2,4},
            {2,3,4},
            {3,0,4}
        };
    }
}

// configura las conexiones entre vértices para los modelos
void Renderer::configurarConexiones(const std::vector<Vertice>& vertices, 
                                  std::vector<std::pair<int, int>>& conexiones) {
    // limpia el vector de conexiones
    conexiones.clear();

    // conexiones para cubo (8 vértices)
    if (vertices.size() == 8) {
        conexiones = {
            {0,1}, {1,2}, {2,3}, {3,0},  // cara frontal
            {4,5}, {5,6}, {6,7}, {7,4},  // cara trasera
            {0,4}, {1,5}, {2,6}, {3,7},  // conexiones laterales
            {0,5}, {1,4}, {2,7}, {3,6}   // diagonales
        };
    } 
    // conexiones para pirámide (5 vértices)
    else if (vertices.size() == 5) {
        conexiones = {
            {0,1}, {1,2}, {2,3}, {3,0},  // base
            {0,4}, {1,4}, {2,4}, {3,4}   // aristas laterales
        };
    }
}

// función principal de renderizado del modelo 3d
void Renderer::renderizarModelo(sf::RenderWindow& ventana, 
                              const std::vector<Vertice>& vertices,
                              const std::vector<std::pair<int, int>>& conexiones,
                              const Camara& camara,
                              ModoRenderizado modo) {
    // verifica si hay vértices o la ventana está cerrada
    if (vertices.empty() || !ventana.isOpen()) return;

    // limpia la ventana con color de fondo
    ventana.clear(FONDO);

    // 1. transformación y proyección de vértices
    std::vector<sf::Vector2f> verticesProyectados;
    verticesProyectados.reserve(vertices.size());
    
    // procesa cada vértice
    for (const auto& v : vertices) {
        Vertice vt = v;
        // transforma a espacio de cámara
        CameraController::transformarVertice(vt, camara);

        // proyección perspectiva
        if (vt.z < -Z_NEAR && vt.z > -Z_FAR) {
            // cálculo de factor de perspectiva
            float factor = 1.0f / (-vt.z * tan(FOV/2));
            // proyección en x e y
            float x = vt.x * factor * ASPECT_RATIO * VIEWPORT_SCALE + 512.0f;
            float y = -vt.y * factor * VIEWPORT_SCALE + 384.0f;
            
            // guarda vértice proyectado
            verticesProyectados.emplace_back(x, y);
        } else {
            // posición fuera de pantalla si no es visible
            verticesProyectados.emplace_back(-10000, -10000);
        }
    }

    // 2. renderizado de caras con ordenación por profundidad
    if (modo == MODO_SOLIDO || modo == MODO_MIXTO) {
        std::vector<std::vector<int>> caras;
        // genera caras según tipo de modelo
        generarCaras(vertices, caras, vertices.size());

        // vector para ordenar caras por profundidad
        std::vector<std::pair<float, size_t>> carasOrdenadas;
        for (size_t i = 0; i < caras.size(); ++i) {
            float zSum = 0;
            bool valida = true;
            // calcula profundidad promedio de la cara
            for (int idx : caras[i]) {
                if (idx < 0 || static_cast<size_t>(idx) >= vertices.size()) {
                    valida = false;
                    break;
                }
                zSum += vertices[idx].z;
            }
            // si la cara es válida, guarda su profundidad promedio
            if (valida && !caras[i].empty()) {
                carasOrdenadas.emplace_back(zSum / caras[i].size(), i);
            }
        }

        // ordena caras de lejano a cercano
        std::sort(carasOrdenadas.begin(), carasOrdenadas.end(), 
            [](const auto& a, const auto& b) { return a.first < b.first; });

        // renderiza cada cara ordenada
        for (const auto& [z, i] : carasOrdenadas) {
            const auto& cara = caras[i];
            // crea polígono según tipo de cara (quad o triángulo)
            sf::VertexArray poligono(cara.size() == 4 ? sf::Quads : sf::Triangles);
            bool visible = true;

            // añade vértices al polígono
            for (int idx : cara) {
                // verifica índices válidos
                if (idx < 0 || static_cast<size_t>(idx) >= verticesProyectados.size()) {
                    visible = false;
                    break;
                }
                
                const auto& pos = verticesProyectados[idx];
                // verifica si está dentro de límites razonables
                if (pos.x < -500 || pos.x > 1500 || pos.y < -500 || pos.y > 1500) {
                    visible = false;
                    break;
                }
                
                // añade vértice al polígono
                poligono.append(sf::Vertex(pos, COLOR_CARA_OPACA));
            }

            // dibuja el polígono si es visible
            if (visible && poligono.getVertexCount() >= 3) {
                ventana.draw(poligono);
            }
        }
    }

    // 3. renderizado de aristas
    if (modo == MODO_LINEAS || modo == MODO_MIXTO) {
        sf::VertexArray lineas(sf::Lines);
        
        // procesa cada conexión entre vértices
        for (const auto& conn : conexiones) {
            // verifica índices válidos
            if (conn.first < 0 || static_cast<size_t>(conn.first) >= verticesProyectados.size() ||
                conn.second < 0 || static_cast<size_t>(conn.second) >= verticesProyectados.size()) {
                continue;
            }

            // obtiene posiciones proyectadas
            const auto& p1 = verticesProyectados[conn.first];
            const auto& p2 = verticesProyectados[conn.second];
            
            // calcula distancia al cuadrado entre puntos
            float dx = p1.x - p2.x;
            float dy = p1.y - p2.y;
            float distancia2 = dx*dx + dy*dy;
            
            // filtra líneas muy cortas/largas o fuera de límites
            if (distancia2 > 0 && distancia2 < 500000 &&
                p1.x > -300 && p1.x < 1300 && p2.x > -300 && p2.x < 1300) {
                // añade línea al array
                lineas.append(sf::Vertex(p1, COLOR_ARISTAS));
                lineas.append(sf::Vertex(p2, COLOR_ARISTAS));
            }
        }
        
        // dibuja todas las líneas
        ventana.draw(lineas);
    }

    // 4. renderizado de vértices como puntos
    if (modo != MODO_SOLIDO) {
        sf::VertexArray puntos(sf::Points);
        
        // procesa cada vértice proyectado
        for (const auto& pos : verticesProyectados) {
            // solo dibuja si está dentro de la ventana
            if (pos.x > 0 && pos.x < 1024 && pos.y > 0 && pos.y < 768) {
                puntos.append(sf::Vertex(pos, COLOR_VERTICES));
            }
        }
        
        // dibuja todos los puntos
        ventana.draw(puntos);
    }
}

// dibuja un puntero FPS en el centro de la pantalla
void Renderer::dibujarPuntero(sf::RenderWindow& ventana, const sf::Color& color) {
    // calcula centro de la ventana
    float centroX = ventana.getSize().x / 2.0f;
    float centroY = ventana.getSize().y / 2.0f;
    // parámetros visuales del puntero
    float tamaño = 15.0f;
    float grosor = 2.0f;
    float espacio = 5.0f;

    // crea y dibuja punto central
    sf::CircleShape punto(2.0f);
    punto.setFillColor(color);
    punto.setPosition(centroX - 2.0f, centroY - 2.0f);
    ventana.draw(punto);

    // línea horizontal izquierda
    sf::RectangleShape lineaH(sf::Vector2f(tamaño, grosor));
    lineaH.setFillColor(color);
    lineaH.setPosition(centroX - tamaño - espacio, centroY - grosor/2.0f);
    ventana.draw(lineaH);

    // línea horizontal derecha
    lineaH.setPosition(centroX + espacio, centroY - grosor/2.0f);
    ventana.draw(lineaH);

    // línea vertical superior
    sf::RectangleShape lineaV(sf::Vector2f(grosor, tamaño));
    lineaV.setFillColor(color);
    lineaV.setPosition(centroX - grosor/2.0f, centroY - tamaño - espacio);
    ventana.draw(lineaV);

    // línea vertical inferior
    lineaV.setPosition(centroX - grosor/2.0f, centroY + espacio);
    ventana.draw(lineaV);
}