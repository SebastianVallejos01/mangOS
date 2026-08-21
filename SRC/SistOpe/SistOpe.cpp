#include "userManager/userManager.hpp"

int main(){
    fs::path rutaEnv = getEnvFile(fs::current_path(), ".env");
    if (rutaEnv.empty()) {
        std::cerr << "No se encontró el archivo .env en ningún superdirectorio." << std::endl;
        return 1;
    }
    std::cout << "Archivo .env encontrado en: " << rutaEnv << std::endl;

    // 2. Leer una variable específica
    std::optional<std::string> valor = getEnvVar(rutaEnv, "USER_FILE");

    if (valor) {
        std::cout << "El valor de USER_FILE es: " << *valor << std::endl;
    } else {
        std::cout << "La variable no existe en el archivo .env." << std::endl;
    }


    return 0;
}