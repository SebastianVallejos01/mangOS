#include "ClassManager.hpp"
#include "../Metodo/time.hpp"

std::vector<std::string> ClassManager::getClasesDisponibles(Equipo bando) {
    if (bando == Equipo::AVENTURERO) {
        return {"Guerrero", "Mago", "Paladin","Asesino"}; 
    } else if (bando == Equipo::COLOSO) {
        return {"Lobo Eclipse", "Dragon Dorado","Samurai Eterno"}; 
    }
    return {};
}

bool ClassManager::asignarClase(Player& jugador, const std::string& nombre_clase) {
    Equipo bando_actual = jugador.getEquipo();

    // ==========================================
    // CLASES DE AVENTUREROS
    // ==========================================
    if (bando_actual == Equipo::AVENTURERO) {
        if (nombre_clase == "Guerrero") {
            // Orden: clase, hp, atk, mag, def, mag_def
            jugador.asignarClase("Guerrero", 250, 30, 20, 25, 25);
            
            // Orden: nombre, poder, tipo, cooldown_base Todos tienen cooldown +1 de lo que deberia porque es mas sencillo que mover codigo de sitio
            // La industria de los videojuegos siempre se ha tratado de atajos y hacer facil lo dificil
            jugador.agregarMovimiento("Espadazo", 40, TipoMovimiento::FISICO, 0);
            jugador.agregarMovimiento("Corte Flamigero", 60, TipoMovimiento::MAGICO, 1);
            jugador.agregarMovimiento("Estocada al Ojo",100,TipoMovimiento::FISICO,3);
            return true;
        }
        else if (nombre_clase=="Paladin"){
            jugador.asignarClase("Paladin",300, 35, 25,50,30);
            jugador.agregarMovimiento("Placaje", 25,TipoMovimiento::FISICO,0);
            jugador.agregarMovimiento("Arremetida Propulsada", 50,TipoMovimiento::FISICO,1);
            jugador.agregarMovimiento("Luz Santa", 60,TipoMovimiento::MAGICO,2);
            jugador.agregarMovimiento("Descenso Final",80,TipoMovimiento::FISICO,3);
            return true;
        }

        else if (nombre_clase == "Mago") {
            jugador.asignarClase("Mago", 220, 20, 45, 10, 50);
            jugador.agregarMovimiento("Bola de Fuego", 35, TipoMovimiento::MAGICO, 0);
            jugador.agregarMovimiento("Cuchillo Conjurado", 40,TipoMovimiento::FISICO,1);
            jugador.agregarMovimiento("Explosión", 100,TipoMovimiento::MAGICO,4);
            jugador.agregarMovimiento("Luz Sanadora", 80, TipoMovimiento::CURACION, 3);
            return true;
        }
        else if (nombre_clase == "Asesino"){
            jugador.asignarClase("Asesino",200,45,40,15,15);
            jugador.agregarMovimiento("Tajo Diagonal", 35,TipoMovimiento::FISICO,0);
            jugador.agregarMovimiento("Filo Electrico", 60,TipoMovimiento::MAGICO,1);
            jugador.agregarMovimiento("Danza Incesante", 60,TipoMovimiento::FISICO,1);
            jugador.agregarMovimiento("Analgesico", 40,TipoMovimiento::CURACION,2);
            return true;
        }
    }
    // ==========================================
    // CLASES DE COLOSOS
    // ==========================================
    else if (bando_actual == Equipo::COLOSO) {
        if (nombre_clase == "Dragon Dorado") {
            jugador.asignarClase("Dragon Dorado", 1100, 35, 65, 40, 50);
            jugador.agregarMovimiento("Zarpa Gigante", 55, TipoMovimiento::FISICO, 2);
            jugador.agregarMovimiento("Rafaga de viento", 35, TipoMovimiento::MAGICO, 1);
            jugador.agregarMovimiento("Escombros Gigantes", 60,TipoMovimiento::FISICO,3);
            jugador.agregarMovimiento("Flama Dorada", 70, TipoMovimiento::MAGICO, 5);
            return true;
        }
        else if (nombre_clase == "Samurai Eterno") {
            jugador.asignarClase("Samurai Eterno", 2000, 50, 50, 5, 5);
            jugador.agregarMovimiento("Corte Instantaneo", 55, TipoMovimiento::FISICO, 3);
            jugador.agregarMovimiento("Corte de Viento", 50, TipoMovimiento::MAGICO, 3);
            jugador.agregarMovimiento("Ogro Celestial", 60,TipoMovimiento::MAGICO,4);
            jugador.agregarMovimiento("Dragon Piroclastico", 70, TipoMovimiento::MAGICO, 5);
            jugador.agregarMovimiento("Cielo Azul", 95, TipoMovimiento::FISICO, 6);
            return true;
        }
        else if (nombre_clase == "Lobo Eclipse") {
            jugador.asignarClase("Lobo Eclipse", 1500, 40, 40, 50, 10);
            jugador.agregarMovimiento("Embestida", 40, TipoMovimiento::FISICO, 0);
            jugador.agregarMovimiento("Trampa de Sombras",50 , TipoMovimiento::MAGICO, 1);
            jugador.agregarMovimiento("Mordisco Sorpresa",60,TipoMovimiento::FISICO, 2);
            jugador.agregarMovimiento("Luz de Luna", 400, TipoMovimiento::CURACION, 4);
            return true;
        }
    }

    return false; // La clase no existe o no pertenece a su bando
}
