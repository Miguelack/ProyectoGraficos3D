// archivo de cabecera de la clase CameraController
#include "CameraController.hpp"
// archivo de cabecera para el estado de entrada
#include "InputHandler.hpp"
// biblioteca para funciones matemáticas
#include <cmath>
// biblioteca para algoritmos como std::clamp
#include <algorithm>

// inicialización de constantes estáticas
const float CameraController::VELOCIDAD_MOVIMIENTO = 0.2f;
const float CameraController::VELOCIDAD_ROTACION = 0.05f;
const float CameraController::FACTOR_LERP = 0.2f;

// actualiza la posición y rotación de la cámara
void CameraController::actualizar(Camara& camara, const EstadoEntrada& entrada, float deltaTiempo) {
    // asegura que deltaTime esté en un rango válido
    deltaTiempo = std::clamp(deltaTiempo, 0.001f, 0.1f);
    
    // calcula factores de movimiento basados en el tiempo
    const float factorMovimiento = VELOCIDAD_MOVIMIENTO * deltaTiempo * 60.0f;
    const float factorRotacion = VELOCIDAD_ROTACION * deltaTiempo * 60.0f;
    
    // precalcula valores trigonométricos para mejor performance
    const float senoY = sin(camara.rotY);
    const float cosenoY = cos(camara.rotY);
    
    // movimiento relativo basado en entrada
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
    
    // movimiento vertical
    if (entrada.espacio) camara.objetivoY -= factorMovimiento;
    if (entrada.ctrl) camara.objetivoY += factorMovimiento;
    
    // rotación de cámara
    if (entrada.arriba) camara.rotX -= factorRotacion;
    if (entrada.abajo) camara.rotX += factorRotacion;
    if (entrada.izquierda) camara.rotY -= factorRotacion;
    if (entrada.derecha) camara.rotY += factorRotacion;
    
    // normaliza los ángulos de rotación
    camara.rotX = fmod(camara.rotX, 2.0f * M_PI);
    camara.rotY = fmod(camara.rotY, 2.0f * M_PI);
    
    // aplica interpolación lineal para movimiento suave
    camara.x += (camara.objetivoX - camara.x) * FACTOR_LERP;
    camara.y += (camara.objetivoY - camara.y) * FACTOR_LERP;
    camara.z += (camara.objetivoZ - camara.z) * FACTOR_LERP;
}

// transforma un vértice al espacio de la cámara
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