// incluye el archivo de cabecera del manejador de entradas
#include "Graficos/InputHandler.hpp"

// incluye las funciones para manejar el teclado de SFML
#include <SFML/Window/Keyboard.hpp>

// incluye las funciones para manejar el mouse de SFML
#include <SFML/Window/Mouse.hpp>

// incluye la clase para manejar la ventana de renderizado
#include <SFML/Graphics/RenderWindow.hpp>

// funcion principal que actualiza el estado de las entradas
void InputHandler::actualizar(EstadoEntrada& entrada, sf::RenderWindow& ventana) {
    // actualiza el estado de la tecla W (movimiento adelante)
    entrada.w = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    
    // actualiza el estado de la tecla A (movimiento izquierda)
    entrada.a = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    
    // actualiza el estado de la tecla S (movimiento atras)
    entrada.s = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    
    // actualiza el estado de la tecla D (movimiento derecha)
    entrada.d = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    
    // actualiza el estado de la tecla espacio (movimiento arriba)
    entrada.espacio = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    
    // actualiza el estado de la tecla shift (movimiento abajo)
    entrada.ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    
    // maneja la captura del mouse con la tecla escape
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        // cambia el estado de captura del mouse
        entrada.mouseCapturado = !entrada.mouseCapturado;
        // muestra u oculta el cursor segun el estado
        ventana.setMouseCursorVisible(!entrada.mouseCapturado);
    }
    
    // procesa el movimiento del mouse cuando esta capturado
    if (entrada.mouseCapturado) {
        // calcula el centro de la ventana
        sf::Vector2i centroVentana(
            static_cast<int>(ventana.getSize().x) / 2, 
            static_cast<int>(ventana.getSize().y) / 2
        );
        
        // obtiene la posicion actual del mouse
        sf::Vector2i posicionRaton = sf::Mouse::getPosition(ventana);
        
        // calcula el movimiento relativo al centro
        entrada.mouseX = static_cast<float>(posicionRaton.x - centroVentana.x);
        entrada.mouseY = static_cast<float>(posicionRaton.y - centroVentana.y);
        
        // vuelve a centrar el cursor
        sf::Mouse::setPosition(centroVentana, ventana);
    } else {
        // si el mouse no esta capturado, no hay movimiento
        entrada.mouseX = 0;
        entrada.mouseY = 0;
    }
}

// funcion auxiliar para verificar el estado de una tecla
bool InputHandler::teclaPresionada(sf::Keyboard::Key tecla) {
    // devuelve true si la tecla esta presionada
    return sf::Keyboard::isKeyPressed(tecla);
}