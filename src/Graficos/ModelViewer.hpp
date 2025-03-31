#ifndef MODEL_VIEWER_HPP
#define MODEL_VIEWER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Common/Vertice.hpp"
#include "Cache/Cache.hpp"

class ModelViewer {
public:
    static void visualize(const std::vector<Vertice>& vertices, 
                         const Cache& cache,
                         double simulationTime, 
                         bool graphicMode, 
                         const sf::Font& font);
};

#endif // MODEL_VIEWER_HPP