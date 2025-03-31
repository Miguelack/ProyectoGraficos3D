#include <algorithm>  // Para std::clamp
#include <cmath>      // Para funciones matemáticas
#include <limits>     // Para std::numeric_limits
#include <vector>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <sys/stat.h>

// Incluye SFML y tus headers después
#include <SFML/Graphics.hpp>
#include "Cache/Cache.hpp"
#include "Graficos/Graficos.hpp"
#include "DataGenerators/GeneradorDatos.hpp"
#include "DataGenerators/GeneradorModelos3D.hpp"


template <typename T>
const T& clamp(const T& value, const T& min, const T& max) {
    return (value < min) ? min : (max < value) ? max : value;
}

struct Camara {
    float x, y, z;
    float rotX, rotY;
    float velocidadMovimiento;
    float velocidadRotacion;
    float targetX, targetY, targetZ;
    float lerpFactor;
    
    Camara() : x(0), y(0), z(5.0f), rotX(0), rotY(0), 
              velocidadMovimiento(0.2f), velocidadRotacion(0.05f),
              targetX(0), targetY(0), targetZ(5.0f), lerpFactor(0.2f) {}
    
    void actualizar() {
        x += (targetX - x) * lerpFactor;
        y += (targetY - y) * lerpFactor;
        z += (targetZ - z) * lerpFactor;
    }
    
    void reset() {
        targetX = 0; targetY = 0; targetZ = 5.0f;
        rotX = 0; rotY = 0;
    }
};

struct InputState {
    bool w, a, s, d, up, down, left, right, space, ctrl;
    
    InputState() : w(false), a(false), s(false), d(false),
                  up(false), down(false), left(false), right(false),
                  space(false), ctrl(false) {}
};

struct UI {
    sf::Text statsText;
    sf::Text controlsText;
    sf::Text positionText;
    
    void init(const sf::Font& font, const Cache& cache, double tiempoSimulacion, size_t vertexCount) {
        std::ostringstream stats;
        stats << "Estadisticas de Cache:\n"
              << "Aciertos: " << cache.getAciertos() << " (" 
              << static_cast<int>(cache.getTasaAciertos()) << "%)\n"
              << "Fallos: " << cache.getFallos() << "\n"
              << "Tiempo: " << tiempoSimulacion << " ms\n"
              << "Modelo: " << (vertexCount == 8 ? "Cubo" : "Piramide");
              
        statsText.setFont(font);
        statsText.setString(stats.str());
        statsText.setCharacterSize(16);
        statsText.setFillColor(sf::Color::White);
        statsText.setPosition(20, 20);
        
        controlsText.setFont(font);
        controlsText.setString(
            "Controles:\n"
            "WASD: Movimiento\n"
            "Flechas: Rotar\n"
            "ESPACIO/CTRL: Subir/Bajar\n"
            "R: Resetear vista\n"
            "ESC: Salir");
        controlsText.setCharacterSize(16);
        controlsText.setFillColor(sf::Color(200, 200, 200));
        controlsText.setPosition(20, 680);
        
        positionText.setFont(font);
        positionText.setCharacterSize(14);
        positionText.setFillColor(sf::Color::Yellow);
        positionText.setPosition(20, 620);
    }
    
    void updatePositionText(const Camara& cam) {
        std::ostringstream pos;
        pos << "Posicion: X=" << std::fixed << std::setprecision(1) << cam.x 
            << " Y=" << cam.y << " Z=" << cam.z
            << "\nRotacion: X=" << cam.rotX << " Y=" << cam.rotY;
        positionText.setString(pos.str());
    }
};

bool clipLine(Vertice& v1, Vertice& v2, float nearPlane = 0.1f) {
    if (v1.z <= nearPlane && v2.z <= nearPlane) {
        return false;
    }
    
    if (v1.z <= nearPlane) {
        float t = (nearPlane - v2.z) / (v1.z - v2.z);
        v1.x = v2.x + t * (v1.x - v2.x);
        v1.y = v2.y + t * (v1.y - v2.y);
        v1.z = nearPlane;
    }
    else if (v2.z <= nearPlane) {
        float t = (nearPlane - v1.z) / (v2.z - v1.z);
        v2.x = v1.x + t * (v2.x - v1.x);
        v2.y = v1.y + t * (v2.y - v1.y);
        v2.z = nearPlane;
    }
    
    return true;
}

sf::Vector2f proyectarPunto(const Vertice& v, float scale = 50.0f) {
    if (v.z > 0.1f) {
        float factor = 500.0f / (v.z + 500.0f);
        return sf::Vector2f(512.f + v.x * factor * scale, 384.f + v.y * factor * scale);
    }
    return sf::Vector2f(-1000, -1000);
}

