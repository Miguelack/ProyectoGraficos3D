#include "UIHandler.hpp"
#include <sstream>
#include <iomanip>
#include <limits>  // Para std::numeric_limits
#include <cmath>   // Para funciones matemáticas

void UIHandler::init(UIElements& ui, const sf::Font& font, const Cache& cache, 
                    double simulationTime, size_t vertexCount) {
    // Validación básica de parámetros
    if (!font.getInfo().family.empty()) {  // Verificar si la fuente es válida
        std::ostringstream stats;
        stats.precision(2);
        stats << std::fixed;
        
        // Estadísticas de caché
        stats << "Estadisticas de Cache:\n"
              << "Aciertos: " << cache.getAciertos() << " (" 
              << static_cast<int>(cache.getTasaAciertos()) << "%)\n"
              << "Fallos: " << cache.getFallos() << "\n"
              << "Tiempo: " << simulationTime << " ms\n"
              << "Modelo: " << (vertexCount == 8 ? "Cubo" : 
                               (vertexCount == 5 ? "Piramide" : "Desconocido"));
              
        // Configurar texto de estadísticas
        ui.statsText.setFont(font);
        ui.statsText.setString(stats.str());
        ui.statsText.setCharacterSize(16);
        ui.statsText.setFillColor(sf::Color::White);
        ui.statsText.setPosition(20, 20);
        
        // Configurar texto de controles
        ui.controlsText.setFont(font);
        ui.controlsText.setString(
            "Controles:\n"
            "WASD: Movimiento\n"
            "Flechas: Rotar\n"
            "ESPACIO/CTRL: Subir/Bajar\n"
            "R: Resetear vista\n"
            "ESC: Salir");
        ui.controlsText.setCharacterSize(16);
        ui.controlsText.setFillColor(sf::Color(200, 200, 200));
        ui.controlsText.setPosition(20, 680);
        
        // Configurar texto de posición
        ui.positionText.setFont(font);
        ui.positionText.setCharacterSize(14);
        ui.positionText.setFillColor(sf::Color::Yellow);
        ui.positionText.setPosition(20, 620);
    }
}

void UIHandler::updatePositionText(UIElements& ui, float x, float y, float z, 
                                 float rotX, float rotY) {
    std::ostringstream pos;
    pos.precision(2);
    pos << std::fixed;
    
    // Convertir radianes a grados para mejor legibilidad
    float rotXDeg = rotX * (180.0f / M_PI);
    float rotYDeg = rotY * (180.0f / M_PI);
    
    pos << "Posicion: X=" << x 
        << " Y=" << y 
        << " Z=" << z
        << "\nRotacion: X=" << rotXDeg << "° Y=" << rotYDeg << "°";
    
    ui.positionText.setString(pos.str());
}

void UIHandler::draw(UIElements& ui, sf::RenderWindow& window) {
    // Verificar si la ventana está abierta antes de dibujar
    if (window.isOpen()) {
        window.draw(ui.statsText);
        window.draw(ui.controlsText);
        window.draw(ui.positionText);
    }
}