#include "funcion_hash.h"

size_t funcion_hash(const std::string& clave) {
    size_t hash = 0;
 
    for (size_t i = 0; i < clave.size(); i ++){  // Definimos que la variable "i" sea un entero sin signo para que no entre en conflicto
                                                 // con la comparación.
                                                 
        hash = hash * 23 + clave[i]; // arranca en el valor de la letra segun ASCII
    }

    return hash;
}

size_t funcion_hash(int clave) {
    size_t hash = static_cast<size_t>(clave); // Le saco el signo
    hash = (hash ^ (hash >> 16)) * 2654435761u; // combino mediante un XOR y desplazo los bits 16 a la derecha y lugo multiplico por cte
    return hash;
}
// 2654435761 es el numero y la u es de unsigned