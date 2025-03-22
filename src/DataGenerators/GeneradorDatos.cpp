#include "GeneradorDatos.hpp"
#include <cstdlib>
#include <ctime>

// genera una secuencia lineal de direcciones
std::vector<int> GeneradorDatos::generarSecuenciaLineal(int inicio, int fin, int paso) {
    std::vector<int> secuencia;
    for (int i = inicio; i <= fin; i += paso) {
        secuencia.push_back(i);
    }
    return secuencia;
}

// genera una secuencia aleatoria de direcciones
std::vector<int> GeneradorDatos::generarSecuenciaAleatoria(int tamano, int rangoMin, int rangoMax) {
    std::vector<int> secuencia;
    std::srand(std::time(nullptr)); // inicializar la semilla aleatoria

    for (int i = 0; i < tamano; ++i) {
        int valor = rangoMin + (std::rand() % (rangoMax - rangoMin + 1));
        secuencia.push_back(valor);
    }
    return secuencia;
}