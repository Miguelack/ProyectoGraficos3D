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
void CameraController::transformarVertice(Vertice& v, const Camara& camara) {
    // traslada el vértice a coordenadas relativas a la cámara
    const float dx = v.x - camara.x;
    const float dy = v.y - camara.y;
    const float dz = v.z - camara.z;
    
    // precalcula valores trigonométricos para mejor performance
    const float cosenoY = cos(camara.rotY);
    const float senoY = sin(camara.rotY);
    const float cosenoX = cos(camara.rotX);
    const float senoX = sin(camara.rotX);
    
    // aplica rotación horizontal (eje Y)
    const float x1 = dx * cosenoY + dz * senoY;
    const float z1 = -dx * senoY + dz * cosenoY;
    
    // aplica rotación vertical (eje X) y actualiza el vértice
    v.y = dy * cosenoX - z1 * senoX;
    v.z = dy * senoX + z1 * cosenoX;
    v.x = x1;
}