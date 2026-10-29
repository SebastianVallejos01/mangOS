#include "GameState.hpp"
#include <cmath>
#include "../Metodo/time.hpp"
#include <algorithm>
#include <iostream> // esta aca pa mandar mensajes al server

// AÑADIDO: Declaramos que esta función existe en otro archivo (server.cpp)
// Así evitamos el error de "múltiples definiciones" al compilar.
extern void guardarLog(const std::string& mensaje);

GameState::GameState() {
    turno_actual_index = 0;
    juego_iniciado = false;
}

void GameState::agregarJugador(const Player& jugador) {
    jugadores.push_back(jugador);
}

bool GameState::verificarCondicionInicio() const {
    int aventureros = 0, colosos = 0;
    for (const auto& j : jugadores) {
        if (j.getEquipo() == Equipo::AVENTURERO) aventureros++;
        if (j.getEquipo() == Equipo::COLOSO) colosos++;
    }
    return (aventureros == 3 && colosos == 1);
}

void GameState::iniciarJuego() {
    if (verificarCondicionInicio()) {
        juego_iniciado = true;
        turno_actual_index = 0;
        // Nos aseguramos de que el primer jugador empiece con sus cooldowns listos
        jugadores[turno_actual_index].reducirCooldowns();
    }
}

Player& GameState::getJugadorActual() {
    return jugadores[turno_actual_index];
}

void GameState::avanzarTurno() {
    do {
        turno_actual_index++;
        if (turno_actual_index >= jugadores.size()) {
            turno_actual_index = 0;
        }
    } while (!jugadores[turno_actual_index].estaVivo());

    // Inicia el turno de este jugador: reducimos sus tiempos de espera
    jugadores[turno_actual_index].reducirCooldowns();
}

int GameState::calcularEfectividad(int poder_total, int valor_dado) {
    // Un dado hara de multiplicador para variar el combate a algo con suerte tambien
    float multiplicadores[6] = {0.7f, 0.85f, 1.0f, 1.10f, 1.2f, 1.4f};
    float multiplicador = multiplicadores[valor_dado - 1];
    
    return static_cast<int>(std::round(poder_total * multiplicador));
}

std::string GameState::ejecutarMovimiento(Player& atacante, Player& objetivo, int index_movimiento, int valor_dado) {
    auto& movimientos = atacante.getMovimientos();
    if (index_movimiento < 0 || index_movimiento >= movimientos.size()) {
        return "Movimiento inválido.";
    }
    
    Movimiento& mov = movimientos[index_movimiento];
    
    // Verificamos si el movimiento está en enfriamiento
    if (mov.cooldown_actual > 0) {
        return "'" + mov.nombre + " aún está recargándose, no puedo usarlo.";
    }

    // CORRECCIÓN APLICADA: Ponemos el movimiento en enfriamiento (con el +1 para cuadrar turnos)
    if (mov.cooldown_base > 0) {
        mov.cooldown_actual = mov.cooldown_base + 1;
    } else {
        mov.cooldown_actual = 0;
    }

    std::string resumen = atacante.getNombre() + " usó " + mov.nombre + " (Dado: " + std::to_string(valor_dado) + "). ";

    // ==========================================
    // CÁLCULO DE CURACIÓN
    // ==========================================
    if (mov.tipo == TipoMovimiento::CURACION) {
        int curacion = calcularEfectividad(mov.poder, valor_dado);
        objetivo.curar(curacion);
        resumen += objetivo.getNombre() + " recuperó " + std::to_string(curacion) + " puntos de su HP maxima.";
        
        std::cout<<getTimestamp()<<"; "<<atacante.getNombre() <<"; "
                    << (atacante.getEquipo() == Equipo::COLOSO ? "COLOSO" : "AVENTURERO") 
                    << "; Curó "<<std::to_string(curacion) <<" a "<<objetivo.getNombre() <<"; "
                    << (objetivo.getEquipo() == Equipo::COLOSO ? "COLOSO" : "AVENTURERO") 
                    << std::endl;
                    
        guardarLog(getTimestamp() + "; " + atacante.getNombre() + "; " +
                   (atacante.getEquipo() == Equipo::COLOSO ? "COLOSO" : "AVENTURERO") + 
                   "; Curó " + std::to_string(curacion) + " a " + objetivo.getNombre() + "; " +
                   (objetivo.getEquipo() == Equipo::COLOSO ? "COLOSO" : "AVENTURERO"));
                   
        return resumen;
    }

    // ==========================================
    // CÁLCULO DE DAÑO (FÍSICO O MÁGICO)
    // ==========================================
    int stat_ataque = 0;
    int stat_defensa = 0;

    if (mov.tipo == TipoMovimiento::FISICO) {
        stat_ataque = atacante.getAtk();
        stat_defensa = objetivo.getDef();
    } else if (mov.tipo == TipoMovimiento::MAGICO) {
        stat_ataque = atacante.getMag();
        stat_defensa = objetivo.getMagDef();
    }

    // Sumamos el stat base del jugador + el poder bruto del ataque y lanzamos el dado
    int poder_bruto = stat_ataque + mov.poder;
    int poder_con_dado = calcularEfectividad(poder_bruto, valor_dado);

    // Restamos la defensa enemiga correspondiente. Si la defensa es mayor, el daño mínimo es 0.
    int dano_final = std::max(0, poder_con_dado - stat_defensa);
    
    objetivo.recibirDano(dano_final);
    resumen += "Causó " + std::to_string(dano_final) + " de daño a " + objetivo.getNombre() +" (" + objetivo.getClase()+").";
    std::cerr << atacante.getNombre() + " causó " + std::to_string(dano_final) + " de daño a "+ objetivo.getNombre() +" (" + objetivo.getClase()+")." << std::endl;

    std::cout<<getTimestamp()<<"; "<<atacante.getNombre() <<"; "
    << (atacante.getEquipo() == Equipo::COLOSO ? "COLOSO" : "AVENTURERO") 
    << "; Dañó "<<std::to_string(dano_final) <<" a "<<objetivo.getNombre() <<"; "
    << (objetivo.getEquipo() == Equipo::COLOSO ? "COLOSO" : "AVENTURERO") 
    << std::endl;
    
    guardarLog(getTimestamp() + "; " + atacante.getNombre() + "; " +
               (atacante.getEquipo() == Equipo::COLOSO ? "COLOSO" : "AVENTURERO") + 
               "; Dañó " + std::to_string(dano_final) + " a " + objetivo.getNombre() + "; " +
               (objetivo.getEquipo() == Equipo::COLOSO ? "COLOSO" : "AVENTURERO"));

    return resumen;
}

Equipo GameState::verificarGanador() const {
    bool coloso_vivo = false;
    bool aventurero_vivo = false;

    for (const auto& j : jugadores) {
        if (j.estaVivo()) {
            if (j.getEquipo() == Equipo::COLOSO) coloso_vivo = true;
            if (j.getEquipo() == Equipo::AVENTURERO) aventurero_vivo = true;
        }
    }

    if (!coloso_vivo) {
        std::cout<<getTimestamp()<<"; Ganan los AVENTUREROS"<< std::endl;
        guardarLog(getTimestamp() + "; Ganan los AVENTUREROS");
        return Equipo::AVENTURERO; 
    }
    if (!aventurero_vivo){
        std::cout<<getTimestamp()<<"; Gana el Coloso"<< std::endl; 
        guardarLog(getTimestamp() + "; Gana el Coloso");
        return Equipo::COLOSO;
    } 

    return Equipo::NINGUNO; 
}

Player& GameState::getJugador(int index) {
    return jugadores[index];
}