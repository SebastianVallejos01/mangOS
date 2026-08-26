#include "../userManager/userManager.hpp"

int main() {
    // Obtener la ruta del archivo .env
    fs::path rutaEnv;
    rutaEnv = getEnvFile(fs::current_path(), ".env");
    // Validar si se encontró el archivo .env
    if (rutaEnv.empty()) return 1;
    
    // Obtener las variables de entorno necesarias
    valorEnvPerfil = getEnvVar(rutaEnv, "PERFIL_FILE");
    valorEnvUsuario = getEnvVar(rutaEnv, "USER_FILE");
    // Validar si se encontraron las variables de entorno
    if (!valorEnvPerfil || !valorEnvUsuario) return 1;

    // Crear listas de perfiles y usuarios
    ProfileList ListaPerfiles;
    UserList ListaUsuarios;

    // Leer los archivos de perfiles y usuarios
    leePerfilesTxt(valorEnvPerfil.value(), ListaPerfiles);
    leeUsuariosTxt(valorEnvUsuario.value(), ListaUsuarios, ListaPerfiles);
    
    //Ejecutar módulo de gestión de usuarios y perfiles
    menuUserManager(valorEnvUsuario.value(), valorEnvPerfil.value(), ListaUsuarios, ListaPerfiles);
    
    
    return 0;
}