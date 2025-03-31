#include "GeneradorModelos3D.hpp"
#include <cmath>
#include <iostream>
#include <algorithm> // Para std::max

std::vector<Vertice> GeneradorModelos3D::generarCubo(float tamano) {
    float halfTamano = tamano / 2.0f;
    
    // Definición clara de los 8 vértices con nombres descriptivos
    std::vector<Vertice> vertices = {
        // Cara frontal inferior
        {-halfTamano, -halfTamano, -halfTamano}, // V0: Frente-inferior-izquierda
        { halfTamano, -halfTamano, -halfTamano}, // V1: Frente-inferior-derecha
        { halfTamano,  halfTamano, -halfTamano}, // V2: Frente-superior-derecha 
        {-halfTamano,  halfTamano, -halfTamano}, // V3: Frente-superior-izquierda
        
        // Cara trasera inferior
        {-halfTamano, -halfTamano, halfTamano},  // V4: Trasera-inferior-izquierda
        { halfTamano, -halfTamano, halfTamano},  // V5: Trasera-inferior-derecha
        { halfTamano,  halfTamano, halfTamano},  // V6: Trasera-superior-derecha
        {-halfTamano,  halfTamano, halfTamano}   // V7: Trasera-superior-izquierda
    };
    
    return vertices;
}

std::vector<Vertice> GeneradorModelos3D::generarPiramide(float base, float altura) {
    float halfBase = base / 2.0f;
    
    std::vector<Vertice> vertices = {
        // Base cuadrada
        {-halfBase, -halfBase, 0}, // V0: Esquina posterior-izquierda
        { halfBase, -halfBase, 0}, // V1: Esquina posterior-derecha
        { halfBase,  halfBase, 0}, // V2: Esquina frontal-derecha
        {-halfBase,  halfBase, 0}, // V3: Esquina frontal-izquierda
        
        // Vértice superior
        {0, 0, altura} // V4: Punta de la pirámide
    };
    
    return vertices;
}

void GeneradorModelos3D::imprimirVertices(const std::vector<Vertice>& vertices) {
    std::cout << "\n=== Vértices del Modelo ===\n";
    std::cout << "Total vértices: " << vertices.size() << "\n";
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << "V" << i << ": (" 
                  << vertices[i].x << ", " 
                  << vertices[i].y << ", " 
                  << vertices[i].z << ")\n";
    }
    
    // Mostrar diagrama ASCII del cubo si tiene 8 vértices
    if (vertices.size() == 8) {
        std::cout << "\nDiagrama 3D:\n";
        std::cout << "    V7-------V6\n";
        std::cout << "   /|       /|\n";
        std::cout << "  V3-------V2|\n";
        std::cout << "  | |      | |\n"; 
        std::cout << "  |V4------|V5\n";
        std::cout << "  |/       |/\n";
        std::cout << "  V0-------V1\n";
    }
    // Mostrar diagrama de pirámide si tiene 5 vértices
    else if (vertices.size() == 5) {
        std::cout << "\nDiagrama 3D:\n";
        std::cout << "      V4\n";
        std::cout << "     /|\\\n";
        std::cout << "    / | \\\n";
        std::cout << " V0/__|__\\V1\n";
        std::cout << "   \\  |  /\n";
        std::cout << "    \\ | /\n";
        std::cout << "     \\|/\n";
        std::cout << "      V2\n";
    }
}