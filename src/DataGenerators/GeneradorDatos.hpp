#ifndef GENERADORDATOS_HPP
#define GENERADORDATOS_HPP

#include <vector>

class GeneradorDatos {
public:
    // genera una secuencia lineal de direcciones
    static std::vector<int> generarSecuenciaLineal(int inicio, int fin, int paso);

    // genera una secuencia aleatoria de direcciones
    static std::vector<int> generarSecuenciaAleatoria(int tamano, int rangoMin, int rangoMax);
};

#endif // GENERADORDATOS_HPP