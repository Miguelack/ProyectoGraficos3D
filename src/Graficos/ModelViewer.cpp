// incluye librería para entrada/salida estándar
#include <iostream>

// incluye librería para algoritmos como std::clamp
#include <algorithm>

// incluye librería para manejo de excepciones
#include <stdexcept>

// incluye cabecera del visualizador de modelos
#include "ModelViewer.hpp"

// incluye manejador de entradas de usuario
#include "InputHandler.hpp"

// incluye controlador de cámara
#include "CameraController.hpp"

// incluye renderizador gráfico
#include "Renderer.hpp"

// incluye manejador de interfaz de usuario
#include "UIHandler.hpp"

// incluye módulos gráficos de SFML
#include <SFML/Graphics.hpp>

// incluye módulos de ventana de SFML
#include <SFML/Window.hpp>

// función principal para visualización del modelo
void ModelViewer::visualizar(const std::vector<Vertice>& vertices, 
                           const Cache& cache,
                           double tiempoSimulacion, 
                           bool modoGrafico, 
                           const sf::Font& fuente,
                           sf::RenderWindow& ventana) {
    // si no está en modo gráfico, termina la función
    if (!modoGrafico) {
        return;
    }

    // bloque try-catch para manejo de errores
    try {
        // verifica si la ventana está abierta
        if (!ventana.isOpen()) {
            throw std::runtime_error("Ventana recibida no está abierta");
        }

        // intenta activar el contexto OpenGL
        if (!ventana.setActive(true)) {
            throw std::runtime_error("Error al iniciar OpenGL");
        }

        // configura el cursor del mouse (oculto y capturado)
        ventana.setMouseCursorVisible(false);
        ventana.setMouseCursorGrabbed(true);
        // centra el cursor en la ventana
        sf::Mouse::setPosition(sf::Vector2i(ventana.getSize().x/2, ventana.getSize().y/2), ventana);

        // vector para almacenar conexiones entre vértices
        std::vector<std::pair<int, int>> conexiones;
        // configura las conexiones entre vértices
        Renderer::configurarConexiones(vertices, conexiones);
        
        // estructura para elementos de la interfaz de usuario
        UIHandler::ElementosUI interfaz;
        // inicializa los elementos de la interfaz
        UIHandler::inicializar(interfaz, fuente, cache, tiempoSimulacion, vertices.size());
        
        // crea una cámara y estado de entrada
        Camara camara;
        EstadoEntrada entrada;
        // reloj para medir tiempo entre frames
        sf::Clock reloj;

        // bucle principal de renderizado
        while (ventana.isOpen()) {
            try {
                // calcula tiempo entre frames
                float deltaTiempo = reloj.restart().asSeconds();
                // limita el delta de tiempo
                deltaTiempo = std::clamp(deltaTiempo, 0.001f, 0.1f);

                // procesamiento de eventos SFML
                sf::Event evento;
                while (ventana.pollEvent(evento)) {
                    // cierra la ventana con escape o click en cerrar
                    if (evento.type == sf::Event::Closed || 
                       (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape)) {
                        ventana.close();
                    }
                    // resetea la cámara con tecla R
                    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::R) {
                        camara = Camara();
                    }
                }

                // actualiza entradas del usuario
                InputHandler::actualizar(entrada, ventana);
                // actualiza posición de la cámara
                CameraController::actualizar(camara, entrada, deltaTiempo);
                // actualiza texto de posición en la interfaz
                UIHandler::actualizarTextoPosicion(interfaz, camara.x, camara.y, camara.z, camara.rotX, camara.rotY);

                // limpia la ventana con color de fondo
                ventana.clear(sf::Color(45, 45, 60));
                // renderiza el modelo 3D
                Renderer::renderizarModelo(ventana, vertices, conexiones, camara, Renderer::MODO_MIXTO);
                
                // dibuja puntero FPS en el centro
                Renderer::dibujarPuntero(ventana, sf::Color::White);
                
                // dibuja la interfaz de usuario
                UIHandler::dibujar(interfaz, ventana);
                // muestra el frame renderizado
                ventana.display();

            } catch (const std::exception& e) {
                // maneja errores en el bucle principal
                std::cerr << "Error en el bucle principal: " << e.what() << "\n";
                if (!ventana.isOpen()) break;
            }
        }

        // cierra la ventana al terminar
        ventana.close();
        // pequeña pausa antes de terminar
        sf::sleep(sf::milliseconds(50));

    } catch (const std::exception& e) {
        // maneja errores fatales
        std::cerr << "Error fatal en ModelViewer: " << e.what() << "\n";
        throw;
    }
}