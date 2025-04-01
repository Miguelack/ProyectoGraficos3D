// incluye la definicion de la clase Cache
#include "Cache.hpp"
// para entrada/salida por consola
#include <iostream>
// para manejo de excepciones
#include <stdexcept>
// para usar funciones como max/min
#include <algorithm>
// para tipos enteros de tamaño fijo
#include <cstdint>

// constructor de la clase Cache
Cache::Cache(int tamano, int tamanoBloque, int asociatividad) :
    // inicializa tamaño total de cache
    tamanoCache(tamano), 
    // inicializa tamaño de bloque
    tamanoBloque(tamanoBloque), 
    // inicializa nivel de asociatividad
    asociatividad(asociatividad),
    // inicializa contadores a cero
    aciertos(0), fallos(0) {
    
    // valida que los parametros sean positivos
    if (tamano <= 0 || tamanoBloque <= 0 || asociatividad <= 0) {
        throw std::invalid_argument("Parámetros deben ser positivos");
    }

    // calcula el numero de conjuntos (total bloques / asociatividad)
    numConjuntos = tamanoCache / (tamanoBloque * asociatividad);
    // verifica que haya al menos un conjunto
    if (numConjuntos == 0) {
        throw std::invalid_argument("Número de conjuntos no puede ser cero");
    }

    // redimensiona la estructura de cache
    cache.resize(numConjuntos, std::vector<LineaCache>(asociatividad));
    // inicializa contadores de aciertos por conjunto
    aciertosPorConjunto.resize(numConjuntos, 0);
    // inicializa contadores de fallos por conjunto
    fallosPorConjunto.resize(numConjuntos, 0);
}

// actualiza una linea como mas recientemente usada (mru)
void Cache::actualizarComoMRU(int conjunto, int via) {
    // verifica indices validos
    if (conjunto < 0 || conjunto >= numConjuntos || via < 0 || via >= asociatividad) return;
    
    // decrementa contadores de todas las lineas validas
    for (auto& linea : cache[conjunto]) {
        if (linea.valido && linea.contadorAccesos > 0) {
            linea.contadorAccesos--;
        }
    }
    // marca la linea actual como mru
    cache[conjunto][via].contadorAccesos = asociatividad;
}

// encuentra la linea menos recientemente usada en un conjunto
std::size_t Cache::encontrarLRU(int conjunto) const {
    // verifica indice valido
    if (conjunto < 0 || conjunto >= numConjuntos) return 0;
    
    // inicializa con la primera via
    std::size_t viaLRU = 0;
    int minAccesos = cache[conjunto][0].contadorAccesos;
    
    // busca la via con menor contador de accesos
    for (std::size_t via = 1; via < cache[conjunto].size(); ++via) {
        if (cache[conjunto][via].contadorAccesos < minAccesos) {
            minAccesos = cache[conjunto][via].contadorAccesos;
            viaLRU = via;
        }
    }
    return viaLRU;
}

// simula un acceso a memoria
bool Cache::acceder(int direccion) {
    // verifica direccion valida
    if (direccion < 0) return false;
    
    // calcula conjunto mediante hash simple
    uint32_t conjunto = (direccion / tamanoBloque) & (numConjuntos - 1);
    // verifica conjunto valido
    if (conjunto >= cache.size()) return false;
    
    // calcula etiqueta de direccion
    uint32_t etiqueta = direccion / (tamanoBloque * numConjuntos);
    
    // busca la etiqueta en todas las vias del conjunto
    for (std::size_t via = 0; via < cache[conjunto].size(); ++via) {
        if (cache[conjunto][via].valido && cache[conjunto][via].etiqueta == etiqueta) {
            // actualiza contador y marca como mru
            cache[conjunto][via].contadorAccesos++;
            actualizarComoMRU(conjunto, via);
            // incrementa contadores
            aciertos++;
            aciertosPorConjunto[conjunto]++;
            return true;
        }
    }
    
    // si no encontro, es fallo - busca victima lru
    std::size_t viaVictima = encontrarLRU(conjunto);
    if (viaVictima >= cache[conjunto].size()) viaVictima = 0;
    
    // reemplaza la linea victima
    cache[conjunto][viaVictima].inicializar(etiqueta, true, false);
    actualizarComoMRU(conjunto, viaVictima);
    // incrementa contadores de fallos
    fallos++;
    fallosPorConjunto[conjunto]++;
    return false;
}

// version con prefetching de acceder
bool Cache::accederConPrefetch(int direccion) {
    // realiza acceso normal
    bool acierto = acceder(direccion);
    
    // calcula direccion para prefetch (2 bloques adelante)
    int stride = tamanoBloque * 2;
    int prefetchAddr = direccion + stride;
    // verifica que prefetch sea valido y este en mismo conjunto
    if (prefetchAddr >= 0 && 
        (prefetchAddr / (tamanoBloque * numConjuntos)) == (direccion / (tamanoBloque * numConjuntos))) {
        acceder(prefetchAddr);
    }
    
    return acierto;
}

// muestra estadisticas de la cache
void Cache::imprimirEstadisticas() const {
    std::cout << "\n=== Estadísticas de Caché ===\n";
    std::cout << "Total accesos: " << (aciertos + fallos) << "\n";
    std::cout << "Aciertos: " << aciertos << " (" 
              << (aciertos * 100.0 / (aciertos + fallos)) << "%)\n";
    std::cout << "Fallos: " << fallos << " (" 
              << (fallos * 100.0 / (aciertos + fallos)) << "%)\n";
    
    // muestra estadisticas por conjunto
    for (int c = 0; c < numConjuntos; ++c) {
        if (aciertosPorConjunto[c] + fallosPorConjunto[c] > 0) {
            std::cout << "Conjunto " << c << ": "
                      << (aciertosPorConjunto[c] * 100.0 / (aciertosPorConjunto[c] + fallosPorConjunto[c]))
                      << "% aciertos\n";
        }
    }
}

// muestra estado actual de la cache
void Cache::imprimirEstado() const {
    std::cout << "\n=== Estado de la Caché ===\n";
    // recorre todos los conjuntos
    for (int conjunto = 0; conjunto < numConjuntos; ++conjunto) {
        std::cout << "Conjunto " << conjunto << ":\n";
        // recorre todas las vias de cada conjunto
        for (std::size_t via = 0; via < cache[conjunto].size(); ++via) {
            const auto& linea = cache[conjunto][via];
            std::cout << "  Via " << via << ": ";
            if (linea.valido) {
                // muestra info de linea valida
                std::cout << "Tag=" << linea.etiqueta 
                          << " (Accesos=" << linea.contadorAccesos << ")";
            } else {
                std::cout << "Inválido";
            }
            std::cout << "\n";
        }
    }
}