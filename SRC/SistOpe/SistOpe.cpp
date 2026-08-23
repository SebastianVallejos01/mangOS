#include "userManager/userManager.hpp"

int main(){
    fs::path rutaEnv;
    
    rutaEnv = getEnvFile(fs::current_path(), ".env");
    
    if (rutaEnv.empty())
        return 1;
    else {
        valorEnvUsuario = getEnvVar(rutaEnv, "USER_FILE");
        valorEnvPerfil =getEnvVar(rutaEnv, "PERFIL_FILE");
    }
    if (!valorEnvUsuario && !valorEnvPerfil) return 1;

    
    return 0;
}