#include "CargadorDatos.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

// carga un modelo 3d desde un archivo de texto
std::vector<Vertice> CargadorDatos::cargarModeloDesdeArchivo(const std::string& rutaArchivo) {
    std::vector<Vertice> vertices;
    std::ifstream archivo(rutaArchivo);
    std::string linea;

    if (!archivo.is_open()) {
        std::cerr << "error: no se pudo abrir el archivo " << rutaArchivo << std::endl;
        return vertices;
    }

    while (std::getline(archivo, linea)) {
        std::istringstream ss(linea);
        Vertice v;
        ss >> v.x >> v.y >> v.z;
        vertices.push_back(v);
    }

    archivo.close();
    return vertices;
}