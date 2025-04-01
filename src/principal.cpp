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

// funcion para limpiar la terminal de forma multiplataforma
void limpiarTerminal() {
    #ifdef _WIN32
    int result = system("cls");
    (void)result; // para evitar el warning de unused result
    #else
    int result = system("clear");
    (void)result; // para evitar el warning de unused result
    #endif
}

// funcion para pausar la ejecucion hasta que se presione enter
void esperarEnter() {
    std::cout << "\nPresione enter para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    limpiarTerminal();
}

// muestra el menu principal y obtiene la seleccion del usuario
int mostrarMenuPrincipal() {
    int opcion;
    while (true) {
        std::cout << "\n=== MENU PRINCIPAL ===\n";
        std::cout << "1. Mostrar estadisticas de cache\n";
        std::cout << "2. Modelar figuras 3d\n";
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
        
        std::cout << "Opcion no valida. intente nuevamente.\n";
    }
}

// muestra el menu de modelado 3d y obtiene la seleccion del usuario
int mostrarMenuModelado() {
    int opcion;
    while (true) {
        std::cout << "\n=== menu de modelado 3d ===\n";
        std::cout << "1. Visualizar cubo\n";
        std::cout << "2. Visualizar piramide\n";
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
        
        std::cout << "Opcion no valida. intente nuevamente.\n";
    }
}

// muestra las estadisticas de la cache en la consola
void mostrarEstadisticasCache(const Cache& cache, double simTime) {
    std::cout << "\n=== Estadisticas de cache ===\n";
    std::cout << "Tiempo simulacion: " << simTime << " ms\n";
    cache.imprimirEstadisticas();
    esperarEnter();
}

// carga una fuente desde un archivo
bool cargarFuente(sf::Font& font, const std::string& path) {
    struct stat buffer;   
    if (stat(path.c_str(), &buffer) != 0) {
        std::cerr << "Archivo de fuente no encontrado: " << path << "\n";
        return false;
    }
    return font.loadFromFile(path);
}

// funcion principal del programa
int main() {
    try {
        // inicializa la cache con parametros predefinidos
        Cache cache(1024, 64, 4);

        // genera direcciones para pruebas de cache
        std::vector<int> addresses = GeneradorDatos::generarSecuenciaOptimizada(1024, 64, 4);

        // simula accesos a la cache y mide el tiempo
        auto startSim = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < addresses.size(); ++i) {
            cache.accederConPrefetch(addresses[i]);
        }
        auto endSim = std::chrono::high_resolution_clock::now();

        // calcula el tiempo de simulacion
        double simTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            endSim - startSim).count();

        // carga la fuente para la interfaz grafica
        sf::Font font;
        bool fontLoaded = cargarFuente(font, "arial.ttf");
        if (!fontLoaded) {
            std::cerr << "Advertencia: usando fuente por defecto\n";
        }

        // determina si el modo grafico esta disponible
        bool graphicMode = (getenv("DISPLAY") != nullptr);
        int option;
        
        // bucle principal del menu
        do {
            option = mostrarMenuPrincipal();
            
            switch (option) {
                case 1: {
                    // muestra estadisticas de cache
                    mostrarEstadisticasCache(cache, simTime);
                    break;
                }
                case 2: {
                    // menu de modelado 3d
                    int modelOption = mostrarMenuModelado();
                    switch (modelOption) {
                        case 1: {
                            // genera y visualiza un cubo
                            std::vector<Vertice> cube = GeneradorModelos3D::generarCubo(2.0f);
                            ModelViewer::visualizar(cube, cache, simTime, graphicMode, font);
                            break;
                        }
                        case 2: {
                            // genera y visualiza una piramide
                            std::vector<Vertice> pyramid = GeneradorModelos3D::generarPiramide(3.0f, 2.0f);
                            ModelViewer::visualizar(pyramid, cache, simTime, graphicMode, font);
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