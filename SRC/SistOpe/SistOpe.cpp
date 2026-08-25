#include "../userManager/userManager.hpp"

int main() {
    fs::path rutaEnv;
    rutaEnv = getEnvFile(fs::current_path(), ".env");
    
    if (rutaEnv.empty()) return 1;
    
    valorEnvUsuario = getEnvVar(rutaEnv, "USER_FILE");
    valorEnvPerfil = getEnvVar(rutaEnv, "PERFIL_FILE");
    
    if (!valorEnvUsuario || !valorEnvPerfil) return 1;

    UserList ListaUsuarios;
    ProfileList ListaPerfiles;

    leeUsuariosTxt(valorEnvUsuario.value(), ListaUsuarios);
    leePerfilesTxt(valorEnvPerfil.value(), ListaPerfiles);

    menuUserManager(valorEnvUsuario.value(), valorEnvPerfil.value(), ListaUsuarios, ListaPerfiles);
    
    
    return 0;
}