#include "../userManager/userManager.hpp"

int main() {
    fs::path rutaEnv;
    rutaEnv = getEnvFile(fs::current_path(), ".env");
    
    if (rutaEnv.empty()) return 1;
    
    valorEnvPerfil = getEnvVar(rutaEnv, "PERFIL_FILE");
    valorEnvUsuario = getEnvVar(rutaEnv, "USER_FILE");
    
    if (!valorEnvPerfil || !valorEnvUsuario) return 1;

    ProfileList ListaPerfiles;
    UserList ListaUsuarios;
    
    leePerfilesTxt(valorEnvPerfil.value(), ListaPerfiles);
    leeUsuariosTxt(valorEnvUsuario.value(), ListaUsuarios, ListaPerfiles);
    

    menuUserManager(valorEnvUsuario.value(), valorEnvPerfil.value(), ListaUsuarios, ListaPerfiles);
    
    
    return 0;
}