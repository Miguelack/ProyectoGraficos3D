#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <SFML/Window/Keyboard.hpp>

struct InputState {
    bool w, a, s, d, up, down, left, right, space, ctrl;
    
    InputState() : w(false), a(false), s(false), d(false),
                  up(false), down(false), left(false), right(false),
                  space(false), ctrl(false) {}
};

class InputHandler {
public:
    static void update(InputState& input);
    static bool isKeyPressed(sf::Keyboard::Key key);  // Añade esta declaración
};

#endif // INPUT_HANDLER_HPP