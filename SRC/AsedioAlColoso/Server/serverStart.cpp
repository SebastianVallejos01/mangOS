#include "server.hpp"

int main() {
    Server mi_servidor(8080);

    if (mi_servidor.iniciar()) {
        bool juego_listo = false;

        while (!juego_listo) {
            mi_servidor.esperarJugadores();
            
            // Si alguien se desconecta en Fase 1, repetimos el ciclo volviendo al lobby
            if (!mi_servidor.faseSeleccionEquipo()) {
                continue; 
            }

            // FAse 2, los jugadores elegiran quienes seran en esta fiera batalla

            if (!mi_servidor.faseSeleccionClase()) {
                continue;
            }
  

            // Si llegamos hasta aquí, nadie se desconectó
            juego_listo = true;
            // FAse 3 Los jugadores se enfrentaran en una batalla 3 vs 1
            // Si alguno se desconecta, el jugador se dará por muerto en batalla, si era el unico vivo de su equipo entonces su equipo perderá
            
            mi_servidor.loopCombate();
        }
    }

    return 0;
}