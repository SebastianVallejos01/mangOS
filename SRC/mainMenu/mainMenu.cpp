#include "mainMenu.hpp"

void mainMenu(const User& loggedUser, const Profile& loggedProfile, const std::string& userRuta, const std::string& rutaUserFile, const std::string& rutaPerfilFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles){
    int opcion = 0;
    bool tienePermiso=false;

    do {
        std::cout << "\n========================================" << std::endl;
        std::cout << "         MENÚ PRINCIPAL mangOS          " << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Usuario: " << loggedUser.username << std::endl;
        std::cout << "Perfil:  " << loggedProfile.name << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "1. Administración de usuarios y perfiles" << std::endl;
        std::cout << "2. Multiplica matrices NxM" << std::endl;
        std::cout << "3. Juego" << std::endl;
        std::cout << "4. ¿Es palíndromo?" << std::endl;
        std::cout << "5. Calcular f(x) = x^2 + 2x + 8" << std::endl;
        std::cout << "6. Conteo sobre texto" << std::endl;
        std::cout << "7. Conteo sobre archivo" << std::endl;
        std::cout << "0. Salir" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Seleccione una opción: ";

        while (!(std::cin >> opcion)) {
            std::cout << "Entrada inválida. Intente nuevamente: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        tienePermiso = hasPermiso(loggedProfile, opcion);
        if (tienePermiso){
                
            switch (opcion) {
                case 1:
                    std::cout << "\n=== ADMINISTRACIÓN DE USUARIOS Y PERFILES ===" << std::endl;
                    std::cout << "Llamando al sistema correspondiente..." << std::endl;
                    menuUserManager(rutaUserFile, rutaPerfilFile, ListaUsuarios, ListaPerfiles);
                    break;

                case 2: {
                    std::cout << "\n=== MULTIPLICA MATRICES NxM ===" << std::endl;
                    std::string rutaA, rutaB, separador;
                    std::cout << "Ingrese la ruta de la matriz A: ";
                    std::getline(std::cin, rutaA);
                    std::cout << "Ingrese la ruta de la matriz B: ";
                    std::getline(std::cin, rutaB);
                    std::cout << "Ingrese el caracter separador: ";
                    std::getline(std::cin, separador);

                    if (rutaA.empty() || rutaB.empty() || separador.empty()) {
                        std::cout << "Error: Todos los campos son obligatorios." << std::endl;
                        break;
                    }

                    // Llamada de sistema al programa independiente "multi" (multi.exe en Windows)
                    std::string comando = "./multi \"" + rutaA + "\" \"" + rutaB + "\" \"" + separador + "\" \"" + loggedUser.username + "\" \"" + loggedProfile.name + "\"";
                    std::cout << "Ejecutando: " << comando << "\n" << std::endl;
                    int result = system(comando.c_str());
                    
                    if (result != 0) {
                        std::cout << "\nEl programa externo finalizó con código de error o no pudo ser encontrado." << std::endl;
                    }
                    break;
                }
                case 3:
                    std::cout << "\n=== JUEGO ===" << std::endl;
                    std::cout << "En construcción. Funcionalidad futura." << std::endl;
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
        } else {
            std::cout << "\nError: No tiene permiso para acceder a esta opción." << std::endl;
        }

        if (opcion != 0) {
            std::cout << "\nPresione Enter para continuar...";
            std::cin.get();
        }

    } while (opcion != 0);
}