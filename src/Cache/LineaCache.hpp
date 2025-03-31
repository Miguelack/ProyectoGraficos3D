#ifndef LINEACACHE_HPP
#define LINEACACHE_HPP

#include <cstdint>  // Para uint32_t

struct LineaCache {
    uint32_t etiqueta;
    bool valido;
    bool modificado;
    int contadorAccesos;
    
    void inicializar(uint32_t etiq, bool val, bool mod) {
        etiqueta = etiq;
        valido = val;
        modificado = mod;
        contadorAccesos = 0;
    }
    
    LineaCache() : etiqueta(0), valido(false), modificado(false), contadorAccesos(0) {}
};

#endif // LINEACACHE_HPP