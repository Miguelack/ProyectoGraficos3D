#include "Cache/Cache.hpp"
#include "Graficos/Graficos.hpp"
#include "DataGenerators/GeneradorDatos.hpp"
#include "DataGenerators/GeneradorModelos3D.hpp"
#include "DataLoaders/CargadorDatos.hpp"
#include "DataLoaders/LectorModelos3D.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

int main() {

    Cache cache(1024, 64, 4); // 1 KB, bloques de 64 bytes, 4-way

    std::vector<int> direcciones = GeneradorDatos::generarSecuenciaLineal(0, 512, 64);
    for (int dir : direcciones) {
        cache.acceder(dir);
    }
    cache.imprimirEstadisticas();

    // Ejemplo de generacion de un cubo
    std::vector<Vertice> cubo = GeneradorModelos3D::generarCubo(2.0f);

    // Crear una ventana SFML
    sf::RenderWindow ventana(sf::VideoMode(800, 600), "Cubo 3D");
    ventana.setFramerateLimit(60);

    // Bucle principal de la ventana
    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();
        }

        // Limpiar la ventana
        ventana.clear();

        // Dibujar el cubo
        for (const auto& v : cubo) {
            sf::CircleShape punto(5);  // representacion del vertice como circulo
            punto.setPosition(400 + v.x * 100, 300 - v.y * 100);  // Escalar y centrar
            ventana.draw(punto);
        }

        // Mostrar lo dibujado
        ventana.display();
    }

    return 0;
}