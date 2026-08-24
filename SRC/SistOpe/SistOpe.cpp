#include "userManager/userManager.hpp"

int main() {
    fs::path rutaEnv;
    rutaEnv = getEnvFile(fs::current_path(), ".env");
    
    if (rutaEnv.empty()) return 1;
    
    valorEnvUsuario = getEnvVar(rutaEnv, "USER_FILE");
    valorEnvPerfil = getEnvVar(rutaEnv, "PERFIL_FILE");
    
    if (!valorEnvUsuario || !valorEnvPerfil) return 1;

    UserList listaUsuarios;
    ProfileList listaPerfiles;

    leeUsuariosTxt(valorEnvUsuario.value(), listaUsuarios);
    leePerfilesTxt(valorEnvPerfil.value(), listaPerfiles);

    int opcion;
    do {
        std::cout << "\n--- SISTEMA SISTOPE ---\n";
        std::cout << "1) Gestion de Usuarios\n";
        std::cout << "2) Gestion de Perfiles\n";
        std::cout << "0) Salir\n";
        std::cout << "Opcion: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                menuUsuarios(valorEnvUsuario.value(), listaUsuarios);
                break;
            case 2:
                menuPerfiles(valorEnvPerfil.value(), listaPerfiles);
                break;
            case 0:
                break;
            default:
                std::cout << "Opcion invalida.\n";
                break;
        }
    } while (opcion != 0);
    
    return 0;
}