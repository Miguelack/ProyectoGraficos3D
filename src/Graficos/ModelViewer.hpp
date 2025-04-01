#ifndef MODEL_VIEWER_HPP
#define MODEL_VIEWER_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "../Cache/Cache.hpp"
#include "../Common/Vertice.hpp"

class ModelViewer {
public:
    static void visualizar(const std::vector<Vertice>& modelo, 
                         const Cache& cache, 
                         double tiempoSimulacion,
                         bool modoGrafico,
                         const sf::Font& font,
                         sf::RenderWindow& ventana); // Parámetro añadido
};

#endif // MODEL_VIEWER_HPP