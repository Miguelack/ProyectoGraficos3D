// directiva para evitar inclusiones multiples
#ifndef CACHE_HPP
#define CACHE_HPP

// inclusion para usar contenedor vector
#include <vector>
// inclusion para tipos size_t y ptrdiff_t
#include <cstddef>
// inclusion para tipos enteros de tamaño fijo
#include <cstdint>
// inclusion del archivo con la definicion de LineaCache
#include "LineaCache.hpp"

// clase que representa una memoria cache
class Cache {
private:
    // tamaño total de la cache en bytes
    int tamanoCache;
    // tamaño de cada bloque de cache en bytes
    int tamanoBloque;
    // nivel de asociatividad (1=directo, n=n-way)
    int asociatividad;
    // numero de conjuntos en la cache
    int numConjuntos;

    // estructura principal que almacena las lineas de cache
    // vector de conjuntos, cada conjunto es un vector de lineas
    std::vector<std::vector<LineaCache>> cache;
    // contador de aciertos por cada conjunto
    std::vector<int> aciertosPorConjunto;
    // contador de fallos por cada conjunto
    std::vector<int> fallosPorConjunto;

    // contador total de aciertos
    int aciertos;
    // contador total de fallos
    int fallos;

    // metodo para marcar una linea como mas recientemente usada (mru)
    void actualizarComoMRU(int conjunto, int via);
    // metodo para encontrar la linea menos recientemente usada (lru) en un conjunto
    std::size_t encontrarLRU(int conjunto) const;

public:
    // constructor principal que recibe parametros de configuracion
    Cache(int tamano, int tamanoBloque, int asociatividad);
    
    // metodos principales
    // simula un acceso a la direccion de memoria, devuelve true si fue acierto
    bool acceder(int direccion);
    // version con prefetch del acceso a memoria
    bool accederConPrefetch(int direccion);
    
    // metodos de visualizacion
    // muestra estadisticas de aciertos/fallos
    void imprimirEstadisticas() const;
    // muestra el estado actual de la cache
    void imprimirEstado() const;
    
    // metodos de consulta
    // devuelve el numero de conjuntos en la cache
    int calcularNumConjuntos() const { return numConjuntos; }
    // obtiene el contador total de aciertos
    int getAciertos() const { return aciertos; }
    // obtiene el contador total de fallos
    int getFallos() const { return fallos; }
    // calcula el porcentaje de aciertos
    double getTasaAciertos() const {
        return (aciertos + fallos) > 0 ? (aciertos * 100.0) / (aciertos + fallos) : 0.0;
    }
    
    // validacion
    // verifica si una direccion esta dentro del rango manejado por la cache
    bool direccionValida(int direccion) const {
        return direccion >= 0 && (direccion / tamanoBloque) < (numConjuntos * asociatividad);
    }
};

// fin de la directiva de preprocesador ifndef
#endif