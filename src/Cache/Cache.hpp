#ifndef CACHE_HPP
#define CACHE_HPP

#include <vector>
#include <list>
#include <unordered_map>
#include "LineaCache.hpp"

class Cache {
private:
    int tamanoCache;
    int tamanoBloque;
    int asociatividad;
    int numConjuntos;

    std::vector<std::vector<LineaCache>> cache;
    std::vector<std::list<uint32_t>> listasLRU;
    std::vector<std::unordered_map<uint32_t, LineaCache*>> mapaEtiquetas;

    int aciertos;
    int fallos;

    // Nuevos métodos privados para optimización
    bool manejarBloqueFrecuente(int conjunto, uint32_t etiqueta);
    void actualizarLRU(int conjunto, uint32_t etiqueta, bool esAcierto);

public:
    Cache(int tamano, int tamanoBloque, int asociatividad);
    bool acceder(int direccion);
    bool accederConPrefetch(int direccion);  // Nuevo método
    void imprimirEstadisticas() const;
    void imprimirEstado() const;
    int calcularNumConjuntos() const { return numConjuntos; }
};

#endif