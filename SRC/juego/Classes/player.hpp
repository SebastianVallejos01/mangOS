#pragma once
#include <string>
#include <vector>

// Definimos los equipos
enum class Equipo { NINGUNO, AVENTURERO, COLOSO };

// Definimos los tipos de daño/acción para saber qué stats usar
enum class TipoMovimiento { FISICO, MAGICO, CURACION };

// Estructura que define cómo es un movimiento
struct Movimiento {
    std::string nombre;
    int poder;
    TipoMovimiento tipo;
    
    int cooldown_base;    // Ej: 3 (Tarda 3 turnos en recargarse)
    int cooldown_actual;  // Ej: Empieza en 0. Si lo usas, sube a 3. Baja 1 cada turno.
};

class Player {
private:
    // Fase 1: Lobby y Equipo
    std::string nombre;
    Equipo bando;

    // Fase 2: Selección de Clase
    std::string nombre_clase;

    // Estadísticas universales
    int hp;
    int hp_max;
    int atk;        //daño fisico
    int mag;        //daño magico
    int def;        //defensa fisica
    int mag_def;    //defensa magica

    // La lista de habilidades que tiene este jugador
    std::vector<Movimiento> movimientos;
    

public:
    Player(const std::string& nombre_jugador);

    void asignarEquipo(Equipo nuevo_equipo);
    void asignarClase(const std::string& clase, int vida, int ataque, int magia, int defensa, int def_magica);
    
    // Método para guardar los ataques cuando elige su clase
    void agregarMovimiento(const std::string& nom, int pod, TipoMovimiento t, int cd);

    // Método que llamaremos al inicio del turno del jugador para reducir sus tiempos de espera
    void reducirCooldowns();

    // Getters
    std::string getNombre() const;
    Equipo getEquipo() const;
    std::string getNombreClase() const;

    // Lógica de combate
    void recibirDano(int cantidad);
    void curar(int cantidad);
    bool estaVivo() const;

    // Getters de estadísticas
    int getHp() const;
    int getHpMax() const;
    int getAtk() const;
    int getMag() const;
    int getDef() const;
    int getMagDef() const;
    
    // Devuelve los movimientos por referencia para poder modificar sus cooldowns
    std::vector<Movimiento>& getMovimientos();
    std::string getClase() const;
};