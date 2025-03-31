#ifndef CAMERA_CONTROLLER_HPP
#define CAMERA_CONTROLLER_HPP

#include "Common/Vertice.hpp"

// Añade esta declaración forward o incluye el header correspondiente
struct InputState;  // Declaración forward

struct Camera {
    float x, y, z;
    float rotX, rotY;
    float targetX, targetY, targetZ;
    float velocidadMovimiento;
    float velocidadRotacion;
    float lerpFactor;
    
    Camera() : x(0), y(0), z(5.0f), rotX(0), rotY(0),
              targetX(0), targetY(0), targetZ(5.0f),
              velocidadMovimiento(0.2f), velocidadRotacion(0.05f),
              lerpFactor(0.2f) {}
};

class CameraController {
public:
    static const float MOVEMENT_SPEED;
    static const float ROTATION_SPEED;
    static const float LERP_FACTOR;
    
    static void update(Camera& camera, const InputState& input, float deltaTime);
    static void transformVertex(Vertice& v, const Camera& camera);
};

#endif // CAMERA_CONTROLLER_HPP