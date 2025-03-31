#ifndef CACHE_HPP
#define CACHE_HPP

#include <vector>
#include <cstddef>
#include <cstdint>
#include "LineaCache.hpp"

class Cache {
private:
    int tamanoCache;
    int tamanoBloque;
    int asociatividad;
    int numConjuntos;

    std::vector<std::vector<LineaCache>> cache;
    std::vector<int> aciertosPorConjunto;
    std::vector<int> fallosPorConjunto;

    int aciertos;
    int fallos;

    void actualizarComoMRU(int conjunto, int via);
    std::size_t encontrarLRU(int conjunto) const;

public:
    Cache(int tamano, int tamanoBloque, int asociatividad);
    
    // Métodos principales
    bool acceder(int direccion);
    bool accederConPrefetch(int direccion);
    
    // Métodos de visualización
    void imprimirEstadisticas() const;
    void imprimirEstado() const;
    
    // Métodos de consulta
    int calcularNumConjuntos() const { return numConjuntos; }
    int getAciertos() const { return aciertos; }
    int getFallos() const { return fallos; }
    double getTasaAciertos() const {
        return (aciertos + fallos) > 0 ? (aciertos * 100.0) / (aciertos + fallos) : 0.0;
    }
    
    // Validación
    bool direccionValida(int direccion) const {
        return direccion >= 0 && (direccion / tamanoBloque) < (numConjuntos * asociatividad);
    }
};

#endif