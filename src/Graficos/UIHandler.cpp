// incluye el archivo de cabecera de la clase ui handler
#include "UIHandler.hpp"

// incluye biblioteca para formateo de strings
#include <sstream>

// incluye biblioteca para manipulación de formato de salida
#include <iomanip>

// incluye biblioteca para límites numéricos
#include <limits>

// incluye biblioteca para funciones matemáticas
#include <cmath>

// función para inicializar los elementos de la interfaz de usuario
void UIHandler::inicializar(ElementosUI& ui, const sf::Font& fuente, const Cache& cache, 
                          double tiempoSimulacion, size_t cantidadVertices) {
    // verifica si la fuente está cargada correctamente
    if (!fuente.getInfo().family.empty()) {
        // crea un stream para formatear el texto
        std::ostringstream estadisticas;
        // configura precisión de decimales
        estadisticas.precision(2);
        // fija notación de punto fijo
        estadisticas << std::fixed;
        
        // construye el texto de estadísticas
        estadisticas << "Estadisticas de Cache:\n"
                    << "Aciertos: " << cache.getAciertos() << " (" 
                    << static_cast<int>(cache.getTasaAciertos()) << "%)\n"
                    << "Fallos: " << cache.getFallos() << "\n"
                    << "Tiempo: " << tiempoSimulacion << " ms\n"
                    << "Modelo: " << (cantidadVertices == 8 ? "Cubo" : 
                                    (cantidadVertices == 5 ? "Piramide" : "Desconocido"));
                
        // configura el texto de estadísticas
        ui.textoEstadisticas.setFont(fuente);
        ui.textoEstadisticas.setString(estadisticas.str());
        ui.textoEstadisticas.setCharacterSize(16);
        ui.textoEstadisticas.setFillColor(sf::Color::White);
        ui.textoEstadisticas.setPosition(20, 20);
        
        // configura el texto de controles
        ui.textoControles.setFont(fuente);
        ui.textoControles.setString(
            "Controles:\n"
            "WASD: Movimiento\n"
            "Flechas: Rotar\n"
            "ESPACIO/CTRL: Subir/Bajar\n"
            "R: Resetear vista\n"
            "ESC: Salir");
        ui.textoControles.setCharacterSize(16);
        ui.textoControles.setFillColor(sf::Color(200, 200, 200));
        ui.textoControles.setPosition(20, 680);
        
        // configura el texto de posición (inicialmente vacío)
        ui.textoPosicion.setFont(fuente);
        ui.textoPosicion.setCharacterSize(14);
        ui.textoPosicion.setFillColor(sf::Color::Yellow);
        ui.textoPosicion.setPosition(20, 620);
    }
}

// función para actualizar el texto de posición de la cámara
void UIHandler::actualizarTextoPosicion(ElementosUI& ui, float x, float y, float z, 
                                      float rotX, float rotY) {
    // stream para formatear la posición
    std::ostringstream posicion;
    // configura precisión de decimales
    posicion.precision(2);
    // fija notación de punto fijo
    posicion << std::fixed;
    
    // convierte rotación de radianes a grados
    float rotXDeg = rotX * (180.0f / M_PI);
    float rotYDeg = rotY * (180.0f / M_PI);
    
    // construye el string con la posición y rotación
    posicion << "Posicion: X=" << x 
            << " Y=" << y 
            << " Z=" << z
            << "\nRotacion: X=" << rotXDeg << "° Y=" << rotYDeg << "°";
    
    // actualiza el texto en la interfaz
    ui.textoPosicion.setString(posicion.str());
}

// función para dibujar todos los elementos de la interfaz
void UIHandler::dibujar(ElementosUI& ui, sf::RenderWindow& ventana) {
    // verifica que la ventana esté abierta
    if (ventana.isOpen()) {
        // dibuja el texto de estadísticas
        ventana.draw(ui.textoEstadisticas);
        // dibuja el texto de controles
        ventana.draw(ui.textoControles);
        // dibuja el texto de posición
        ventana.draw(ui.textoPosicion);
    }
}