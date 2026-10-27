#include "goleadores.h"

#include <iostream>

Goleadores::Goleadores() {
    // COMPLETAR (si hace falta)
}

void Goleadores::anotar_gol(const std::string& jugador) {

    // COMPLETAR

    int goles_actuales = 0;

    if (goles_por_jugador.pertenece(jugador) == true) {

        goles_actuales = goles_por_jugador.obtener(jugador);
    }

    int nuevos_goles_anotados = goles_actuales + 1;

    goles_por_jugador.insertar(jugador, nuevos_goles_anotados);

    if (goles_por_jugador.cantidad() == 1 or nuevos_goles_anotados > goles_por_jugador.obtener(maximo)) {  // Consideramos al "maximo" únicamente
                                                                                                           // a aquel que supere al jugador con mayor
                                                                                                           // cantidad de goles.

        maximo = jugador;  // Actualizamos el valor de "maximo" si dicho jugador es el que más goles anotó.
    }

}

int Goleadores::goles(const std::string& jugador) {

    // COMPLETAR

    if (goles_por_jugador.pertenece(jugador)){  // Verificamos primero que el jugador se encuentre en el diccionario.
                                                // (La precondición del contrato de la función no garantiza que pertenezca).

        return goles_por_jugador.obtener(jugador);
    }

    return 0;

}

size_t Goleadores::cantidad_de_jugadores() {

    // COMPLETAR

    return goles_por_jugador.cantidad();
    
}

std::string Goleadores::maximo_goleador() {

    // COMPLETAR

    return maximo;  // Devuelve únicamente el jugador con más goles anotados dado que
                   // "maximo" solo se actualiza si un jugador supera la cifra de goles de otro (no lo hace si la empata).
    
}

// FUNCIONES AUXILIARES PARA LA PARTE OPTATIVA DE LA FUNCIÓN "imprimir_por_nombre()":

void Goleadores::unir_nombres(std::vector<std::string>& arr, int izquierda, int medio, int derecha) { 

    int mitad_1 = medio - izquierda  + 1;
    int mitad_2 = derecha - medio;

    std::vector<std::string> izq(mitad_1);
    std::vector<std::string> der(mitad_2);

    for (int i = 0; i < mitad_1; i++) {
        izq[i] = arr[izquierda + 1];
    }

    for (int j = 0; j < mitad_2; j++){
        der[j] = arr[medio + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = izquierda;

    while (i < mitad_1 and j < mitad_2) {
        if (izq[i] <= der[j]) {
            arr[k] = izq[i];
            i++;
        }

        else {
            arr[k] = der[j];
            j++;
        }

        k++;
    }

    while (i < mitad_1) {
        arr[k] = izq[i];
        i++;
        k++;
    }

    while (j < mitad_2) {
        arr[k] = der[j];
        j++;
        k++;
    }
}

void Goleadores::merge_sort_nombres_goleadores(std::vector<std::string>& arr, int izquierda, int derecha) {
    if (izquierda < derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;

        merge_sort_nombres_goleadores(arr, izquierda, medio);
        merge_sort_nombres_goleadores(arr, medio + 1, derecha);
    }
}

void Goleadores::imprimir_por_nombre() {

    // OPTATIVO

    size_t cantidad_jugadores = cantidad_de_jugadores();
    if (cantidad_jugadores == 0) {
        return;
    }

    std::vector<std::string> jugadores;
    Diccionario<std::string, int>::Iterador it = goles_por_jugador.crear_iterador();

    while (it.hay_mas() == true) {
        jugadores.push_back(it.clave());
        it.avanzar();
    }

    merge_sort_nombres_goleadores(jugadores, 0, jugadores.size() - 1);

    for (const std::string& jugador : jugadores) {
        std::cout << jugador <<": " << goles(jugador) << std::endl;
    }


}

void Goleadores::imprimir_por_goles() {

    // OPTATIVO

    
}
