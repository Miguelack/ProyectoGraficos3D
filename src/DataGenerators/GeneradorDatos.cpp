// incluye el archivo de cabecera de la clase generadordatos
#include "GeneradorDatos.hpp"
// incluye libreria para generacion de numeros aleatorios
#include <random>
// incluye libreria con algoritmos como shuffle
#include <algorithm>
// incluye libreria matematica para operaciones avanzadas
#include <cmath>

// implementacion del metodo para generar secuencia optimizada para cache
std::vector<int> GeneradorDatos::generarSecuenciaOptimizada(uint32_t tamanoCache, uint32_t tamanoBloque, uint32_t asociatividad) {
    // vector que almacenara las direcciones generadas
    std::vector<int> direcciones;
    // calcula el numero de conjuntos en la cache
    const uint32_t numConjuntos = tamanoCache / (tamanoBloque * asociatividad);
    // define cuantas repeticiones se haran por cada bloque
    const uint32_t repeticionesPorBloque = 20;
    
    // configura generador de numeros aleatorios de mejor calidad
    std::random_device rd;
    std::mt19937 gen(rd());
    // distribucion para offsets dentro de un bloque
    std::uniform_int_distribution<> dist_offset(1, tamanoBloque-1);
    
    // 1. genera bloques base para cada conjunto
    for (uint32_t conjunto = 0; conjunto < numConjuntos; ++conjunto) {
        for (uint32_t via = 0; via < asociatividad; ++via) {
            // calcula direccion base para este bloque
            uint32_t direccionBase = (conjunto * asociatividad + via) * tamanoBloque;
            
            // 2. crea patrones de acceso con localidad espacial
            for (uint32_t r = 0; r < repeticionesPorBloque; ++r) {
                // accede a la direccion base
                direcciones.push_back(direccionBase);
                // accede a offset aleatorio dentro del mismo bloque
                direcciones.push_back(direccionBase + dist_offset(gen));
                
                // accede a bloques adyacentes (si existen)
                if (via > 0) direcciones.push_back(direccionBase - tamanoBloque);
                if (via < asociatividad-1) direcciones.push_back(direccionBase + tamanoBloque);
                
                // repite acceso a la base cada 2 iteraciones
                if (r % 2 == 0) direcciones.push_back(direccionBase);
            }
        }
    }
    
    // 3. mezcla los accesos pero mantiene cierta localidad
    std::shuffle(direcciones.begin(), direcciones.end(), gen);
    
    // 4. añade un pequeño porcentaje de accesos aleatorios (1%)
    std::uniform_int_distribution<> dist_global(0, tamanoCache * 4);
    for (size_t i = 0; i < direcciones.size()/100; ++i) {
        // genera direccion aleatoria alineada a bloque
        int addr = dist_global(gen) & ~(tamanoBloque-1);
        if (addr >= 0) {
            direcciones.push_back(addr);
        }
    }
    
    // devuelve la secuencia generada
    return direcciones;
}

// implementacion de la version anterior (compatibilidad)
std::vector<int> GeneradorDatos::generarSecuenciaCacheConsciente(int tamanoCache, int tamanoBloque, int asociatividad) {
    // simplemente llama a la nueva version con casteo de tipos
    return generarSecuenciaOptimizada(static_cast<uint32_t>(tamanoCache), 
                                    static_cast<uint32_t>(tamanoBloque), 
                                    static_cast<uint32_t>(asociatividad));
}