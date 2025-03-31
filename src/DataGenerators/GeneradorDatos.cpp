#include "GeneradorDatos.hpp"
#include <random>
#include <algorithm>
#include <cmath>

std::vector<int> GeneradorDatos::generarSecuenciaOptimizada(uint32_t tamanoCache, uint32_t tamanoBloque, uint32_t asociatividad) {
    std::vector<int> direcciones;
    const uint32_t numConjuntos = tamanoCache / (tamanoBloque * asociatividad);
    const uint32_t repeticionesPorBloque = 20;
    
    // Generador aleatorio mejorado
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_offset(1, tamanoBloque-1);
    
    // 1. Generar bloques base para cada conjunto
    for (uint32_t conjunto = 0; conjunto < numConjuntos; ++conjunto) {
        for (uint32_t via = 0; via < asociatividad; ++via) {
            uint32_t direccionBase = (conjunto * asociatividad + via) * tamanoBloque;
            
            // 2. Crear patrones de acceso con localidad
            for (uint32_t r = 0; r < repeticionesPorBloque; ++r) {
                direcciones.push_back(direccionBase);
                direcciones.push_back(direccionBase + dist_offset(gen));
                
                if (via > 0) direcciones.push_back(direccionBase - tamanoBloque);
                if (via < asociatividad-1) direcciones.push_back(direccionBase + tamanoBloque);
                
                if (r % 2 == 0) direcciones.push_back(direccionBase);
            }
        }
    }
    
    // 3. Mezclar los patrones pero manteniendo cierta localidad
    std::shuffle(direcciones.begin(), direcciones.end(), gen);
    
    // 4. Añadir algunos accesos aleatorios (solo 1%)
    std::uniform_int_distribution<> dist_global(0, tamanoCache * 4);
    for (size_t i = 0; i < direcciones.size()/100; ++i) {
        int addr = dist_global(gen) & ~(tamanoBloque-1);
        if (addr >= 0) {
            direcciones.push_back(addr);
        }
    }
    
    return direcciones;
}

std::vector<int> GeneradorDatos::generarSecuenciaCacheConsciente(int tamanoCache, int tamanoBloque, int asociatividad) {
    return generarSecuenciaOptimizada(static_cast<uint32_t>(tamanoCache), 
                                    static_cast<uint32_t>(tamanoBloque), 
                                    static_cast<uint32_t>(asociatividad));
}