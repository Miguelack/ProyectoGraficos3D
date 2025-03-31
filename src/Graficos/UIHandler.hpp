#ifndef UI_HANDLER_HPP
#define UI_HANDLER_HPP

#include <SFML/Graphics.hpp>
#include "Cache/Cache.hpp"

class UIHandler {
public:
    struct UIElements {
        sf::Text statsText;
        sf::Text controlsText;
        sf::Text positionText;
    };
    
    static void init(UIElements& ui, const sf::Font& font, const Cache& cache, 
                    double simulationTime, size_t vertexCount);
    static void updatePositionText(UIElements& ui, float x, float y, float z, 
                                 float rotX, float rotY);
    static void draw(UIElements& ui, sf::RenderWindow& window);
};

#endif // UI_HANDLER_HPP