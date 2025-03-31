#ifndef GENERADORDATOS_HPP
#define GENERADORDATOS_HPP

#include <vector>
#include <cstdint>  // Para uint32_t

class GeneradorDatos {
public:
    // Métodos existentes
    static std::vector<int> generarSecuenciaLineal(int inicio, int fin, int paso);
    static std::vector<int> generarSecuenciaAleatoria(int tamano, int rangoMin, int rangoMax);
    
    // Método optimizado para alta tasa de aciertos
    static std::vector<int> generarSecuenciaOptimizada(uint32_t tamanoCache, uint32_t tamanoBloque, uint32_t asociatividad);
    
    // Versión anterior (puede eliminarse)
    static std::vector<int> generarSecuenciaCacheConsciente(int tamanoCache, int tamanoBloque, int asociatividad);
};

#endif // GENERADORDATOS_HPP