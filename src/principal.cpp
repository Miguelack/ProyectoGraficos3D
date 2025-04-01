// includes para manejo de tiempo y fechas
#include <chrono>
// para entrada/salida estándar
#include <iostream>
// para funciones generales como exit()
#include <cstdlib>
// para manejo de excepciones
#include <stdexcept>
// para verificar existencia de archivos
#include <sys/stat.h>
// para gráficos 2D
#include <SFML/Graphics.hpp>
// para manejo de ventanas
#include <SFML/Window.hpp>
// nuestro archivo de caché
#include "Cache/Cache.hpp"
// generador de patrones de acceso
#include "DataGenerators/GeneradorDatos.hpp"
// generador de modelos 3D básicos
#include "DataGenerators/GeneradorModelos3D.hpp"
// visualizador 3D principal
#include "Graficos/ModelViewer.hpp"

// función que limpia la terminal
void limpiarTerminal();
// espera que usuario presione enter
void esperarEnter();
// muestra el menú principal y devuelve opción
int mostrarMenuPrincipal();
// muestra menú de modelado 3D
int mostrarMenuModelado();
// muestra stats de caché con formato
void mostrarEstadisticasCache(const Cache& cache, double simTime);
// carga fuente tipográfica desde archivo
bool cargarFuente(sf::Font& font, const std::string& path);

// implementación función limpiar terminal
void limpiarTerminal() {
    // código diferente para windows
    #ifdef _WIN32
    if (system("cls") == -1) {
        std::cerr << "Error al limpiar terminal\n";
    }
    // código para linux/mac
    #else
    if (system("clear") == -1) {
        std::cerr << "Error al limpiar terminal\n";
    }
    #endif
}

// función que pausa hasta que usuario presione enter
void esperarEnter() {
    std::cout << "\nPresione enter para continuar...";
    // limpia buffer de entrada
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // limpia la pantalla
    limpiarTerminal();
}

// implementación menú principal
int mostrarMenuPrincipal() {
    int opcion;
    // loop hasta obtener input válido
    while (true) {
        std::cout << "\n=== MENU PRINCIPAL ===\n";
        std::cout << "1. Mostrar estadisticas de cache\n";
        std::cout << "2. Modelar figuras 3d\n";
        std::cout << "3. Salir\n";
        std::cout << "Seleccione una opcion (1-3): ";
        
        // verifica si input es válido
        if (std::cin >> opcion && opcion >= 1 && opcion <= 3) {
            // limpia buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return opcion;
        }
        
        // limpia estado de error
        std::cin.clear();
        // descarta entrada inválida
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Opcion no valida. Intente nuevamente.\n";
    }
}

// implementación menú de modelado 3D
int mostrarMenuModelado() {
    int opcion;
    // loop hasta obtener selección válida
    while (true) {
        std::cout << "\n=== MENU DE MODELADO 3D ===\n";
        std::cout << "1. Visualizar cubo\n";
        std::cout << "2. Visualizar piramide\n";
        std::cout << "3. Volver al menu principal\n";
        std::cout << "Seleccione una opcion (1-3): ";
        
        // valida la entrada
        if (std::cin >> opcion && opcion >= 1 && opcion <= 3) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return opcion;
        }
        
        // maneja entrada inválida
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Opcion no valida. Intente nuevamente.\n";
    }
}

// muestra estadísticas formateadas de la caché
void mostrarEstadisticasCache(const Cache& cache, double simTime) {
    std::cout << "\n=== Estadisticas de cache ===\n";
    // muestra tiempo de simulación
    std::cout << "Tiempo simulacion: " << simTime << " ms\n";
    // llama a método de caché para mostrar stats
    cache.imprimirEstadisticas();
    // pausa antes de continuar
    esperarEnter();
}

// verifica y carga fuente desde archivo
bool cargarFuente(sf::Font& font, const std::string& path) {
    struct stat buffer;   
    // verifica que archivo exista y lo carga
    return (stat(path.c_str(), &buffer) == 0) && font.loadFromFile(path);
}

// punto de entrada principal del programa
int main() {
    try {
        // inicializa caché con 1024 bytes, bloques de 64, 4 vías
        Cache cache(1024, 64, 4);
        // genera secuencia de direcciones optimizada
        std::vector<int> addresses = GeneradorDatos::generarSecuenciaOptimizada(1024, 64, 4);

        // marca tiempo inicial para medir simulación
        auto startSim = std::chrono::high_resolution_clock::now();
        // ejecuta secuencia de accesos a caché
        for (int addr : addresses) {
            cache.accederConPrefetch(addr);
        }
        // calcula tiempo transcurrido en milisegundos
        double simTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - startSim).count();

        // objeto para almacenar la fuente
        sf::Font font;
        // intenta cargar fuente arial.ttf
        if (!cargarFuente(font, "arial.ttf")) {
            std::cerr << "Advertencia: usando fuente por defecto\n";
        }

        // verifica si hay soporte para gráficos
        bool graphicMode = (getenv("DISPLAY") != nullptr);
        int option;
        
        // bucle principal de la aplicación
        do {
            // muestra menú y obtiene selección
            option = mostrarMenuPrincipal();
            
            // procesa opción seleccionada
            switch (option) {
                case 1:
                    // muestra estadísticas de caché
                    mostrarEstadisticasCache(cache, simTime);
                    break;
                case 2: {
                    // muestra menú de modelado
                    int modelOption = mostrarMenuModelado();
                    if (modelOption == 1 || modelOption == 2) {
                        // crea ventana de visualización
                        sf::RenderWindow ventana(sf::VideoMode(1024, 768), "Visualizador 3D");
                        if (modelOption == 1) {
                            // visualiza cubo 3D
                            ModelViewer::visualizar(
                                GeneradorModelos3D::generarCubo(2.0f), 
                                cache, 
                                simTime, 
                                graphicMode, 
                                font,
                                ventana
                            );
                        } else {
                            // visualiza pirámide 3D
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
        // captura y muestra errores no controlados
        std::cerr << "\nError: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // termina programa exitosamente
    return EXIT_SUCCESS;
}