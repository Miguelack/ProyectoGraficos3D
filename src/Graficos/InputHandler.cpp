#include "Graficos/InputHandler.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp> // Añade esta línea

void InputHandler::actualizar(EstadoEntrada& entrada, sf::RenderWindow& ventana) {
    // Teclas de movimiento WASD
    entrada.w = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    entrada.a = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    entrada.s = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    entrada.d = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    
    // Teclas para movimiento vertical
    entrada.espacio = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    entrada.ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    
    // Captura/libera el ratón
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        entrada.mouseCapturado = !entrada.mouseCapturado;
        ventana.setMouseCursorVisible(!entrada.mouseCapturado);
    }
    
    // Movimiento del ratón
    if (entrada.mouseCapturado) {
        sf::Vector2i centroVentana(
            static_cast<int>(ventana.getSize().x) / 2, 
            static_cast<int>(ventana.getSize().y) / 2
        );
        sf::Vector2i posicionRaton = sf::Mouse::getPosition(ventana);
        
        entrada.mouseX = static_cast<float>(posicionRaton.x - centroVentana.x);
        entrada.mouseY = static_cast<float>(posicionRaton.y - centroVentana.y);
        
        sf::Mouse::setPosition(centroVentana, ventana);
    } else {
        entrada.mouseX = 0;
        entrada.mouseY = 0;
    }
}

bool InputHandler::teclaPresionada(sf::Keyboard::Key tecla) {
    return sf::Keyboard::isKeyPressed(tecla);
}