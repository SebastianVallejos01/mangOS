#include "menuJuego.hpp"
#include <iostream>
#include <limits>
#include <cstdlib> // Necesario para la función system()

void mostrarInstrucciones() {
    std::cout << "\n======================================================\n";
    std::cout << "          INSTRUCCIONES: ASEDIO AL COLOSO\n";
    std::cout << "======================================================\n";
    std::cout << "1. OBJETIVO: 3 Aventureros se enfrentaran a 1 Coloso, buscando reclamar el mundo.\n";
    std::cout << "2. FASE DE SELECCION: Identificate, elige tu bando y tu clase.\n";
    std::cout << "3. COMBATE: Usa tus habilidades en batalla considerando a vuestros\n";
    std::cout << "   adversarios, sus cualidades y lo que necesiten en el momento.\n";
    std::cout << "4. VICTORIA: Reduce los HP del bando enemigo a 0.\n";
    std::cout << "======================================================\n\n";
}

void menuJuego() {
    int opcion = -1;
    
    while (opcion != 0) {
        std::cout << "\n--- SUBMENU: ASEDIO AL COLOSO ---\n";
        std::cout << "1. Crear Servidor\n";
        std::cout << "2. Unirse a Servidor\n";
        std::cout << "3. Instrucciones\n";
        std::cout << "0. Salir (Volver atras)\n";
        std::cout << "Selecciona una opcion: ";
        
        if (!(std::cin >> opcion)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "ERROR| Entrada invalida. Ingresa un numero.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (opcion) {
            case 1:
                std::cout << "\n[SISTEMA] Iniciando el servidor de Asedio al Coloso...\n";
                // Llama al ejecutable de tu SERVIDOR
                // Cambia el nombre si tu ejecutable se llama distinto
                system("./servidor_juego"); 
                break;
            case 2:
                std::cout << "\n[SISTEMA] Conectando con el mundo de Asedio al Coloso...\n";
                // Llama al ejecutable de tu CLIENTE
                system("./cliente_juego"); 
                break;
            case 3:
                mostrarInstrucciones();
                break;
            case 0:
                std::cout << "\n[SISTEMA] Volviendo al menu principal de mangOS...\n";
                break;
            default:
                std::cout << "ERROR| Opcion no valida. Intenta de nuevo.\n";
                break;
        }
    }
}