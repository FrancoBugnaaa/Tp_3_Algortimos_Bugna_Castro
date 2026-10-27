#ifndef TABLA_HASH_H
#define TABLA_HASH_H

#include <cstddef>

#include "funcion_hash.h"

// TAD TablaHash<K, V>: tabla de hash con direccionamiento cerrado.
//
// Guarda pares <clave, valor> repartidos en buckets: el par de una clave esta
// en el bucket numero funcion_hash(clave) % capacidad().
//
// Requisitos sobre los tipos:
//   - K: se puede copiar, comparar con == y existe funcion_hash(K).
//   - V: se puede copiar.
//
// Invariante de representacion:
// c >= 1.
// buckets apunta a un arreglo de c punteros a Nodo.
// Cada bucket contiene una lista enlazada sin ciclos.
// Cada clave se encuentra en el bucket
// funcion_hash(clave) % c.
// No hay claves repetidas en la tabla.
// n representa la cantidad total de nodos almacenados.
// n <= FACTOR_DE_CARGA_MAXIMO * c.
//
// Complejidades: n es la cantidad de claves guardadas y c la capacidad.
// "Promedio" supone que la funcion de hash reparte las claves de manera
// uniforme y que calcularla (y comparar dos claves) cuesta O(1).
template <typename K, typename V>
class TablaHash {
  private:
    // COMPLETAR: la representacion la eligen ustedes (que guardan, y que hay
    // en cada bucket). Pueden agregar tambien funciones auxiliares privadas.
    // Lo que el Iterador necesite de aca tiene que estar declarado antes que el.
    struct Nodo{
        K clave;
        V valor;
        Nodo* sig;
    };

    Nodo** buckets;
    size_t n;
    size_t c;

  public:
    static const size_t FACTOR_DE_CARGA_MAXIMO = 2;

    // Pre:  capacidad_inicial >= 1.
    // Post: crea una tabla vacia, con capacidad_inicial buckets.
    // Costo: O(capacidad_inicial).
    TablaHash(size_t capacidad_inicial);

    // Post: libera toda la memoria pedida por la tabla.
    // Costo: O(n + c) en peor caso.
    ~TablaHash();

    // Copiar una tabla copiaria los punteros y no su contenido: lo prohibimos.
    TablaHash(const TablaHash&) = delete;
    TablaHash& operator=(const TablaHash&) = delete;

    // Pre:  la clave NO esta en la tabla.
    // Post: la tabla tiene ademas el par <clave, valor>, y
    //       cantidad() <= FACTOR_DE_CARGA_MAXIMO * capacidad().
    // Costo: O(1) amortizado (en peor caso O(n), cuando toca redimensionar).
    void insertar(const K& clave, const V& valor);

    // Pre:  la clave esta en la tabla.
    // Post: la tabla ya no tiene la clave (ni su valor).
    // Costo: O(1) en promedio, O(n) en peor caso.
    void eliminar(const K& clave);

    // Pre:  -
    // Post: si la clave esta, devuelve un puntero a su valor (que se puede
    //       usar para leerlo o modificarlo); si no, devuelve nullptr.
    //       El puntero deja de ser valido al insertar o eliminar.
    // Costo: O(1) en promedio, O(n) en peor caso.
    V* buscar(const K& clave);

    // Post: devuelve la cantidad de claves guardadas. Costo: O(1).
    size_t cantidad();

    // Post: devuelve la cantidad de buckets. Costo: O(1).
    size_t capacidad();

    // Post: devuelve la cantidad de claves que comparten bucket con alguna
    //       clave que llego antes, es decir:
    //       cantidad() - (cantidad de buckets que no estan vacios).
    // Costo: O(c) en peor caso.
    size_t colisiones();

    // Iterador de la tabla: recorre todos los pares <clave, valor>, cada uno
    // exactamente una vez, en algun orden (ver TAD Iterador en el enunciado).
    // Deja de ser valido si se inserta o se elimina en la tabla.
    //
    // Invariante de representacion:
    // tabla apunta a la TablaHash que se esta recorriendo.
    // pos indica el bucket donde se encuentra actual.
    // Si actual != nullptr, apunta a un nodo valido de buckets[pos].
    // Si actual == nullptr, el iterador llego al final y pos == tabla->c.
    // Cada nodo se recorre una sola vez.
    class Iterador {
      private:
       TablaHash<K,V>* tabla;
       size_t pos;
       Nodo* act;
        // Para que crear_iterador() pueda armar el iterador.
        friend class TablaHash;

      public:
        // Pre:  -
        // Post: indica si quedan pares por recorrer.
        // Costo: O(1).
        bool hay_mas() {
            return act != nullptr;
        }

        // Pre:  hay_mas().
        // Post: devuelve la clave del par actual, sin modificar el iterador.
        // Costo: O(1).
        K clave() {
            return act->clave;
        }

