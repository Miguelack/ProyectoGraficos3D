#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Common/Vertice.hpp"
#include "CameraController.hpp"

class Renderer {
public:
    // modo de renderizado para controlar la visualización
    enum ModoRenderizado {
        MODO_LINEAS,      // solo aristas
        MODO_SOLIDO,      // caras sólidas
        MODO_MIXTO        // aristas sobre caras
    };

    static void configurarConexiones(const std::vector<Vertice>& vertices, 
                                   std::vector<std::pair<int, int>>& conexiones);
    
    static void renderizarModelo(sf::RenderWindow& ventana, 
                               const std::vector<Vertice>& vertices,
                               const std::vector<std::pair<int, int>>& conexiones,
                               const Camara& camara,
                               ModoRenderizado modo = MODO_MIXTO);
    
    // nuevo método para generar caras
    static void generarCaras(const std::vector<Vertice>& vertices,
                           std::vector<std::vector<int>>& caras,
                           size_t tipoModelo);
};

#endif // RENDERER_HPP