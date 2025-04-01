// protección para evitar inclusiones múltiples
#ifndef UI_HANDLER_HPP
#define UI_HANDLER_HPP

// biblioteca para gráficos de SFML
#include <SFML/Graphics.hpp>
// definición de la estructura Cache
#include "Cache/Cache.hpp"

// clase para manejar la interfaz de usuario del simulador 3D
class UIHandler {
public:
    // estructura que contiene los elementos gráficos de la UI
    struct ElementosUI {
        sf::Text textoEstadisticas;  // muestra estadísticas de la simulación
        sf::Text textoControles;     // muestra los controles disponibles
        sf::Text textoPosicion;      // muestra la posición y rotación de la cámara
    };
    
    // inicializa los elementos de la interfaz de usuario
    static void inicializar(ElementosUI& ui, const sf::Font& fuente, const Cache& cache, 
                          double tiempoSimulacion, size_t cantidadVertices);
    
    // actualiza el texto de posición con los nuevos valores de la cámara
    static void actualizarTextoPosicion(ElementosUI& ui, float x, float y, float z, 
                                      float rotX, float rotY);
    
    // dibuja todos los elementos de la interfaz en la ventana
    static void dibujar(ElementosUI& ui, sf::RenderWindow& ventana);
};

#endif // UI_HANDLER_HPP