#include "Cache/Cache.hpp"
#include "Graficos/Graficos.hpp"
#include "DataGenerators/GeneradorDatos.hpp"
#include "DataGenerators/GeneradorModelos3D.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <chrono> // Para medición de tiempo

int main() {
    try {
        // =============================================
        // 1. Configuración del Caché
        // =============================================
        const uint32_t tamanoCache = 1024;   // 1 KB
        const uint32_t tamanoBloque = 64;    // 64 bytes
        const uint32_t asociatividad = 4;    // 4-way
        Cache cache(tamanoCache, tamanoBloque, asociatividad);

        // =============================================
        // 2. Generación de Direcciones Optimizada
        // =============================================
        auto inicioGeneracion = std::chrono::high_resolution_clock::now();
        std::vector<int> direcciones = GeneradorDatos::generarSecuenciaOptimizada(
            tamanoCache, tamanoBloque, asociatividad);
        auto finGeneracion = std::chrono::high_resolution_clock::now();

        std::cout << "=== Secuencia generada en "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(finGeneracion - inicioGeneracion).count()
                  << " ms ===\n";

        // =============================================
        // 3. Simulación de Accesos con Prefetching
        // =============================================
        std::cout << "\n=== Simulación de Caché ===\n";
        auto inicioSimulacion = std::chrono::high_resolution_clock::now();
        
        for (size_t i = 0; i < direcciones.size(); ++i) {
            int dir = direcciones[i];
            bool acierto = cache.accederConPrefetch(dir); // Usamos la versión con prefetch
            
            // Mostrar solo cada 10 accesos para no saturar la salida
            if (i % 10 == 0 || i == direcciones.size() - 1) {
                std::cout << "Acceso " << i + 1 << ": Dirección " << dir 
                          << " | Conjunto " << ((dir / tamanoBloque) % cache.calcularNumConjuntos())
                          << " | " << (acierto ? "ACIERTO" : "FALLO") << "\n";
            }
        }
        
        auto finSimulacion = std::chrono::high_resolution_clock::now();
        std::cout << "\nTiempo de simulacion: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(finSimulacion - inicioSimulacion).count()
                  << " ms\n";

        // =============================================
        // 4. Resultados y Estadísticas
        // =============================================
        cache.imprimirEstadisticas();
        cache.imprimirEstado();

        // =============================================
        // 5. Visualización 3D (Opcional)
        // =============================================
        std::vector<Vertice> cubo = GeneradorModelos3D::generarCubo(1.5f);
        
        // Configuración de SFML
        sf::Vertex ejeX[] = {
            sf::Vertex(sf::Vector2f(400.f, 300.f), sf::Color::Red),
            sf::Vertex(sf::Vector2f(500.f, 300.f), sf::Color::Red)
        };
        
        sf::Vertex ejeY[] = {
            sf::Vertex(sf::Vector2f(400.f, 300.f), sf::Color::Green),
            sf::Vertex(sf::Vector2f(400.f, 200.f), sf::Color::Green)
        };

        std::vector<std::pair<int, int>> conexiones = {
            {0,1}, {1,2}, {2,3}, {3,0}, {4,5}, {5,6}, {6,7}, {7,4},
            {0,4}, {1,5}, {2,6}, {3,7}
        };

        // Verificar soporte gráfico
        bool modoGrafico = (getenv("DISPLAY") != nullptr);
        
        if (modoGrafico) {
            sf::RenderWindow ventana(sf::VideoMode(800, 600), "Simulador 3D con Caché (Optimizado)");
            if (ventana.isOpen()) {
                ventana.setFramerateLimit(60);
                
                while (ventana.isOpen()) {
                    sf::Event evento;
                    while (ventana.pollEvent(evento)) {
                        if (evento.type == sf::Event::Closed) {
                            ventana.close();
                        }
                    }

                    ventana.clear(sf::Color::Black);
                    
                    // Dibujar ejes
                    ventana.draw(ejeX, 2, sf::Lines);
                    ventana.draw(ejeY, 2, sf::Lines);

                    // Dibujar conexiones
                    for (const auto& conexion : conexiones) {
                        sf::Vertex linea[] = {
                            sf::Vertex(sf::Vector2f(400.f + cubo[conexion.first].x * 50.f, 
                                        300.f - cubo[conexion.first].y * 50.f), sf::Color::Cyan),
                            sf::Vertex(sf::Vector2f(400.f + cubo[conexion.second].x * 50.f,
                                        300.f - cubo[conexion.second].y * 50.f), sf::Color::Cyan)
                        };
                        ventana.draw(linea, 2, sf::Lines);
                    }

                    // Dibujar vértices con color basado en posición Z (profundidad)
                    for (const auto& vertice : cubo) {
                        sf::CircleShape punto(5.f);
                        int colorIntensity = 150 + (vertice.z * 50); // Mapear Z a color
                        punto.setFillColor(sf::Color(colorIntensity, colorIntensity, 255));
                        punto.setPosition(400.f + vertice.x * 50.f, 300.f - vertice.y * 50.f);
                        ventana.draw(punto);
                    }

                    ventana.display();
                }
            }
        } else {
            std::cout << "\n=== Modo Texto ===\n";
            std::cout << "Cubo 3D generado con " << cubo.size() << " vertices.\n";
            GeneradorModelos3D::imprimirVertices(cubo);
            
            // Mostrar diagrama ASCII
            std::cout << "\nDiagrama del Cubo:\n";
            std::cout << "    V4-------V5\n";
            std::cout << "   /|       /|\n";
            std::cout << "  V0-------V1|\n";
            std::cout << "  | |      | |\n";
            std::cout << "  |V7------|V6\n";
            std::cout << "  |/       |/\n";
            std::cout << "  V3-------V2\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}