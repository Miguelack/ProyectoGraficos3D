#include "GeneradorDatos.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <cmath>

std::vector<int> GeneradorDatos::generarSecuenciaLineal(int inicio, int fin, int paso) {
    std::vector<int> secuencia;
    for (int i = inicio; i <= fin; i += paso) {
        secuencia.push_back(i);
    }
    return secuencia;
}

std::vector<int> GeneradorDatos::generarSecuenciaAleatoria(int tamano, int rangoMin, int rangoMax) {
    std::vector<int> secuencia;
    std::srand(std::time(nullptr));
    
    for (int i = 0; i < tamano; ++i) {
        secuencia.push_back(rangoMin + (std::rand() % (rangoMax - rangoMin + 1)));
    }
    
    return secuencia;
}

std::vector<int> GeneradorDatos::generarSecuenciaOptimizada(uint32_t tamanoCache, uint32_t tamanoBloque, uint32_t asociatividad) {
    std::vector<int> direcciones;
    const uint32_t numConjuntos = tamanoCache / (tamanoBloque * asociatividad);
    const uint32_t repeticionesPorBloque = 12;  // Aumentamos las repeticiones por bloque
    
    // Generador aleatorio con mejor distribución
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_offset(1, tamanoBloque-1);
    
    // 1. Generar bloques base para cada conjunto
    for (uint32_t conjunto = 0; conjunto < numConjuntos; ++conjunto) {
        for (uint32_t via = 0; via < asociatividad; ++via) {
            const uint32_t direccionBase = conjunto * (tamanoBloque * numConjuntos) + (via * tamanoBloque);
            
            // 2. Crear patrones de acceso con localidad espacial y temporal mejorada
            for (uint32_t r = 0; r < repeticionesPorBloque; ++r) {
                // Patrón 1: Acceso secuencial con diferentes offsets
                direcciones.push_back(direccionBase);
                direcciones.push_back(direccionBase + dist_offset(gen));
                
                // Patrón 2: Acceso a bloques vecinos en el mismo conjunto
                if (via > 0) {
                    direcciones.push_back(direccionBase - tamanoBloque);
                }
                if (via < asociatividad - 1) {
                    direcciones.push_back(direccionBase + tamanoBloque);
                }
                
                // Patrón 3: Acceso con stride predecible
                if (r % 3 == 0) {
                    direcciones.push_back(direccionBase + (tamanoBloque * 2));
                }
            }
        }
    }
    
    // 3. Mezclar los patrones pero manteniendo cierta localidad temporal
    std::shuffle(direcciones.begin(), direcciones.end(), gen);
    
    // 4. Añadir más localidad temporal repitiendo accesos recientes
    const size_t tamanoInicial = direcciones.size();
    for (size_t i = 0; i < tamanoInicial / 3; i++) {
        size_t idx = i % tamanoInicial;
        direcciones.push_back(direcciones[idx]);
    }
    
    // 5. Añadir algunos accesos aleatorios (solo 3%) para evitar patrones perfectos
    std::uniform_int_distribution<> dist_global(0, tamanoCache * 4);
    for (size_t i = 0; i < direcciones.size()/30; ++i) {
        direcciones.push_back(dist_global(gen) & ~(tamanoBloque-1)); // Alineado a bloques
    }
    
    return direcciones;
}

std::vector<int> GeneradorDatos::generarSecuenciaCacheConsciente(int tamanoCache, int tamanoBloque, int asociatividad) {
    return generarSecuenciaOptimizada(static_cast<uint32_t>(tamanoCache), 
                                    static_cast<uint32_t>(tamanoBloque), 
                                    static_cast<uint32_t>(asociatividad));
}