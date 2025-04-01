// biblioteca para operaciones de entrada/salida básica
#include <iostream>
// biblioteca con algoritmos útiles como limitar valores
#include <algorithm>
// biblioteca para manejo de excepciones
#include <stdexcept>

// archivos de cabecera del proyecto
#include "ModelViewer.hpp"
#include "InputHandler.hpp"
#include "CameraController.hpp"
#include "Renderer.hpp"
#include "UIHandler.hpp"
// bibliotecas para gráficos y manejo de ventanas
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// función principal que controla la visualización 3D
void ModelViewer::visualizar(const std::vector<Vertice>& vertices, 
                           const Cache& cache,
                           double tiempoSimulacion, 
                           bool modoGrafico, 
                           const sf::Font& fuente) {
    // si está en modo consola, termina la ejecución
    if (!modoGrafico) {
        return; // no se requiere renderizado gráfico
    }

    // configuración avanzada de la ventana
    sf::ContextSettings configuracion;
    configuracion.antialiasingLevel = 4; // suavizado de bordes (4x)
    configuracion.majorVersion = 3; // versión mayor de OpenGL
    configuracion.minorVersion = 3; // versión menor de OpenGL
    configuracion.depthBits = 24; // bits para buffer de profundidad

    try {
        // crea y configura la ventana principal
        sf::RenderWindow ventana(sf::VideoMode(1024, 768), "Simulador 3D", sf::Style::Default, configuracion);
        ventana.setVerticalSyncEnabled(true); // sincronización vertical activada
        ventana.setFramerateLimit(60); // límite de 60 cuadros por segundo

        // verifica la inicialización correcta de OpenGL
        if (!ventana.setActive(true)) {
            throw std::runtime_error("error al iniciar OpenGL");
        }

        // prepara las conexiones entre vértices para dibujar las aristas
        std::vector<std::pair<int, int>> conexiones;
        Renderer::configurarConexiones(vertices, conexiones);
        
        // inicializa los elementos de la interfaz de usuario
        UIHandler::ElementosUI interfaz;
        UIHandler::inicializar(interfaz, fuente, cache, tiempoSimulacion, vertices.size());
        
        // configura la cámara inicial
        Camara camara;
        // estado actual de las entradas del usuario
        EstadoEntrada entrada;
        // reloj para controlar el tiempo entre cuadros
        sf::Clock reloj;

        // bucle principal de la aplicación
        while (ventana.isOpen()) {
            try {
                // calcula el tiempo entre cuadros
                float deltaTiempo = reloj.restart().asSeconds();
                // limita el valor para evitar problemas
                deltaTiempo = std::clamp(deltaTiempo, 0.001f, 0.1f);

                // procesamiento de eventos del sistema
                sf::Event evento;
                while (ventana.pollEvent(evento)) {
                    // cierra la ventana con ESC o la X
                    if (evento.type == sf::Event::Closed || 
                       (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape)) {
                        ventana.close();
                    }
                    // reinicia la cámara al presionar R
                    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::R) {
                        camara.objetivoX = 0; camara.objetivoY = 0; camara.objetivoZ = 5.0f;
                        camara.rotX = 0; camara.rotY = 0;
                    }
                }

                // actualiza el estado de las entradas
                InputHandler::actualizar(entrada);
                // mueve la cámara según las entradas
                CameraController::actualizar(camara, entrada, deltaTiempo);
                // actualiza la información de posición en la UI
                UIHandler::actualizarTextoPosicion(interfaz, camara.x, camara.y, camara.z, camara.rotX, camara.rotY);

                // prepara el nuevo cuadro
                ventana.clear(sf::Color::Black); // fondo negro
                // dibuja el modelo 3D con modo mixto (caras + aristas)
                Renderer::renderizarModelo(ventana, vertices, conexiones, camara, Renderer::MODO_MIXTO);
                // dibuja los elementos de la interfaz
                UIHandler::dibujar(interfaz, ventana);
                // muestra el cuadro terminado
                ventana.display();

            } catch (const std::exception& e) {
                // maneja errores durante la ejecución
                std::cerr << "error en el bucle principal: " << e.what() << "\n";
                if (!ventana.isOpen()) break;
            }
        }

        // cierra adecuadamente la ventana
        ventana.close();
        // breve pausa para asegurar el cierre
        sf::sleep(sf::milliseconds(50));

    } catch (const std::exception& e) {
        // maneja errores críticos de inicialización
        std::cerr << "error fatal al iniciar: " << e.what() << "\n";
    }
}