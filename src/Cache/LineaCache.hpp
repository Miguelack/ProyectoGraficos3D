#ifndef LINEACACHE_HPP
#define LINEACACHE_HPP

// inclusion para tipos de enteros de tamaño fijo
#include <cstdint>  // para uint32_t

// estructura que representa una linea de cache
struct LineaCache {
    // campo que almacena la etiqueta de direccion de memoria
    uint32_t etiqueta;
    
    // flag que indica si la linea contiene datos validos
    bool valido;
    
    // flag que indica si la linea ha sido modificada (politica write-back)
    bool modificado;
    
    // contador para politicas de reemplazo lru
    int contadorAccesos;
    
    // metodo para inicializar los valores de la linea
    void inicializar(uint32_t etiq, bool val, bool mod) {
        // asigna la etiqueta de direccion
        etiqueta = etiq;
        
        // establece el estado de validez
        valido = val;
        
        // establece el estado de modificacion
        modificado = mod;
        
        // resetea el contador de accesos
        contadorAccesos = 0;
    }
    
    // constructor por defecto que inicializa todos los valores a cero/false
    LineaCache() : etiqueta(0), valido(false), modificado(false), contadorAccesos(0) {}
};

// fin de la directiva de preprocesador ifndef
#endif // LINEACACHE_HPP