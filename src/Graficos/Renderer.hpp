#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Common/Vertice.hpp"
#include "CameraController.hpp"

class Renderer {
public:
    static void setupConnections(const std::vector<Vertice>& vertices, 
                               std::vector<std::pair<int, int>>& connections);
    static void renderModel(sf::RenderWindow& window, 
                          const std::vector<Vertice>& vertices,
                          const std::vector<std::pair<int, int>>& connections,
                          const Camera& camera);
};

#endif // RENDERER_HPP