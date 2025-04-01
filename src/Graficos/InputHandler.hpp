#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp> // Añade esta línea

struct EstadoEntrada {
    bool w, a, s, d;             // Teclas de movimiento WASD
    bool espacio, ctrl;          // Teclas para movimiento vertical
    float mouseX, mouseY;        // Movimiento del ratón
    bool mouseCapturado;         // Estado del ratón
    
    EstadoEntrada() : 
        w(false), a(false), s(false), d(false),
        espacio(false), ctrl(false),
        mouseX(0), mouseY(0),
        mouseCapturado(true) {}
};

class InputHandler {
public:
    static void actualizar(EstadoEntrada& entrada, sf::RenderWindow& ventana);
    static bool teclaPresionada(sf::Keyboard::Key tecla);
};

#endif // INPUT_HANDLER_HPP