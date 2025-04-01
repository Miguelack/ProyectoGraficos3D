// proteccion para evitar inclusiones multiples del archivo
#ifndef MODEL_VIEWER_HPP
#define MODEL_VIEWER_HPP

// biblioteca para graficos de sfml
#include <SFML/Graphics.hpp>
// contenedor para almacenar vertices
#include <vector>
// definicion de la estructura vertice
#include "Common/Vertice.hpp"
// definicion de la estructura de cache
#include "Cache/Cache.hpp"

// clase principal para la visualizacion 3d
class ModelViewer {
public:
    // metodo estatico para visualizar modelos 3d
    static void visualizar(const std::vector<Vertice>& vertices, 
                         const Cache& cache,
                         double tiempoSimulacion, 
                         bool modoGrafico, 
                         const sf::Font& fuente);
    /*
     * parametros:
     * - vertices: lista de vertices del modelo 3d
     * - cache: estructura de cache para simulacion
     * - tiempoSimulacion: duracion de la simulacion
     * - modoGrafico: true para graficos, false para consola
     * - fuente: tipo de letra para la interfaz
     */
};

// fin de la proteccion de inclusion
#endif // MODEL_VIEWER_HPP