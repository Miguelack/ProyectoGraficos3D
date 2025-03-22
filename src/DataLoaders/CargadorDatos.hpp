#ifndef CARGADORDATOS_HPP
#define CARGADORDATOS_HPP

#include <vector>
#include <string>
#include "Common/Vertice.hpp"  // Incluir la definicion de Vertice

class CargadorDatos {
public:
    // carga un modelo 3d desde un archivo de texto
    static std::vector<Vertice> cargarModeloDesdeArchivo(const std::string& rutaArchivo);
};

#endif // CARGADORDATOS_HPP