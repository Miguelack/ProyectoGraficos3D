#ifndef LINEACACHE_HPP
#define LINEACACHE_HPP

#include <list>
#include <cstdint>

struct LineaCache {
    uint32_t etiqueta;
    bool valido;
    bool modificado;
    int contadorAccesos;  // Nuevo campo para seguimiento de accesos frecuentes
    std::list<uint32_t>::iterator iteradorLRU;
    
    void inicializar(uint32_t etiq, bool val, bool mod, std::list<uint32_t>::iterator it) {
        etiqueta = etiq;
        valido = val;
        modificado = mod;
        contadorAccesos = 0;  // Inicializar contador
        iteradorLRU = it;
    }
    
    LineaCache() : etiqueta(0), valido(false), modificado(false), contadorAccesos(0) {}
};

#endif