#include "InputHandler.hpp"
#include <SFML/Window/Keyboard.hpp>

void InputHandler::update(InputState& input) {
    // Actualizar estado de las teclas de movimiento
    input.w = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    input.a = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    input.s = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    input.d = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    
    // Actualizar estado de las teclas de rotación
    input.up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    input.down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    input.left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    input.right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    
    // Actualizar estado de las teclas de altura
    input.space = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    input.ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
}

bool InputHandler::isKeyPressed(sf::Keyboard::Key key) {
    // Método de conveniencia para verificar estado de teclas
    return sf::Keyboard::isKeyPressed(key);
}