#include "Cache.hpp"
#include <iostream>

// constructor
Cache::Cache(int tamano, int tamanoBloque, int asociatividad)
    : tamanoCache(tamano), tamanoBloque(tamanoBloque), asociatividad(asociatividad),
      aciertos(0), fallos(0) {
    // calcular el numero de conjuntos
    numConjuntos = tamanoCache / (tamanoBloque * asociatividad);

    // inicializar la estructura del cache
    cache.resize(numConjuntos, std::vector<LineaCache>(asociatividad, {0, false, false}));

    // inicializar las listas LRU
    listasLRU.resize(numConjuntos);
}

// metodo para acceder a una direccion en el cache
bool Cache::acceder(int direccion) {
    // calcular el indice de conjunto y etiqueta
    int indiceConjunto = (direccion / tamanoBloque) % numConjuntos;
    int etiqueta = direccion / (tamanoBloque * numConjuntos);

    // obtener el conjunto y la lista LRU correspondiente
    auto& lineas = cache[indiceConjunto];
    auto& listaLRU = listasLRU[indiceConjunto];

    for (auto& linea : lineas) {
        if (linea.valido && linea.etiqueta == etiqueta) {
            // acierto: actualizar la lista LRU
            aciertos++;
            listaLRU.erase(linea.iteradorLRU);
            listaLRU.push_front(etiqueta);
            linea.iteradorLRU = listaLRU.begin();
            return true; // acierto
        }
    }

    fallos++;

    if (listaLRU.empty()) {
     
        return false;
    }

    int etiquetaLRU = listaLRU.back();  
    listaLRU.pop_back();

    for (auto& linea : lineas) {
        if (linea.etiqueta == etiquetaLRU) {
 
            linea.etiqueta = etiqueta;
            linea.valido = true;
            listaLRU.push_front(etiqueta);
            linea.iteradorLRU = listaLRU.begin();
            break;
        }
    }

    return false; // fallo
}

// metodo para imprimir estadisticas del cache
void Cache::imprimirEstadisticas() const {
    std::cout << "aciertos: " << aciertos << ", fallos: " << fallos << std::endl;
}