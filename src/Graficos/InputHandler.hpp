// protección para evitar inclusiones múltiples
#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

// biblioteca para manejo de entrada de teclado
#include <SFML/Window/Keyboard.hpp>

// estructura que representa el estado de entrada del usuario
struct EstadoEntrada {
    bool w, a, s, d;             // teclas de movimiento WASD
    bool arriba, abajo;          // flechas arriba/abajo (rotación vertical)
    bool izquierda, derecha;     // flechas izquierda/derecha (rotación horizontal)
    bool espacio, ctrl;          // teclas para movimiento vertical
    
    // constructor que inicializa todos los valores en false
    EstadoEntrada() : w(false), a(false), s(false), d(false),
                     arriba(false), abajo(false), 
                     izquierda(false), derecha(false),
                     espacio(false), ctrl(false) {}
};

// clase para manejar la entrada del usuario
class InputHandler {
public:
    // actualiza el estado de entrada basado en las teclas presionadas
    static void actualizar(EstadoEntrada& entrada);
    
    // verifica si una tecla específica está presionada
    static bool teclaPresionada(sf::Keyboard::Key tecla);
};

#endif // INPUT_HANDLER_HPP