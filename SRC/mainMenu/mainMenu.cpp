#include "mainMenu.hpp"

#include <iostream>
#include <string>
#include <limits>

void mainMenu(const User& loggedUser, const std::string& userRuta, const std::string& rutaUserFile, const std::string& rutaPerfilFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles){
    int option = 0;

    do {
        std::cout << "\n========================================" << std::endl;
        std::cout << "            MENÚ PRINCIPAL              " << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Usuario: " << loggedUser.username << std::endl;
        std::cout << "Perfil:  " << loggedUser.perfil << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "1. Administración de usuarios y perfiles" << std::endl;
        std::cout << "2. Multiplica matrices NxM" << std::endl;
        std::cout << "3. Juego" << std::endl;
        std::cout << "4. ¿Es palíndromo?" << std::endl;
        std::cout << "5. Calcular f(x) = x*x + 2x + 8" << std::endl;
        std::cout << "6. Conteo sobre texto" << std::endl;
        std::cout << "7. Conteo sobre archivo" << std::endl;
        std::cout << "0. Salir" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Seleccione una opción: ";

        while (!(std::cin >> option)) {
            std::cout << "Entrada inválida. Intente nuevamente: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (option) {
            case 1:
                if (loggedUser.perfil == "ADMIN") {
                    std::cout << "\n=== ADMINISTRACIÓN DE USUARIOS Y PERFILES ===" << std::endl;
                    std::cout << "Llamando al sistema correspondiente..." << std::endl;
                    menuUserManager(rutaUserFile, rutaPerfilFile, ListaUsuarios, ListaPerfiles);
                } else {
                    std::cout << "\nAcceso denegado: esta opción solo puede ser usada por el perfil ADMIN." << std::endl;
                }
                break;

            case 2:
                std::cout << "\n=== MULTIPLICA MATRICES NxM ===" << std::endl;
                std::cout << "Debe leer los archivos de texto con las matrices desde otro programa." << std::endl;
                break;

            case 3:
                std::cout << "\n=== JUEGO ===" << std::endl;
                std::cout << "Mensaje en construcción." << std::endl;
                break;

            case 4:
                menuPalindromo();
                break;

            case 5:
                menuPoli();
                break;

            case 6:
                conteoTexto(userRuta);
                break;

            case 7:
                menuConteoTexto();
                break;

            case 0:
                std::cout << "\nSaliendo del menú principal..." << std::endl;
                break;

            default:
                std::cout << "\nOpción no válida. Intente nuevamente." << std::endl;
                break;
        }

        if (option != 0) {
            std::cout << "\nPresione Enter para continuar...";
            std::cin.get();
        }

    } while (option != 0);
}