void transformarVertice(Vertice& v, const Camara& cam) {
    // Normalizar ángulos primero para evitar overflow
    float rotY = fmod(cam.rotY, 2*M_PI);
    float rotX = fmod(cam.rotX, 2*M_PI);
    
    // Traslación
    float dx = v.x - cam.x;
    float dy = v.y - cam.y;
    float dz = v.z - cam.z;
    
    // Rotación Y (horizontal) - usamos funciones sincronizadas
    float cosY, sinY;
    sincosf(rotY, &sinY, &cosY);
    float x1 = dx * cosY + dz * sinY;
    float z1 = -dx * sinY + dz * cosY;
    
    // Rotación X (vertical) - usamos funciones sincronizadas
    float cosX, sinX;
    sincosf(rotX, &sinX, &cosX);
    v.y = dy * cosX - z1 * sinX;
    v.z = dy * sinX + z1 * cosX;
    v.x = x1;
    
    // Pequeña protección contra valores extremos
    const float MAX_VAL = 1e6f;
    v.x = clamp(v.x, -MAX_VAL, MAX_VAL);
    v.y = clamp(v.y, -MAX_VAL, MAX_VAL);
    v.z = clamp(v.z, -MAX_VAL, MAX_VAL);
}

void configurarConexiones(const std::vector<Vertice>& vertices, 
                         std::vector<std::pair<int, int>>& conexiones) {
    conexiones.clear();
    
    if (vertices.size() == 8) {
        conexiones = {
            {0,1}, {1,2}, {2,3}, {3,0}, {4,5}, {5,6}, {6,7}, {7,4},
            {0,4}, {1,5}, {2,6}, {3,7}, {0,5}, {1,4}, {2,7}, {3,6}
        };
    } else if (vertices.size() == 5) {
        conexiones = {
            {0,1}, {1,2}, {2,3}, {3,0},
            {0,4}, {1,4}, {2,4}, {3,4}
        };
    }
}

void actualizarInput(InputState& input) {
    input.w = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    input.a = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    input.s = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    input.d = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    input.up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    input.down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    input.left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    input.right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    input.space = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    input.ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
}

void visualizarModelo(const std::vector<Vertice>& vertices, const Cache& cache,
                     double tiempoSimulacion, bool modoGrafico, const sf::Font& font) {
    if (!modoGrafico) {
        std::cout << "\n=== Modo Texto ===\n";
        GeneradorModelos3D::imprimirVertices(vertices);
        std::cout << "\n=== Estadísticas ===\n";
        cache.imprimirEstadisticas();
        return;
    }

    try {
        sf::RenderWindow window(sf::VideoMode(1024, 768), "Simulador 3D - WASD para mover, ESC para salir");
        window.setFramerateLimit(60);
        
        sf::View view(sf::FloatRect(0, 0, 1024, 768));
        window.setView(view);
        
        std::vector<std::pair<int, int>> connections;
        configurarConexiones(vertices, connections);
        
        Camara camera;
        InputState input;
        UI ui;
        ui.init(font, cache, tiempoSimulacion, vertices.size());
        
        sf::Clock clock;
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed || 
                   (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                    window.close();
                }
                
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                    camera.reset();
                }
            }
            
            actualizarInput(input);
            
            if (input.w) {
                camera.targetX += sin(camera.rotY) * camera.velocidadMovimiento * deltaTime * 60;
                camera.targetZ += cos(camera.rotY) * camera.velocidadMovimiento * deltaTime * 60;
            }
            if (input.s) {
                camera.targetX -= sin(camera.rotY) * camera.velocidadMovimiento * deltaTime * 60;
                camera.targetZ -= cos(camera.rotY) * camera.velocidadMovimiento * deltaTime * 60;
            }
            if (input.a) {
                camera.targetX -= cos(camera.rotY) * camera.velocidadMovimiento * deltaTime * 60;
                camera.targetZ += sin(camera.rotY) * camera.velocidadMovimiento * deltaTime * 60;
            }
            if (input.d) {
                camera.targetX += cos(camera.rotY) * camera.velocidadMovimiento * deltaTime * 60;
                camera.targetZ -= sin(camera.rotY) * camera.velocidadMovimiento * deltaTime * 60;
            }
            if (input.space) camera.targetY -= camera.velocidadMovimiento * deltaTime * 60;
            if (input.ctrl) camera.targetY += camera.velocidadMovimiento * deltaTime * 60;
            if (input.up) camera.rotX -= camera.velocidadRotacion * deltaTime * 60;
            if (input.down) camera.rotX += camera.velocidadRotacion * deltaTime * 60;
            if (input.left) camera.rotY -= camera.velocidadRotacion * deltaTime * 60;
            if (input.right) camera.rotY += camera.velocidadRotacion * deltaTime * 60;
            
            camera.actualizar();
            ui.updatePositionText(camera);
            
            window.clear(sf::Color::Black);
            
            // Dibujar líneas con recorte
            sf::VertexArray lines(sf::Lines);
            for (const auto& conn : connections) {
                Vertice v1 = vertices[conn.first];
                Vertice v2 = vertices[conn.second];
                
                transformarVertice(v1, camera);
                transformarVertice(v2, camera);
                
                if (!clipLine(v1, v2)) continue;
                
                sf::Vector2f p1 = proyectarPunto(v1);
                sf::Vector2f p2 = proyectarPunto(v2);
                
                if (p1.x > -1000 && p2.x > -1000) {
                    lines.append(sf::Vertex(p1, sf::Color(100, 200, 255)));
                    lines.append(sf::Vertex(p2, sf::Color(100, 200, 255)));
                }
            }
            window.draw(lines);
            
            // Dibujar vértices
            for (const auto& vertex : vertices) {
                Vertice v = vertex;
                transformarVertice(v, camera);
                
                if (v.z <= 0.1f) continue;
                
                sf::Vector2f pos = proyectarPunto(v);
                float distance = sqrt(
                    pow(vertex.x - camera.x, 2) + 
                    pow(vertex.y - camera.y, 2) + 
                    pow(vertex.z - camera.z, 2));
                
                float pointSize = std::max(3.0f, 10.0f - distance * 0.5f);
                sf::CircleShape point(pointSize);
                point.setFillColor(sf::Color(200, 200, 255));
                point.setPosition(pos.x - pointSize, pos.y - pointSize);
                window.draw(point);
            }
            
            // Dibujar ejes
            Vertice origin = {0, 0, 0};
            Vertice axisX = {2, 0, 0}, axisY = {0, 2, 0}, axisZ = {0, 0, 2};
            
            transformarVertice(origin, camera);
            transformarVertice(axisX, camera);
            transformarVertice(axisY, camera);
            transformarVertice(axisZ, camera);
            
            if (origin.z > 0.1f && axisX.z > 0.1f && axisY.z > 0.1f && axisZ.z > 0.1f) {
                sf::Vector2f o = proyectarPunto(origin);
                sf::Vector2f x = proyectarPunto(axisX);
                sf::Vector2f y = proyectarPunto(axisY);
                sf::Vector2f z = proyectarPunto(axisZ);
                
                sf::Vertex axisLines[] = {
                    sf::Vertex(o, sf::Color::Red), sf::Vertex(x, sf::Color::Red),
                    sf::Vertex(o, sf::Color::Green), sf::Vertex(y, sf::Color::Green),
                    sf::Vertex(o, sf::Color::Blue), sf::Vertex(z, sf::Color::Blue)
                };
                window.draw(axisLines, 6, sf::Lines);
            }
            
            window.draw(ui.statsText);
            window.draw(ui.controlsText);
            window.draw(ui.positionText);
            
            window.display();
        }
    } catch (const std::exception& e) {
        std::cerr << "\nError en visualización: " << e.what() << "\n";
    }
}

