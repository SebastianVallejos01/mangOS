#include "mates.hpp"
#include "../userManager/userManager.hpp"
#include "../login/login.hpp"
#include <limits>
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

void menuPoli(){
    std::string opcion;
    do {
        std::cout << "\n--- Cálculo de función polinómica ---\n";
        std::cout << "Ingrese preimagen x. Entrada no numérica para salir: ";
        std::cin >> opcion;
        try {
            std::size_t index = 0;
            double numero = std::stod(opcion, &index);
            if (index != opcion.size()) {
                throw std::invalid_argument("Entrada no válida");
            }
            polinomio(numero);
        } catch (const std::exception&) {
            std::cout << "Entrada no numérica ingresada. Saliendo del cálculo polinómico." << std::endl;
            break;
        }
    } while (true);
}

void polinomio(const double& x){
    std::cout<<"Número ingresado: "<<x<<std::endl;
    std::cout<<"f("<<x<<")= ("<<x<<")^2 + 2*("<<x<<") + 8 = "<<x*x+2*x+8<<std::endl;
}

void menuMatrices(const std::string& usernameParam, const std::string& profileParam) {
    std::string user = usernameParam;
    std::string prof = profileParam;

    if (user.empty() && loggedUser.has_value()) {
        user = loggedUser->username;
        prof = loggedUser->perfil;
    }
    if (user.empty()) user = "usuario";
    if (prof.empty()) prof = "perfil";

    int opcion = 0;
    do {
        std::cout << "\n========================================\n";
        std::cout << "      MULTIPLICACIÓN DE MATRICES NxM     \n";
        std::cout << "========================================\n";
        std::cout << "1. Trabajar con los valores del .env\n";
        std::cout << "2. Ingresar las rutas manualmente\n";
        std::cout << "0. Volver\n";
        std::cout << "========================================\n";
        std::cout << "Seleccione una opción: ";

        if (!(std::cin >> opcion)) {
            std::cout << "Entrada inválida. Intente nuevamente.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (opcion == 0) {
            std::cout << "\nVolviendo al menú principal...\n";
            break;
        }

        std::string rutaA, rutaB, separador;

        if (opcion == 1) {
            std::cout << "\n--- Usando rutas desde .env ---\n";
            std::optional<std::string> envA = valorEnvMatrizA;
            std::optional<std::string> envB = valorEnvMatrizB;
            std::optional<std::string> envSep = valorEnvSeparador;

            // Si aún no estaban cargadas en variables globales, buscamos en el archivo .env
            if (!envA || !envB) {
                fs::path rutaEnv = getEnvFile(fs::current_path(), ".env");
                if (!rutaEnv.empty()) {
                    envA = getEnvVar(rutaEnv, "MATRIZ_A");
                    if (!envA) envA = getEnvVar(rutaEnv, "RUTA_MATRIZ_A");
                    envB = getEnvVar(rutaEnv, "MATRIZ_B");
                    if (!envB) envB = getEnvVar(rutaEnv, "RUTA_MATRIZ_B");
                    envSep = getEnvVar(rutaEnv, "SEPARADOR_MATRICES");
                    if (!envSep) envSep = getEnvVar(rutaEnv, "SEPARADOR");
                }
            }

            if (envA && envB) {
                rutaA = envA.value();
                rutaB = envB.value();
                separador = envSep.value_or("#");
                std::cout << "Ruta Matriz A: " << rutaA << "\n";
                std::cout << "Ruta Matriz B: " << rutaB << "\n";
                std::cout << "Separador: " << separador << "\n";
            } else {
                std::cout << "Error: No se encontraron las rutas de las matrices (MATRIZ_A y/o MATRIZ_B) en el archivo .env.\n";
                continue;
            }
        } else if (opcion == 2) {
            std::cout << "\n--- Ingreso manual de rutas ---\n";
            std::cout << "Ingrese la ruta de la matriz A: ";
            std::getline(std::cin, rutaA);
            std::cout << "Ingrese la ruta de la matriz B: ";
            std::getline(std::cin, rutaB);
            std::cout << "Ingrese el caracter separador: ";
            std::getline(std::cin, separador);

            if (rutaA.empty() || rutaB.empty() || separador.empty()) {
                std::cout << "Error: Todos los campos son obligatorios.\n";
                continue;
            }
        } else {
            std::cout << "\nOpción no válida. Intente nuevamente.\n";
            continue;
        }

        // Ejecutar programa independiente 'multi'
        std::string ejecutable = "./multi";
        #ifdef _WIN32
        if (fs::exists("multi.exe") || fs::exists("./multi.exe")) {
            ejecutable = "multi.exe";
        }
        #endif

        std::string comando = ejecutable + " \"" + rutaA + "\" \"" + rutaB + "\" \"" + separador + "\" \"" + user + "\" \"" + prof + "\"";
        std::cout << "Ejecutando: " << comando << "\n\n";
        int result = system(comando.c_str());

        // Si falló en Windows, intentar con llamada alternativa
        if (result != 0) {
            std::string fallbackCmd = "./multi \"" + rutaA + "\" \"" + rutaB + "\" \"" + separador + "\" \"" + user + "\" \"" + prof + "\"";
            if (fallbackCmd != comando) {
                result = system(fallbackCmd.c_str());
            }
            if (result != 0) {
                std::cout << "\nEl programa externo finalizó con código de error o no pudo ser encontrado.\n";
            }
        }

    } while (opcion != 0);
}

void menuMatrices(const User& loggedUser, const Profile& loggedProfile) {
    menuMatrices(loggedUser.username, loggedProfile.name);
}

void menuMatriz(const std::string& username, const std::string& profile) {
    menuMatrices(username, profile);
}