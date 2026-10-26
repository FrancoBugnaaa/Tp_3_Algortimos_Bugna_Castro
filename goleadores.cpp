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

void Goleadores::imprimir_por_nombre() {
    // OPTATIVO
}

void Goleadores::imprimir_por_goles() {
    // OPTATIVO
}