int mostrarMenu() {
    int opcion;
    while (true) {
        std::cout << "\n=== MENU DE VISUALIZACION 3D ===\n";
        std::cout << "1. Visualizar Cubo\n";
        std::cout << "2. Visualizar Piramide\n";
        std::cout << "3. Salir\n";
        std::cout << "Seleccione una opcion (1-3): ";
        
        if (std::cin >> opcion) {
            if (opcion >= 1 && opcion <= 3) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return opcion;
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
        std::cout << "Opcion no valida. Intente nuevamente.\n";
    }
}

bool cargarFuente(sf::Font& font, const std::string& path) {
    struct stat buffer;   
    if (stat(path.c_str(), &buffer) != 0) {
        std::cerr << "Archivo de fuente no encontrado: " << path << "\n";
        return false;
    }
    return font.loadFromFile(path);
}

int main() {
    try {
        const uint32_t cacheSize = 1024;
        const uint32_t blockSize = 64;
        const uint32_t associativity = 4;
        Cache cache(cacheSize, blockSize, associativity);

        auto startGen = std::chrono::high_resolution_clock::now();
        std::vector<int> addresses = GeneradorDatos::generarSecuenciaOptimizada(
            cacheSize, blockSize, associativity);
        auto endGen = std::chrono::high_resolution_clock::now();

        auto startSim = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < addresses.size(); ++i) {
            cache.accederConPrefetch(addresses[i]);
        }
        auto endSim = std::chrono::high_resolution_clock::now();

        double simTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            endSim - startSim).count();

        std::cout << "=== Estadísticas de Simulación ===\n";
        std::cout << "Tiempo generación: " << std::chrono::duration_cast<std::chrono::milliseconds>(
            endGen - startGen).count() << " ms\n";
        std::cout << "Tiempo simulación: " << simTime << " ms\n";
        cache.imprimirEstadisticas();
        cache.imprimirEstado();

        sf::Font font;
        bool fontLoaded = cargarFuente(font, "arial.ttf");
        if (!fontLoaded) {
            std::cerr << "Advertencia: Usando fuente por defecto\n";
        }

        bool graphicMode = (getenv("DISPLAY") != nullptr);
        int option;
        
        do {
            option = mostrarMenu();
            
            switch (option) {
                case 1: {
                    std::vector<Vertice> cube = GeneradorModelos3D::generarCubo(2.0f);
                    visualizarModelo(cube, cache, simTime, graphicMode, font);
                    break;
                }
                case 2: {
                    std::vector<Vertice> pyramid = GeneradorModelos3D::generarPiramide(3.0f, 2.0f);
                    visualizarModelo(pyramid, cache, simTime, graphicMode, font);
                    break;
                }
                case 3:
                    std::cout << "Saliendo del programa...\n";
                    break;
            }
        } while (option != 3);

    } catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}