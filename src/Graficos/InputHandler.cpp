// archivo de cabecera de la clase InputHandler
#include "InputHandler.hpp"
// biblioteca para entrada de teclado de SFML
#include <SFML/Window/Keyboard.hpp>

// actualiza el estado de entrada basado en las teclas presionadas
void InputHandler::actualizar(EstadoEntrada& entrada) {
    // actualiza estado de las teclas de movimiento WASD
    entrada.w = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    entrada.a = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    entrada.s = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    entrada.d = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    
    // actualiza estado de las teclas de rotación (flechas)
    entrada.arriba = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    entrada.abajo = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    entrada.izquierda = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    entrada.derecha = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    
    // actualiza estado de las teclas de altura
    entrada.espacio = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    entrada.ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
}

// verifica si una tecla específica está presionada
bool InputHandler::teclaPresionada(sf::Keyboard::Key tecla) {
    return sf::Keyboard::isKeyPressed(tecla);
}