// protección para evitar inclusiones múltiples
#ifndef CAMERA_CONTROLLER_HPP
#define CAMERA_CONTROLLER_HPP

// definición de la estructura Vertice
#include "Common/Vertice.hpp"

// declaración anticipada de EstadoEntrada
struct EstadoEntrada;

// estructura que representa la cámara en el espacio 3D
struct Camara {
    float x, y, z;               // posición actual de la cámara
    float rotX, rotY;             // rotación actual (en radianes)
    float objetivoX, objetivoY, objetivoZ; // posición objetivo para interpolación
    float velocidadMovimiento;    // velocidad de movimiento base
    float velocidadRotacion;      // velocidad de rotación base
    float factorLerp;             // factor de interpolación lineal
    
    // constructor con valores por defecto
    Camara() : x(0), y(0), z(5.0f), rotX(0), rotY(0),
              objetivoX(0), objetivoY(0), objetivoZ(5.0f),
              velocidadMovimiento(0.2f), velocidadRotacion(0.05f),
              factorLerp(0.2f) {}
};

// clase controladora para manejar la cámara 3D
class CameraController {
public:
    // constantes para configuración de la cámara
    static const float VELOCIDAD_MOVIMIENTO;
    static const float VELOCIDAD_ROTACION;
    static const float FACTOR_LERP;
    
    // actualiza la posición y rotación de la cámara basado en entrada
    static void actualizar(Camara& camara, const EstadoEntrada& entrada, float deltaTiempo);
    
    // transforma un vértice al espacio de la cámara
    static void transformarVertice(Vertice& v, const Camara& camara);
};

#endif // CAMERA_CONTROLLER_HPP