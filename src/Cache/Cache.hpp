#ifndef CACHE_HPP
#define CACHE_HPP

#include <vector>
#include <list>
#include <unordered_map>
#include "LineaCache.hpp"  // Incluir la definicion de LineaCache

class Cache {
private:
    int tamanoCache;               // tamano total del cache
    int tamanoBloque;              // tamano de cada bloque
    int asociatividad;             // numero de vias (4-way)
    int numConjuntos;              // numero de conjuntos

    std::vector<std::vector<LineaCache>> cache; // estructura del cache
    std::vector<std::list<int>> listasLRU;      // listas LRU por conjunto

    std::unordered_map<int, int> etiquetaAConjunto; // mapeo de etiqueta a conjunto

    int aciertos;                  // numero de aciertos
    int fallos;                    // numero de fallos

public:
    // constructor
    Cache(int tamano, int tamanoBloque, int asociatividad);

    // metodo para acceder a una direccion en el cache
    bool acceder(int direccion);

    // metodo para imprimir estadisticas del cache
    void imprimirEstadisticas() const;
};

#endif // CACHE_HPP