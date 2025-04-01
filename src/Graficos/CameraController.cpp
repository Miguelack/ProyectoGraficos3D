// Inclusión del archivo de cabecera del controlador de cámara
#include "Graficos/CameraController.hpp"

// Inclusión del manejador de entradas de usuario
#include "Graficos/InputHandler.hpp"

// Librería para operaciones matemáticas (sin, cos, etc.)
#include <cmath>

// Librería para algoritmos como std::clamp
#include <algorithm>

// Velocidad base para movimiento de la cámara (0.05 unidades/frame)
const float CameraController::VELOCIDAD_MOVIMIENTO = 0.05f;

// Sensibilidad de rotación de la cámara (0.002 rad/pixel)
const float CameraController::VELOCIDAD_ROTACION = 0.002f;

// Factor de interpolación para movimientos suaves (10% por frame)
const float CameraController::FACTOR_LERP = 0.1f;

// Aceleración para movimientos verticales
const float ACELERACION_VERTICAL = 0.3f;

// Velocidad máxima vertical permitida
const float VELOCIDAD_VERTICAL_MAX = 0.8f;

// Altura máxima a la que puede subir la cámara
const float ALTURA_MAXIMA = 2.0f;

// Altura mínima a la que puede bajar la cámara
const float ALTURA_MINIMA = 0.5f;

// Límite de rotación vertical en radianes (~85 grados)
const float LIMITE_ROTACION_X = 1.48f;

// Función principal que actualiza la posición y rotación de la cámara
void CameraController::actualizar(Camara& camara, const EstadoEntrada& entrada, float deltaTiempo) {
    // Limita el deltaTime para evitar valores extremadamente altos o bajos
    deltaTiempo = std::clamp(deltaTiempo, 0.001f, 0.1f);
    
    // Calcula factores de movimiento ajustados al tiempo transcurrido
    const float factorMovimiento = VELOCIDAD_MOVIMIENTO * deltaTiempo * 60.0f;
    const float factorRotacion = VELOCIDAD_ROTACION * deltaTiempo * 60.0f;
    
    // Precalcula seno y coseno de la rotación horizontal para optimización
    const float senoY = sin(camara.rotY);
    const float cosenoY = cos(camara.rotY);
    
    // Movimiento hacia adelante (tecla W)
    if (entrada.w) {
        camara.objetivoX -= senoY * factorMovimiento;
        camara.objetivoZ -= cosenoY * factorMovimiento;
    }
    // Movimiento hacia atrás (tecla S)
    if (entrada.s) {
        camara.objetivoX += senoY * factorMovimiento;
        camara.objetivoZ += cosenoY * factorMovimiento;
    }
    // Movimiento hacia izquierda (tecla A)
    if (entrada.a) {
        camara.objetivoX -= cosenoY * factorMovimiento;
        camara.objetivoZ += senoY * factorMovimiento;
    }
    // Movimiento hacia derecha (tecla D)
    if (entrada.d) {
        camara.objetivoX += cosenoY * factorMovimiento;
        camara.objetivoZ -= senoY * factorMovimiento;
    }
    
    // Variable estática para almacenar velocidad vertical entre frames
    static float velocidadVerticalActual = 0.0f;
    // Movimiento hacia arriba (tecla espacio)
    if (entrada.espacio) {
        velocidadVerticalActual += 0.5f * deltaTiempo;
    } 
    // Movimiento hacia abajo (tecla control)
    else if (entrada.ctrl) {
        velocidadVerticalActual -= 0.5f * deltaTiempo;
    } 
    // Fricción cuando no hay entrada vertical
    else {
        velocidadVerticalActual *= 0.8f;
    }
    // Limita la velocidad vertical entre -1 y 1
    velocidadVerticalActual = std::clamp(velocidadVerticalActual, -1.0f, 1.0f);
    // Aplica el movimiento vertical a la posición objetivo
    camara.objetivoY += velocidadVerticalActual * factorMovimiento;
    // Limita la altura de la cámara entre los valores mínimo y máximo
    camara.objetivoY = std::clamp(camara.objetivoY, 0.5f, 2.0f);
    
    // Rotación con movimiento del ratón (cuando está capturado)
    if (entrada.mouseCapturado) {
        camara.rotY += entrada.mouseX * factorRotacion;
        camara.rotX += entrada.mouseY * factorRotacion;
    }
    
    // Limita la rotación vertical para evitar volteretas
    camara.rotX = std::clamp(camara.rotX, -1.48f, 1.48f);
    
    // Interpolación suave de la posición X
    camara.x += (camara.objetivoX - camara.x) * FACTOR_LERP;
    // Interpolación suave de la posición Y
    camara.y += (camara.objetivoY - camara.y) * FACTOR_LERP;
    // Interpolación suave de la posición Z
    camara.z += (camara.objetivoZ - camara.z) * FACTOR_LERP;
}

// Función que transforma un vértice al espacio de la cámara
void CameraController::transformarVertice(Vertice& v, const Camara& cam) {
    // Calcula la posición relativa del vértice respecto a la cámara
    float dx = v.x - cam.x;
    float dy = v.y - cam.y;
    float dz = v.z - cam.z;

    // Precalcula componentes para rotación horizontal
    float cosY = cos(cam.rotY), sinY = sin(cam.rotY);
    // Aplica rotación horizontal al eje X
    float x1 = dx * cosY + dz * sinY;
    // Aplica rotación horizontal al eje Z
    float z1 = -dx * sinY + dz * cosY;

    // Limita y prepara la rotación vertical
    float rotX = std::clamp(cam.rotX, -1.48f, 1.48f);
    // Precalcula componentes para rotación vertical
    float cosX = cos(rotX), sinX = sin(rotX);
    // Aplica rotación vertical al eje Y
    v.y = dy * cosX - z1 * sinX;
    // Aplica rotación vertical al eje Z
    v.z = dy * sinX + z1 * cosX;
    // Asigna la nueva posición X transformada
    v.x = x1;
}