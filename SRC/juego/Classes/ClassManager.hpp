#pragma once
#include "player.hpp"
#include <vector>
#include <string>

class ClassManager {
public:
    // Retorna la lista de clases permitidas según el equipo del jugador
    static std::vector<std::string> getClasesDisponibles(Equipo bando);

    // Asigna los stats y movimientos si la clase solicitada corresponde al equipo del jugador.
    // Retorna true si fue exitoso, false si intentó elegir una clase inválida.
    static bool asignarClase(Player& jugador, const std::string& nombre_clase);
};