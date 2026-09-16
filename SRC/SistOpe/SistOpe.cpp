#include "../userManager/userManager.hpp"
#include "../login/login.hpp"
#include "../mainMenu/mainMenu.hpp"
#include "../mates/mates.hpp"
#include "../texto/texto.hpp"
//#include <windows.h>

int main(int argc, char* argv[]) {
    // SetConsoleOutputCP(CP_UTF8);
    //Validar ejecución del sistema y recibir datos de usuario
    if (!validaInicio(argc, argv)) return 1;
    
    //Si se ejecutó con el formato correcto, lee los argumentos
    std::string userName=getUserName(argv);
    std::string passWord=getPassWord(argv);
    std::string userRuta=getUserRuta(argv);

    // Cargar data
    // Obtener la ruta del archivo .env
    std::cout<<"Cargando archivos del sistema..."<<std::endl;
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
    std::cout<<"Archivos cargados con éxito.\n"<<std::endl;
    

    //Validación de credenciales
    loggedUser = validaLogin(userName, passWord, ListaUsuarios);
    if (!loggedUser) return 1;

    // Obtener el perfil del usuario que ha iniciado sesión
    auto loggedProfile = getPerfil(ListaPerfiles, loggedUser->perfil);
    if (!loggedProfile) return 1;

    mainMenu(*loggedUser, *loggedProfile, userRuta, valorEnvUsuario.value(), valorEnvPerfil.value(), ListaUsuarios, ListaPerfiles);
    
    //menuUserManager(valorEnvUsuario.value(), valorEnvPerfil.value(), ListaUsuarios, ListaPerfiles);
    return 0;
}