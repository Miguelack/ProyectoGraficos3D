#include "Graficos/CameraController.hpp"
#include "Graficos/InputHandler.hpp"
#include <cmath>
#include <algorithm>

// Constantes ajustadas para movimiento más lento y realista
const float CameraController::VELOCIDAD_MOVIMIENTO = 0.05f;  // Reducida para mayor realismo
const float CameraController::VELOCIDAD_ROTACION = 0.002f;   // Sensibilidad más baja
const float CameraController::FACTOR_LERP = 0.1f;           // Interpolación más suave

// Constantes para movimiento vertical
const float ACELERACION_VERTICAL = 0.3f;
const float VELOCIDAD_VERTICAL_MAX = 0.8f;
const float ALTURA_MAXIMA = 2.0f;
const float ALTURA_MINIMA = 0.5f;
const float LIMITE_ROTACION_X = 1.48f; // ~85° en radianes

void CameraController::actualizar(Camara& camara, const EstadoEntrada& entrada, float deltaTiempo) {
    // asegura que deltaTime esté en un rango válido
    deltaTiempo = std::clamp(deltaTiempo, 0.001f, 0.1f);
    
    // calcula factores de movimiento basados en el tiempo
    const float factorMovimiento = VELOCIDAD_MOVIMIENTO * deltaTiempo * 60.0f;
    const float factorRotacion = VELOCIDAD_ROTACION * deltaTiempo * 60.0f;
    
    // precalcula valores trigonométricos para mejor performance
    const float senoY = sin(camara.rotY);
    const float cosenoY = cos(camara.rotY);
    
    // MOVIMIENTO HORIZONTAL (WASD) - Más lento y suave
    if (entrada.w) { // adelante
        camara.objetivoX += senoY * factorMovimiento;
        camara.objetivoZ += cosenoY * factorMovimiento;
    }
    if (entrada.s) { // atrás
        camara.objetivoX -= senoY * factorMovimiento;
        camara.objetivoZ -= cosenoY * factorMovimiento;
    }
    if (entrada.a) { // izquierda
        camara.objetivoX -= cosenoY * factorMovimiento;
        camara.objetivoZ += senoY * factorMovimiento;
    }
    if (entrada.d) { // derecha
        camara.objetivoX += cosenoY * factorMovimiento;
        camara.objetivoZ -= senoY * factorMovimiento;
    }
    
    // MOVIMIENTO VERTICAL (ESPACIO/SHIFT) - Estilo CS 1.6
    static float velocidadVerticalActual = 0.0f;
    
    if (entrada.espacio) {
        velocidadVerticalActual += ACELERACION_VERTICAL * deltaTiempo;
    } 
    else if (entrada.ctrl) {
        velocidadVerticalActual -= ACELERACION_VERTICAL * deltaTiempo;
    } 
    else {
        // Fricción/desaceleración cuando no hay entrada
        velocidadVerticalActual *= 0.8f;
    }
    
    velocidadVerticalActual = std::clamp(velocidadVerticalActual, -VELOCIDAD_VERTICAL_MAX, VELOCIDAD_VERTICAL_MAX);
    camara.objetivoY += velocidadVerticalActual * factorMovimiento;
    
    // Limitar altura de la cámara
    camara.objetivoY = std::clamp(camara.objetivoY, ALTURA_MINIMA, ALTURA_MAXIMA);
    
    // ROTACIÓN CON RATÓN - Estilo FPS
    if (entrada.mouseCapturado) {
        camara.rotY -= entrada.mouseX * factorRotacion;
        camara.rotX -= entrada.mouseY * factorRotacion;
    }
    
    // Limitar rotación vertical como en FPS
    camara.rotX = std::clamp(camara.rotX, -LIMITE_ROTACION_X, LIMITE_ROTACION_X);
    
    // normaliza el ángulo de rotación horizontal
    camara.rotY = fmod(camara.rotY, 2.0f * static_cast<float>(M_PI));
    
    // aplica interpolación lineal para movimiento suave
    camara.x += (camara.objetivoX - camara.x) * FACTOR_LERP;
    camara.y += (camara.objetivoY - camara.y) * FACTOR_LERP;
    camara.z += (camara.objetivoZ - camara.z) * FACTOR_LERP;
}

void CameraController::transformarVertice(Vertice& v, const Camara& cam) {
    // Convertir a coordenadas relativas a la cámara
    float dx = v.x - cam.x;
    float dy = v.y - cam.y;
    float dz = v.z - cam.z;

    // Rotación Y (horizontal)
    float cosY = cos(cam.rotY), sinY = sin(cam.rotY);
    float x1 = dx * cosY + dz * sinY;
    float z1 = -dx * sinY + dz * cosY;

    // Rotación X (vertical) - limitada a ±85 grados
    float rotX = std::clamp(cam.rotX, -1.48f, 1.48f); // ~85 grados en radianes
    float cosX = cos(rotX), sinX = sin(rotX);
    v.y = dy * cosX - z1 * sinX;
    v.z = dy * sinX + z1 * cosX;
    v.x = x1;
}