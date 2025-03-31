#include "CameraController.hpp"
#include "InputHandler.hpp"
#include <cmath>
#include <algorithm>

const float CameraController::MOVEMENT_SPEED = 0.2f;
const float CameraController::ROTATION_SPEED = 0.05f;
const float CameraController::LERP_FACTOR = 0.2f;

void CameraController::update(Camera& camera, const InputState& input, float deltaTime) {
    // Asegurar que deltaTime sea válido
    deltaTime = std::clamp(deltaTime, 0.001f, 0.1f);
    
    // Factor de movimiento basado en tiempo delta
    const float moveFactor = MOVEMENT_SPEED * deltaTime * 60.0f;
    const float rotFactor = ROTATION_SPEED * deltaTime * 60.0f;
    
    // Precalcular valores trigonométricos
    const float sinY = sin(camera.rotY);
    const float cosY = cos(camera.rotY);
    
    // Movimiento relativo
    if (input.w) {
        camera.targetX += sinY * moveFactor;
        camera.targetZ += cosY * moveFactor;
    }
    if (input.s) {
        camera.targetX -= sinY * moveFactor;
        camera.targetZ -= cosY * moveFactor;
    }
    if (input.a) {
        camera.targetX -= cosY * moveFactor;
        camera.targetZ += sinY * moveFactor;
    }
    if (input.d) {
        camera.targetX += cosY * moveFactor;
        camera.targetZ -= sinY * moveFactor;
    }
    
    // Movimiento vertical
    if (input.space) camera.targetY -= moveFactor;
    if (input.ctrl) camera.targetY += moveFactor;
    
    // Rotación
    if (input.up) camera.rotX -= rotFactor;
    if (input.down) camera.rotX += rotFactor;
    if (input.left) camera.rotY -= rotFactor;
    if (input.right) camera.rotY += rotFactor;
    
    // Normalizar ángulos
    camera.rotX = fmod(camera.rotX, 2.0f * M_PI);
    camera.rotY = fmod(camera.rotY, 2.0f * M_PI);
    
    // Suavizado de movimiento (interpolación lineal)
    camera.x += (camera.targetX - camera.x) * LERP_FACTOR;
    camera.y += (camera.targetY - camera.y) * LERP_FACTOR;
    camera.z += (camera.targetZ - camera.z) * LERP_FACTOR;
}

void CameraController::transformVertex(Vertice& v, const Camera& camera) {
    // Traslación al espacio de la cámara
    const float dx = v.x - camera.x;
    const float dy = v.y - camera.y;
    const float dz = v.z - camera.z;
    
    // Precalcular valores trigonométricos
    const float cosY = cos(camera.rotY);
    const float sinY = sin(camera.rotY);
    const float cosX = cos(camera.rotX);
    const float sinX = sin(camera.rotX);
    
    // Rotación Y (horizontal)
    const float x1 = dx * cosY + dz * sinY;
    const float z1 = -dx * sinY + dz * cosY;
    
    // Rotación X (vertical)
    v.y = dy * cosX - z1 * sinX;
    v.z = dy * sinX + z1 * cosX;
    v.x = x1;
}