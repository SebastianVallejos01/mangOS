#include "login.hpp"

bool validaInicio(const int& n, char* txt[]){
    if (n==7) 
        if (limpiarString(txt[1])== "-u" && limpiarString(txt[3])== "-p" && limpiarString(txt[5])== "-f") return true;
    std::cout<<"Error. Debe ingresar './mangOS -u <usuario> -p <contraseña> -f <ruta archivo>'"<<std::endl;
    return false;
}
std::string getUserName(char* txt[]){
    return limpiarString(txt[2]);
}
std::string getPassWord(char* txt[]){
    return limpiarString(txt[4]);
}
std::string getUserRuta(char* txt[]){
    return limpiarString(txt[6]);
}

std::optional<User> validaLogin(const std::string& name, const std::string& pass, const UserList& ListaUsuarios){
    bool usuarioExiste=false;
    User usuario;
    for (User usu: ListaUsuarios.users){
        if (usu.username==name){
            usuarioExiste=true;
            usuario=usu;
            break;
        }
    }
    if (usuarioExiste)
        if (usuario.password==pass) return usuario;
    
    std::cout<<"Error. Credenciales incorrectas."<<std::endl;
    return std::nullopt;
}