#include <iostream>  // Necesario para std::cerr
#include <algorithm> // Necesario para std::clamp
#include <stdexcept> // Necesario para std::runtime_error

#include "ModelViewer.hpp"
#include "InputHandler.hpp"
#include "CameraController.hpp"
#include "Renderer.hpp"
#include "UIHandler.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

void ModelViewer::visualize(const std::vector<Vertice>& vertices, 
                          const Cache& cache,
                          double simulationTime, 
                          bool graphicMode, 
                          const sf::Font& font) {
    if (!graphicMode) {
        // Modo texto
        return;
    }

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.depthBits = 24;

    try {
        sf::RenderWindow window(sf::VideoMode(1024, 768), "Simulador 3D", sf::Style::Default, settings);
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(60);

        if (!window.setActive(true)) {
            throw std::runtime_error("No se pudo inicializar OpenGL");
        }

        // Inicialización de componentes
        std::vector<std::pair<int, int>> connections;
        Renderer::setupConnections(vertices, connections);
        
        UIHandler::UIElements ui;
        UIHandler::init(ui, font, cache, simulationTime, vertices.size());
        
        Camera camera;
        InputState input;
        sf::Clock clock;

        while (window.isOpen()) {
            try {
                float deltaTime = clock.restart().asSeconds();
                deltaTime = std::clamp(deltaTime, 0.001f, 0.1f);

                // Manejo de eventos
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed || 
                       (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                        window.close();
                    }
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                        camera.targetX = 0; camera.targetY = 0; camera.targetZ = 5.0f;
                        camera.rotX = 0; camera.rotY = 0;
                    }
                }

                // Actualización
                InputHandler::update(input);
                CameraController::update(camera, input, deltaTime);
                UIHandler::updatePositionText(ui, camera.x, camera.y, camera.z, camera.rotX, camera.rotY);

                // Renderizado
                window.clear(sf::Color::Black);
                Renderer::renderModel(window, vertices, connections, camera);
                UIHandler::draw(ui, window);
                window.display();

            } catch (const std::exception& e) {
                std::cerr << "Error en frame: " << e.what() << "\n";
                if (!window.isOpen()) break;
            }
        }

        window.close();
        sf::sleep(sf::milliseconds(50));

    } catch (const std::exception& e) {
        std::cerr << "Error fatal: " << e.what() << "\n";
    }
}