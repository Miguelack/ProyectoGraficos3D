#ifndef LECTORMODELOS3D_HPP
#define LECTORMODELOS3D_HPP

#include <vector>
#include <string>
#include "Common/Vertice.hpp"  // Incluir la definicion de Vertice

class LectorModelos3D {
public:
    // carga un modelo 3d desde un archivo .obj
    static std::vector<Vertice> cargarModeloOBJ(const std::string& rutaArchivo);
};

#endif // LECTORMODELOS3D_HPP