// directiva para evitar inclusiones multiples del archivo
#ifndef GENERADORDATOS_HPP
// define el identificador unico para este header
#define GENERADORDATOS_HPP

// incluye la libreria para usar contenedores vector
#include <vector>
// incluye definiciones de tipos enteros de tamaño fijo
#include <cstdint>  // para uint32_t

// clase para generar diferentes tipos de secuencias de datos
class GeneradorDatos {
public:
    // metodo estatico que genera una secuencia lineal de numeros
    static std::vector<int> generarSecuenciaLineal(int inicio, int fin, int paso);
    
    // metodo estatico que genera una secuencia de numeros aleatorios
    static std::vector<int> generarSecuenciaAleatoria(int tamano, int rangoMin, int rangoMax);
    
    // version optimizada para generar secuencias con alta tasa de aciertos en cache
    // utiliza parametros especificos de la cache (tamaño, bloque, asociatividad)
    static std::vector<int> generarSecuenciaOptimizada(uint32_t tamanoCache, uint32_t tamanoBloque, uint32_t asociatividad);
    
    // version anterior del generador consciente de cache (marcada como obsoleta)
    static std::vector<int> generarSecuenciaCacheConsciente(int tamanoCache, int tamanoBloque, int asociatividad);
};

// fin de la directiva de preprocesador ifndef
#endif // GENERADORDATOS_HPP