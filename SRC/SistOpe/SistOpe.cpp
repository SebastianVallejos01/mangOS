#include "../userManager/userManager.hpp"
#include "../login/login.hpp"

int main(int argc, char* argv[]) {
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
    //if (!loggedUser) return 1;

    
    //Ejecutar módulo de gestión de usuarios y perfiles
    menuUserManager(valorEnvUsuario.value(), valorEnvPerfil.value(), ListaUsuarios, ListaPerfiles);
    
    
    return 0;
}