        // Pre:  hay_mas().
        // Post: devuelve el valor del par actual, sin modificar el iterador.
        // Costo: O(1).
        V valor() {
            return act->valor;
        }

        // Pre:  hay_mas().
        // Post: el iterador pasa al par siguiente (o al final, si no hay mas).
        // Costo: O(c) en peor caso. Recorrer la tabla entera (crear el
        //        iterador y avanzar hasta el final) cuesta O(n + c).
        void avanzar() {
            if(act->sig != nullptr){
                act = act->sig;
            }
            else{
                pos++;
                while (pos < tabla->c && tabla->buckets[pos] == nullptr){
                    pos++;
                }
                
                if(pos < tabla->c){
                    act = tabla->buckets[pos];
                }
                else{
                    act = nullptr;
                }
            }
        }
    };

    // Pre:  -
    // Post: devuelve un iterador al primer par de la tabla (si la tabla esta
    //       vacia, el iterador ya esta al final).
    // Costo: O(c) en peor caso.
    Iterador crear_iterador();
};

// ---------------------------------------------------------------------------
// Implementacion. Al ser una clase template, va en el mismo .h.
// ---------------------------------------------------------------------------

template <typename K, typename V>
TablaHash<K, V>::TablaHash(size_t capacidad_inicial) {
    n = 0;
    c = capacidad_inicial;
    buckets = new Nodo*[c];

    for (size_t i = 0; i < c; i++){
        buckets[i] = nullptr;
    }
}

template <typename K, typename V>
TablaHash<K, V>::~TablaHash() {
    for(size_t i = 0; i < c; i ++){
        Nodo* act = buckets[i];

        while (act != nullptr){
            Nodo * siguiente = act->sig;
            delete act;
            act = siguiente;
        }   
    }
    delete[] buckets;
}

template <typename K, typename V>
void TablaHash<K, V>::insertar(const K& clave, const V& valor) {
    
    if(n+1 > FACTOR_DE_CARGA_MAXIMO * c){
        size_t nuevaCapacidad = c*2;
        Nodo ** nuevosBuckets = new Nodo*[nuevaCapacidad];
        
        for (size_t i = 0; i < nuevaCapacidad; i++) {
            nuevosBuckets[i] = nullptr;
        }

        for (size_t i = 0; i < c; i ++){
            Nodo* act = buckets[i];

            while (act != nullptr){
                Nodo* sig = act->sig;

                size_t pos = funcion_hash(act->clave) % nuevaCapacidad;
                nuevosBuckets[pos] = act;

                act = sig;
            }
        }
        delete[] buckets;
        buckets = nuevosBuckets;
        c = nuevaCapacidad;
    }
    
    size_t pos = funcion_hash(clave) % c;

    Nodo* act = buckets[pos];
    Nodo* nuevo = new Nodo{clave, valor, act};

    buckets[pos] = nuevo;
    n++;
}

template <typename K, typename V>
void TablaHash<K, V>::eliminar(const K& clave) {
    size_t pos = funcion_hash(clave) % c;

    Nodo* act = buckets[pos];
    Nodo * prev = nullptr;

    while (act != nullptr && act->clave != clave){
        prev = act;
        act = act->sig;
    }
    if(prev == nullptr){
        buckets[pos] = act->sig;
    }
    else{
        prev->sig = act->sig;
    }

    delete act;
    n--;
}

template <typename K, typename V>
V* TablaHash<K, V>::buscar(const K& clave) {
    size_t pos = funcion_hash(clave) % c;
    Nodo* act = buckets[pos];

    while (act != nullptr && act->clave != clave){
        act = act->sig;
    }
    
    if(act != nullptr){
        return &(act->valor);
    }

    return nullptr;
}

template <typename K, typename V>
size_t TablaHash<K, V>::cantidad() {
    return n;
}

template <typename K, typename V>
size_t TablaHash<K, V>::capacidad() {
    return c;
}

template <typename K, typename V>
size_t TablaHash<K, V>::colisiones() {
    size_t contador = 0;

    for(size_t i = 0; i < c; i++){
        if(buckets[i] != nullptr) contador++;
    }

    size_t colisiones_tot = n - contador;

    return colisiones_tot;
}

// El "typename" le avisa al compilador que TablaHash<K, V>::Iterador es un tipo.
template <typename K, typename V>
typename TablaHash<K, V>::Iterador TablaHash<K, V>::crear_iterador() {
    Iterador it;
    it.tabla = this;
    it.pos = 0;

    while (it.pos < c && buckets[it.pos] == nullptr) {
        it.pos++;
    }

    if(it.pos < c){
        it.act = buckets[it.pos];
    }
    else{
        it.act = nullptr;
    }
    return it;
}

#endif
