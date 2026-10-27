#pragma once
#include "player.hpp"
#include <vector>
#include <string>

class GameState {
private:
    std::vector<Player> jugadores;
    int turno_actual_index;
    bool juego_iniciado;

    // Función interna para el cálculo con el dado
    int calcularEfectividad(int poder_total, int valor_dado);

public:
    GameState();

    void agregarJugador(const Player& jugador);
    bool verificarCondicionInicio() const;
    void iniciarJuego();

    Player& getJugadorActual();
    
    // Cambia el turno y reduce los cooldowns del siguiente jugador
    void avanzarTurno(); 
    
    // Procesa la acción y devuelve un texto narrativo con el resultado
    std::string ejecutarMovimiento(Player& atacante, Player& objetivo, int index_movimiento, int valor_dado);
    
    Equipo verificarGanador() const;

    Player& getJugador(int index);

    int getTurnoActualIndex() const { return turno_actual_index; }
};
