#include "Cache.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <algorithm>  // Para std::min

Cache::Cache(int tamano, int tamanoBloque, int asociatividad) :
    tamanoCache(tamano), tamanoBloque(tamanoBloque), asociatividad(asociatividad),
    aciertos(0), fallos(0) {
    
    if (tamano <= 0 || tamanoBloque <= 0 || asociatividad <= 0) {
        throw std::invalid_argument("Parámetros deben ser positivos");
    }

    numConjuntos = tamanoCache / (tamanoBloque * asociatividad);
    if ((numConjuntos & (numConjuntos - 1)) != 0) {
        throw std::invalid_argument("Número de conjuntos debe ser potencia de 2");
    }

    cache.resize(numConjuntos, std::vector<LineaCache>(asociatividad));
    listasLRU.resize(numConjuntos);
    mapaEtiquetas.resize(numConjuntos);
}

bool Cache::manejarBloqueFrecuente(int conjunto, uint32_t etiqueta) {
    auto& mapa = mapaEtiquetas[conjunto];
    auto it = mapa.find(etiqueta);
    
    if (it != mapa.end() && it->second->contadorAccesos > 3) {
        // Bloque muy frecuente - evitar reemplazo
        auto& listaLRU = listasLRU[conjunto];
        listaLRU.erase(it->second->iteradorLRU);
        listaLRU.push_front(etiqueta);
        it->second->iteradorLRU = listaLRU.begin();
        return true;
    }
    return false;
}

void Cache::actualizarLRU(int conjunto, uint32_t etiqueta, bool esAcierto) {
    auto& listaLRU = listasLRU[conjunto];
    auto& mapa = mapaEtiquetas[conjunto];
    
    auto it = mapa.find(etiqueta);
    if (it != mapa.end()) {
        if (esAcierto) {
            it->second->contadorAccesos++;
        }
        
        // Mover al frente solo si es acceso frecuente
        if (it->second->contadorAccesos > 2) {
            listaLRU.erase(it->second->iteradorLRU);
            listaLRU.push_front(etiqueta);
            it->second->iteradorLRU = listaLRU.begin();
        }
    }
}

bool Cache::acceder(int direccion) {
    uint32_t indiceConjunto = (direccion / tamanoBloque) & (numConjuntos - 1);
    uint32_t etiqueta = direccion / (tamanoBloque * numConjuntos);

    // 1. Verificar si es un bloque frecuente que debemos proteger
    if (manejarBloqueFrecuente(indiceConjunto, etiqueta)) {
        aciertos++;
        return true;
    }

    // 2. Búsqueda normal
    auto& conjunto = cache[indiceConjunto];
    auto& listaLRU = listasLRU[indiceConjunto];
    auto& mapa = mapaEtiquetas[indiceConjunto];

    auto it = mapa.find(etiqueta);
    if (it != mapa.end()) {
        aciertos++;
        actualizarLRU(indiceConjunto, etiqueta, true);
        return true;
    }

    // 3. Manejo de fallo
    fallos++;
    LineaCache* lineaAReemplazar = nullptr;

    if (listaLRU.size() < static_cast<size_t>(asociatividad)) {
        for (auto& linea : conjunto) {
            if (!linea.valido) {
                lineaAReemplazar = &linea;
                break;
            }
        }
    } else {
        // Buscar línea con menos accesos recientes que no sea frecuente
        for (auto itLRU = listaLRU.rbegin(); itLRU != listaLRU.rend(); ++itLRU) {
            auto lineaIt = mapa.find(*itLRU);
            if (lineaIt != mapa.end() && lineaIt->second->contadorAccesos <= 2) {
                lineaAReemplazar = lineaIt->second;
                mapa.erase(lineaIt);
                break;
            }
        }
        
        // Si todas son frecuentes, reemplazar la LRU normal
        if (!lineaAReemplazar) {
            uint32_t etiquetaLRU = listaLRU.back();
            auto itLinea = mapa.find(etiquetaLRU);
            if (itLinea != mapa.end()) {
                lineaAReemplazar = itLinea->second;
                mapa.erase(itLinea);
            }
        }
    }

    if (lineaAReemplazar) {
        lineaAReemplazar->inicializar(etiqueta, true, false, listaLRU.begin());
        listaLRU.push_front(etiqueta);
        lineaAReemplazar->iteradorLRU = listaLRU.begin();
        mapa[etiqueta] = lineaAReemplazar;
    }

    return false;
}

bool Cache::accederConPrefetch(int direccion) {
    bool acierto = acceder(direccion);
    
    // Precargar bloques adyacentes de manera más inteligente
    const int prefetchDistance = 4; // Aumentar distancia de prefetch
    for (int i = 1; i <= prefetchDistance; i++) {
        int prefetchAddr = direccion + i * tamanoBloque;
        // Solo hacer prefetch si no está ya en caché
        uint32_t prefetchConjunto = (prefetchAddr / tamanoBloque) & (numConjuntos - 1);
        uint32_t prefetchTag = prefetchAddr / (tamanoBloque * numConjuntos);
        
        auto& mapa = mapaEtiquetas[prefetchConjunto];
        if (mapa.find(prefetchTag) == mapa.end()) {
            acceder(prefetchAddr);
        }
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
    std::cout << "Tamaño caché: " << tamanoCache << " bytes\n";
    std::cout << "Tamaño bloque: " << tamanoBloque << " bytes\n";
    std::cout << "Asociatividad: " << asociatividad << "-way\n";
    std::cout << "Número de conjuntos: " << numConjuntos << "\n";
}

void Cache::imprimirEstado() const {
    std::cout << "\n=== Estado de la Caché ===\n";
    for (size_t conjunto = 0; conjunto < cache.size(); ++conjunto) {
        std::cout << "Conjunto " << conjunto << ":\n";
        for (size_t via = 0; via < cache[conjunto].size(); ++via) {
            const auto& linea = cache[conjunto][via];
            std::cout << "  Via " << via << ": ";
            if (linea.valido) {
                std::cout << "Tag=" << linea.etiqueta 
                          << " (Accesos=" << linea.contadorAccesos << ")";
                if (linea.modificado) {
                    std::cout << " [M]";
                }
            } else {
                std::cout << "Inválido";
            }
            std::cout << "\n";
        }
    }
}
