#include <chrono>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <sys/stat.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Cache/Cache.hpp"
#include "DataGenerators/GeneradorDatos.hpp"
#include "DataGenerators/GeneradorModelos3D.hpp"
#include "Graficos/ModelViewer.hpp"

// Declaraciones adelantadas de las funciones auxiliares
void limpiarTerminal();
void esperarEnter();
int mostrarMenuPrincipal();
int mostrarMenuModelado();
void mostrarEstadisticasCache(const Cache& cache, double simTime);
bool cargarFuente(sf::Font& font, const std::string& path);

// Implementación de las funciones auxiliares
void limpiarTerminal() {
    #ifdef _WIN32
    if (system("cls") == -1) {
        std::cerr << "Error al limpiar terminal\n";
    }
    #else
    if (system("clear") == -1) {
        std::cerr << "Error al limpiar terminal\n";
    }
    #endif
}

void esperarEnter() {
    std::cout << "\nPresione enter para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    limpiarTerminal();
}

int mostrarMenuPrincipal() {
    int opcion;
    while (true) {
        std::cout << "\n=== MENU PRINCIPAL ===\n";
        std::cout << "1. Mostrar estadisticas de cache\n";
        std::cout << "2. Modelar figuras 3d\n";
        std::cout << "3. Salir\n";
        std::cout << "Seleccione una opcion (1-3): ";
        
        if (std::cin >> opcion && opcion >= 1 && opcion <= 3) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return opcion;
        }
        
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Opcion no valida. Intente nuevamente.\n";
    }
}

int mostrarMenuModelado() {
    int opcion;
    while (true) {
        std::cout << "\n=== MENU DE MODELADO 3D ===\n";
        std::cout << "1. Visualizar cubo\n";
        std::cout << "2. Visualizar piramide\n";
        std::cout << "3. Volver al menu principal\n";
        std::cout << "Seleccione una opcion (1-3): ";
        
        if (std::cin >> opcion && opcion >= 1 && opcion <= 3) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return opcion;
        }
        
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Opcion no valida. Intente nuevamente.\n";
    }
}

void mostrarEstadisticasCache(const Cache& cache, double simTime) {
    std::cout << "\n=== Estadisticas de cache ===\n";
    std::cout << "Tiempo simulacion: " << simTime << " ms\n";
    cache.imprimirEstadisticas();
    esperarEnter();
}

bool cargarFuente(sf::Font& font, const std::string& path) {
    struct stat buffer;   
    return (stat(path.c_str(), &buffer) == 0) && font.loadFromFile(path);
}

// Función principal
int main() {
    try {
        Cache cache(1024, 64, 4);
        std::vector<int> addresses = GeneradorDatos::generarSecuenciaOptimizada(1024, 64, 4);

        auto startSim = std::chrono::high_resolution_clock::now();
        for (int addr : addresses) {
            cache.accederConPrefetch(addr);
        }
        double simTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - startSim).count();

        sf::Font font;
        if (!cargarFuente(font, "arial.ttf")) {
            std::cerr << "Advertencia: usando fuente por defecto\n";
        }

        bool graphicMode = (getenv("DISPLAY") != nullptr);
        int option;
        
        do {
            option = mostrarMenuPrincipal();
            
            switch (option) {
                case 1:
                    mostrarEstadisticasCache(cache, simTime);
                    break;
                case 2: {
                    int modelOption = mostrarMenuModelado();
                    if (modelOption == 1 || modelOption == 2) {
                        sf::RenderWindow ventana(sf::VideoMode(1024, 768), "Visualizador 3D");
                        if (modelOption == 1) {
                            ModelViewer::visualizar(
                                GeneradorModelos3D::generarCubo(2.0f), 
                                cache, 
                                simTime, 
                                graphicMode, 
                                font,
                                ventana
                            );
                        } else {
                            ModelViewer::visualizar(
                                GeneradorModelos3D::generarPiramide(3.0f, 2.0f), 
                                cache, 
                                simTime, 
                                graphicMode, 
                                font,
                                ventana
                            );
                        }
                    }
                    break;
                }
            }
        } while (option != 3);

    } catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}