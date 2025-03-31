#include "Cache.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cstdint>

Cache::Cache(int tamano, int tamanoBloque, int asociatividad) :
    tamanoCache(tamano), tamanoBloque(tamanoBloque), asociatividad(asociatividad),
    aciertos(0), fallos(0) {
    
    if (tamano <= 0 || tamanoBloque <= 0 || asociatividad <= 0) {
        throw std::invalid_argument("Parámetros deben ser positivos");
    }

    numConjuntos = tamanoCache / (tamanoBloque * asociatividad);
    if (numConjuntos == 0) {
        throw std::invalid_argument("Número de conjuntos no puede ser cero");
    }

    cache.resize(numConjuntos, std::vector<LineaCache>(asociatividad));
    aciertosPorConjunto.resize(numConjuntos, 0);
    fallosPorConjunto.resize(numConjuntos, 0);
}

void Cache::actualizarComoMRU(int conjunto, int via) {
    if (conjunto < 0 || conjunto >= numConjuntos || via < 0 || via >= asociatividad) return;
    
    for (auto& linea : cache[conjunto]) {
        if (linea.valido && linea.contadorAccesos > 0) {
            linea.contadorAccesos--;
        }
    }
    cache[conjunto][via].contadorAccesos = asociatividad;
}

std::size_t Cache::encontrarLRU(int conjunto) const {
    if (conjunto < 0 || conjunto >= numConjuntos) return 0;
    
    std::size_t viaLRU = 0;
    int minAccesos = cache[conjunto][0].contadorAccesos;
    
    for (std::size_t via = 1; via < cache[conjunto].size(); ++via) {
        if (cache[conjunto][via].contadorAccesos < minAccesos) {
            minAccesos = cache[conjunto][via].contadorAccesos;
            viaLRU = via;
        }
    }
    return viaLRU;
}

bool Cache::acceder(int direccion) {
    if (direccion < 0) return false;
    
    uint32_t conjunto = (direccion / tamanoBloque) & (numConjuntos - 1);
    if (conjunto >= cache.size()) return false;
    
    uint32_t etiqueta = direccion / (tamanoBloque * numConjuntos);
    
    for (std::size_t via = 0; via < cache[conjunto].size(); ++via) {
        if (cache[conjunto][via].valido && cache[conjunto][via].etiqueta == etiqueta) {
            cache[conjunto][via].contadorAccesos++;
            actualizarComoMRU(conjunto, via);
            aciertos++;
            aciertosPorConjunto[conjunto]++;
            return true;
        }
    }
    
    std::size_t viaVictima = encontrarLRU(conjunto);
    if (viaVictima >= cache[conjunto].size()) viaVictima = 0;
    
    cache[conjunto][viaVictima].inicializar(etiqueta, true, false);
    actualizarComoMRU(conjunto, viaVictima);
    fallos++;
    fallosPorConjunto[conjunto]++;
    return false;
}

bool Cache::accederConPrefetch(int direccion) {
    bool acierto = acceder(direccion);
    
    int stride = tamanoBloque * 2;
    int prefetchAddr = direccion + stride;
    if (prefetchAddr >= 0 && 
        (prefetchAddr / (tamanoBloque * numConjuntos)) == (direccion / (tamanoBloque * numConjuntos))) {
        acceder(prefetchAddr);
    }
    
    return acierto;
}

void Cache::imprimirEstadisticas() const {
    std::cout << "\n=== Estadísticas de Caché ===\n";
    std::cout << "Total accesos: " << (aciertos + fallos) << "\n";
    std::cout << "Aciertos: " << aciertos << " (" 
              << (aciertos * 100.0 / (aciertos + fallos)) << "%)\n";
    std::cout << "Fallos: " << fallos << " (" 
              << (fallos * 100.0 / (aciertos + fallos)) << "%)\n";
    
    for (int c = 0; c < numConjuntos; ++c) {
        if (aciertosPorConjunto[c] + fallosPorConjunto[c] > 0) {
            std::cout << "Conjunto " << c << ": "
                      << (aciertosPorConjunto[c] * 100.0 / (aciertosPorConjunto[c] + fallosPorConjunto[c]))
                      << "% aciertos\n";
        }
    }
}

void Cache::imprimirEstado() const {
    std::cout << "\n=== Estado de la Caché ===\n";
    for (int conjunto = 0; conjunto < numConjuntos; ++conjunto) {
        std::cout << "Conjunto " << conjunto << ":\n";
        for (std::size_t via = 0; via < cache[conjunto].size(); ++via) {
            const auto& linea = cache[conjunto][via];
            std::cout << "  Via " << via << ": ";
            if (linea.valido) {
                std::cout << "Tag=" << linea.etiqueta 
                          << " (Accesos=" << linea.contadorAccesos << ")";
            } else {
                std::cout << "Inválido";
            }
            std::cout << "\n";
        }
    }
}