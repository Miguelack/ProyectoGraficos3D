#ifndef LINEACACHE_HPP
#define LINEACACHE_HPP

#include <list>

// estructura para representar una linea de cache
struct LineaCache {
    int etiqueta;                  // etiqueta de la linea
    bool valido;                   // indica si la linea es valida
    bool modificado;               // indica si la linea ha sido modificada
    std::list<int>::iterator iteradorLRU; // iterador para la politica LRU
};

#endif // LINEACACHE_HPP