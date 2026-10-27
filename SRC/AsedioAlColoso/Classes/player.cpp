#include "player.hpp"

// Constructor: Fase 1, solo conocemos el nombre
Player::Player(const std::string& nombre_jugador) {
    nombre = nombre_jugador;
    bando = Equipo::NINGUNO;
    nombre_clase = "Sin Clase";
    
    // Inicializamos todo en 0 por seguridad
    hp = 0;
    hp_max = 0;
    atk = 0;
    mag = 0;
    def = 0;
    mag_def = 0;
}

// Fase 1: Asignar bando (Aventurero o Coloso)
void Player::asignarEquipo(Equipo nuevo_equipo) {
    bando = nuevo_equipo;
}

// Fase 2: Cargar las estadísticas base de la clase elegida
void Player::asignarClase(const std::string& clase, int vida, int ataque, int magia, int defensa, int def_magica) {
    nombre_clase = clase;
    hp_max = vida;
    hp = vida; // Inicia con la vida al máximo
    atk = ataque;
    mag = magia;
    def = defensa;
    mag_def = def_magica;
}

// Añadir habilidades al arsenal del jugador
void Player::agregarMovimiento(const std::string& nom, int pod, TipoMovimiento t, int cd) {
    Movimiento nuevo_mov;
    nuevo_mov.nombre = nom;
    nuevo_mov.poder = pod;
    nuevo_mov.tipo = t;
    nuevo_mov.cooldown_base = cd;
    nuevo_mov.cooldown_actual = 0; // Listo para usarse inmediatamente

    movimientos.push_back(nuevo_mov);
}

// Lógica de turnos: reduce en 1 el tiempo de espera de los ataques usados
void Player::reducirCooldowns() {
    for (auto& mov : movimientos) {
        if (mov.cooldown_actual > 0) {
            mov.cooldown_actual--;
        }
    }
}

// Getters
std::string Player::getNombre() const { return nombre; }
Equipo Player::getEquipo() const { return bando; }
std::string Player::getNombreClase() const { return nombre_clase; }

// ==========================================
// LÓGICA DE COMBATE
// ==========================================
void Player::recibirDano(int cantidad) {
    hp -= cantidad;
    if (hp < 0) {
        hp = 0; // Evitar hp negativo
    }
}

void Player::curar(int cantidad) {
    hp += cantidad;
    if (hp > hp_max) {
        hp = hp_max; // Evitamos curar más allá del límite
    }
}

bool Player::estaVivo() const {
    return hp > 0;
}

// getters de stats===================
int Player::getHp() const { return hp; }
int Player::getHpMax() const { return hp_max; }
int Player::getAtk() const { return atk; }
int Player::getMag() const { return mag; }
int Player::getDef() const { return def; }
int Player::getMagDef() const { return mag_def; }

std::vector<Movimiento>& Player::getMovimientos() { 
    return movimientos; 
}

std::string Player::getClase() const {
    return nombre_clase;
}