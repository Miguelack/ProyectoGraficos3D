// archivo de cabecera de la clase UIHandler
#include "UIHandler.hpp"
// biblioteca para formateo de texto
#include <sstream>
// biblioteca para manipulación de formato de salida
#include <iomanip>
// biblioteca para límites numéricos
#include <limits>
// biblioteca para funciones matemáticas
#include <cmath>

// inicializa los elementos de la interfaz de usuario con valores predeterminados
void UIHandler::inicializar(ElementosUI& ui, const sf::Font& fuente, const Cache& cache, 
                          double tiempoSimulacion, size_t cantidadVertices) {
    // verifica si la fuente cargada es válida
    if (!fuente.getInfo().family.empty()) {
        // stream para formatear las estadísticas
        std::ostringstream estadisticas;
        estadisticas.precision(2);
        estadisticas << std::fixed;
        
        // configura el texto con las estadísticas de la caché
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

// actualiza el texto que muestra la posición y rotación de la cámara
void UIHandler::actualizarTextoPosicion(ElementosUI& ui, float x, float y, float z, 
                                      float rotX, float rotY) {
    // stream para formatear la posición
    std::ostringstream posicion;
    posicion.precision(2);
    posicion << std::fixed;
    
    // convierte la rotación de radianes a grados para mejor legibilidad
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

// dibuja todos los elementos de la interfaz en la ventana especificada
void UIHandler::dibujar(ElementosUI& ui, sf::RenderWindow& ventana) {
    // verifica que la ventana esté abierta antes de dibujar
    if (ventana.isOpen()) {
        ventana.draw(ui.textoEstadisticas);
        ventana.draw(ui.textoControles);
        ventana.draw(ui.textoPosicion);
    }
}