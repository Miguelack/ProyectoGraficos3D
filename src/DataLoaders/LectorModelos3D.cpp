#include "LectorModelos3D.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

// carga un modelo 3d desde un archivo .obj
std::vector<Vertice> LectorModelos3D::cargarModeloOBJ(const std::string& rutaArchivo) {
    std::vector<Vertice> vertices;
    std::ifstream archivo(rutaArchivo);
    std::string linea;

    if (!archivo.is_open()) {
        std::cerr << "error: no se pudo abrir el archivo " << rutaArchivo << std::endl;
        return vertices;
    }

    while (std::getline(archivo, linea)) {
        std::istringstream ss(linea);
        std::string tipo;
        ss >> tipo;

        if (tipo == "v") { // solo nos interesan los vertices
            Vertice v;
            ss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
    }

    archivo.close();
    return vertices;
}