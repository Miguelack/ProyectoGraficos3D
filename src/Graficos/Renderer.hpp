// protección para evitar inclusiones múltiples
#ifndef RENDERER_HPP
#define RENDERER_HPP

// biblioteca para gráficos de SFML
#include <SFML/Graphics.hpp>
// contenedor para almacenar pares de conexiones
#include <vector>
// definición de la estructura Vertice
#include "Common/Vertice.hpp"
// controlador de cámara para transformaciones
#include "CameraController.hpp"

// clase para manejar el renderizado de modelos 3D
class Renderer {
public:
    // configura las conexiones entre vértices para un modelo
    static void configurarConexiones(const std::vector<Vertice>& vertices, 
                                   std::vector<std::pair<int, int>>& connections);
    
    // renderiza un modelo 3D en la ventana especificada
    static void renderizarModelo(sf::RenderWindow& window, 
                               const std::vector<Vertice>& vertices,
                               const std::vector<std::pair<int, int>>& connections,
                               const Camara& camera);
};

#endif // RENDERER_HPP