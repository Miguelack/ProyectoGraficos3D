// incluye el archivo de cabecera de la clase generadormodelos3d
#include "GeneradorModelos3D.hpp"
// incluye libreria matematica para operaciones como division
#include <cmath>
// incluye libreria para entrada/salida por consola
#include <iostream>
// incluye libreria de algoritmos para usar funciones como max
#include <algorithm>

// metodo para generar vertices de un cubo 3d
std::vector<Vertice> GeneradorModelos3D::generarCubo(float tamano) {
    // calcula la mitad del tamaño para centrar el cubo en el origen
    float halfTamano = tamano / 2.0f;
    
    // crea y retorna vector con los 8 vertices del cubo
    std::vector<Vertice> vertices = {
        // vertices de la cara frontal inferior
        {-halfTamano, -halfTamano, -halfTamano}, // vertice 0: frente-inferior-izquierda
        { halfTamano, -halfTamano, -halfTamano}, // vertice 1: frente-inferior-derecha
        { halfTamano,  halfTamano, -halfTamano}, // vertice 2: frente-superior-derecha 
        {-halfTamano,  halfTamano, -halfTamano}, // vertice 3: frente-superior-izquierda
        
        // vertices de la cara trasera inferior
        {-halfTamano, -halfTamano, halfTamano},  // vertice 4: trasera-inferior-izquierda
        { halfTamano, -halfTamano, halfTamano},  // vertice 5: trasera-inferior-derecha
        { halfTamano,  halfTamano, halfTamano},  // vertice 6: trasera-superior-derecha
        {-halfTamano,  halfTamano, halfTamano}   // vertice 7: trasera-superior-izquierda
    };
    
    return vertices;
}

// metodo para generar vertices de una piramide 3d
std::vector<Vertice> GeneradorModelos3D::generarPiramide(float base, float altura) {
    // calcula la mitad del tamaño de la base
    float halfBase = base / 2.0f;
    
    // crea y retorna vector con los 5 vertices de la piramide
    std::vector<Vertice> vertices = {
        // vertices de la base cuadrada
        {-halfBase, -halfBase, 0}, // vertice 0: esquina posterior-izquierda
        { halfBase, -halfBase, 0}, // vertice 1: esquina posterior-derecha
        { halfBase,  halfBase, 0}, // vertice 2: esquina frontal-derecha
        {-halfBase,  halfBase, 0}, // vertice 3: esquina frontal-izquierda
        
        // vertice superior (punta)
        {0, 0, altura} // vertice 4: punta de la piramide
    };
    
    return vertices;
}

// metodo para imprimir informacion sobre los vertices
void GeneradorModelos3D::imprimirVertices(const std::vector<Vertice>& vertices) {
    // muestra encabezado con numero total de vertices
    std::cout << "\n=== Vertices del Modelo ===\n";
    std::cout << "Total vertices: " << vertices.size() << "\n";
    
    // imprime coordenadas de cada vertice con su indice
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << "V" << i << ": (" 
                  << vertices[i].x << ", " 
                  << vertices[i].y << ", " 
                  << vertices[i].z << ")\n";
    }
    
    // muestra diagrama ascii del cubo si tiene 8 vertices
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
    // muestra diagrama de piramide si tiene 5 vertices
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