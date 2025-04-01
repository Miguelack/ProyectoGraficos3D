#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "ModelViewer.hpp"
#include "InputHandler.hpp"
#include "CameraController.hpp"
#include "Renderer.hpp"
#include "UIHandler.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

void ModelViewer::visualizar(const std::vector<Vertice>& vertices, 
                           const Cache& cache,
                           double tiempoSimulacion, 
                           bool modoGrafico, 
                           const sf::Font& fuente,
                           sf::RenderWindow& ventana) {  // Añadido parámetro ventana
    if (!modoGrafico) {
        return;
    }

    try {
        // Configuración avanzada de la ventana (ya no se crea aquí, se recibe como parámetro)
        if (!ventana.isOpen()) {
            throw std::runtime_error("Ventana recibida no está abierta");
        }

        // Configuración de OpenGL
        if (!ventana.setActive(true)) {
            throw std::runtime_error("Error al iniciar OpenGL");
        }

        // Configurar el modo de ratón (capturado para controles FPS)
        ventana.setMouseCursorVisible(false);
        ventana.setMouseCursorGrabbed(true);
        sf::Mouse::setPosition(sf::Vector2i(ventana.getSize().x/2, ventana.getSize().y/2), ventana);

        // Configurar conexiones entre vértices
        std::vector<std::pair<int, int>> conexiones;
        Renderer::configurarConexiones(vertices, conexiones);
        
        // Inicializar interfaz de usuario
        UIHandler::ElementosUI interfaz;
        UIHandler::inicializar(interfaz, fuente, cache, tiempoSimulacion, vertices.size());
        
        // Configurar cámara y controles
        Camara camara;
        EstadoEntrada entrada;
        sf::Clock reloj;

        // Bucle principal de renderizado
        while (ventana.isOpen()) {
            try {
                float deltaTiempo = reloj.restart().asSeconds();
                deltaTiempo = std::clamp(deltaTiempo, 0.001f, 0.1f);

                // Procesamiento de eventos
                sf::Event evento;
                while (ventana.pollEvent(evento)) {
                    if (evento.type == sf::Event::Closed || 
                       (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape)) {
                        ventana.close();
                    }
                    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::R) {
                        camara = Camara(); // Reset completo de la cámara
                    }
                }

                // Actualización del estado del juego
                InputHandler::actualizar(entrada, ventana);
                CameraController::actualizar(camara, entrada, deltaTiempo);
                UIHandler::actualizarTextoPosicion(interfaz, camara.x, camara.y, camara.z, camara.rotX, camara.rotY);

                // Renderizado
                ventana.clear(sf::Color(45, 45, 60)); // Fondo gris azulado
                Renderer::renderizarModelo(ventana, vertices, conexiones, camara, Renderer::MODO_MIXTO);
                UIHandler::dibujar(interfaz, ventana);
                ventana.display();

            } catch (const std::exception& e) {
                std::cerr << "Error en el bucle principal: " << e.what() << "\n";
                if (!ventana.isOpen()) break;
            }
        }

        // Limpieza final
        ventana.close();
        sf::sleep(sf::milliseconds(50));

    } catch (const std::exception& e) {
        std::cerr << "Error fatal en ModelViewer: " << e.what() << "\n";
        throw;
    }
}