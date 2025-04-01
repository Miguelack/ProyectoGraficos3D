#include <chrono>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <sys/stat.h>
#include <SFML/Graphics.hpp>
#include "Cache/Cache.hpp"
#include "DataGenerators/GeneradorDatos.hpp"
#include "DataGenerators/GeneradorModelos3D.hpp"
#include "Graficos/ModelViewer.hpp"

void limpiarTerminal() {
    // Clear terminal screen in a cross-platform way
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void esperarEnter() {
    std::cout << "\nPresione Enter para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    limpiarTerminal();
}

int mostrarMenuPrincipal() {
    int opcion;
    while (true) {
        std::cout << "\n=== MENU PRINCIPAL ===\n";
        std::cout << "1. Mostrar estadisticas de Cache\n";
        std::cout << "2. Modelar figuras 3D\n";
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

int mostrarMenuModelado() {
    int opcion;
    while (true) {
        std::cout << "\n=== MENU DE MODELADO 3D ===\n";
        std::cout << "1. Visualizar Cubo\n";
        std::cout << "2. Visualizar Piramide\n";
        std::cout << "3. Volver al menu principal\n";
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

void mostrarEstadisticasCache(const Cache& cache, double simTime) {
    std::cout << "\n=== Estadisticas de Cache ===\n";
    std::cout << "Tiempo simulacion: " << simTime << " ms\n";
    cache.imprimirEstadisticas();
    esperarEnter();
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
        Cache cache(1024, 64, 4);

        // Eliminadas las variables no utilizadas para generación
        std::vector<int> addresses = GeneradorDatos::generarSecuenciaOptimizada(1024, 64, 4);

        auto startSim = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < addresses.size(); ++i) {
            cache.accederConPrefetch(addresses[i]);
        }
        auto endSim = std::chrono::high_resolution_clock::now();

        double simTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            endSim - startSim).count();

        sf::Font font;
        bool fontLoaded = cargarFuente(font, "arial.ttf");
        if (!fontLoaded) {
            std::cerr << "Advertencia: Usando fuente por defecto\n";
        }

        bool graphicMode = (getenv("DISPLAY") != nullptr);
        int option;
        
        do {
            option = mostrarMenuPrincipal();
            
            switch (option) {
                case 1: {
                    mostrarEstadisticasCache(cache, simTime);
                    break;
                }
                case 2: {
                    int modelOption = mostrarMenuModelado();
                    switch (modelOption) {
                        case 1: {
                            std::vector<Vertice> cube = GeneradorModelos3D::generarCubo(2.0f);
                            ModelViewer::visualize(cube, cache, simTime, graphicMode, font);
                            break;
                        }
                        case 2: {
                            std::vector<Vertice> pyramid = GeneradorModelos3D::generarPiramide(3.0f, 2.0f);
                            ModelViewer::visualize(pyramid, cache, simTime, graphicMode, font);
                            break;
                        }
                        case 3:
                            limpiarTerminal();
                            break;
                    }
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
    } catch (...) {
        std::cerr << "\nError desconocido\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}