#pragma once 
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <optional>
#include <sstream>

namespace fs = std::filesystem;

//Estructuras

//Usuario
struct User {
    int id;
    std::string nombre;
    std::string username;
    std::string password;
    std::string perfil;
};

//Lista de usuarios
struct UserList{
    std::vector<User> users;
    bool txtCargado = false; //flag que indica si se leyó o no el txt
};

//Perfil
struct Profile{
    std::string name; //ADMIN, GENERAL
    std::vector<int> permisosMenu; //entre 0, 1, 2, 3, 4 (0: Salir, 1: Ingresar/crear, 2:Enlistar, 3:Eliminar, 4:----)
};

struct ProfileList{
    std::vector<Profile> profiles;
    bool txtCargado = false;
};



// Métodos / funciones

//Obtener varibales de entorno
inline std::optional<std::string> valorEnvUsuario;
inline std::optional<std::string> valorEnvPerfil;


std::string getEnvFile(fs::path rutaActual, const std::string& varName = ".env");
std::string limpiarString(const std::string& str);
std::optional<std::string> getEnvVar(const fs::path& rutaEnv, const std::string& clave);

//Menú/Submenus
void menuUserManager(const std::string& rutaUserFile, const std::string& rutaPerfilFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles);
void menuUsuarios(const std::string& rutaFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles);
void menuPerfiles(const std::string& rutaFile, ProfileList& ListaPerfiles);

//Usuarios
//Leer USUARIOS.TXT
bool leeUsuariosTxt(const std::string& rutaFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles);

//Listar usuarios
bool mostrarListaUsuarios(UserList& ListaUsuarios, ProfileList& ListaPerfiles);

//Crear usuario
bool creaUsuario(const std::string& rutaFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles);

//Borrar usuario
bool borraUsuario(int idBorrar, const std::string& rutaFile, UserList& ListaUsuarios);


//Perfiles
//Leer PERFILES.TXT
bool leePerfilesTxt(const std::string& rutaFile, ProfileList& ListaPerfiles);

//Crear automáticamente los perfiles base (ADMIN y GENERAL) si no existen
void autoCrearPerfilesBase(const std::string& rutaFile, ProfileList& ListaPerfiles);

//Listar perfiles
bool mostrarListaPerfiles(ProfileList& ListaPerfiles);

//Crear perfil
bool creaPerfil(const std::string& rutaFile, ProfileList& ListaPerfiles);

//Borrar perfil
bool borraPerfil(const std::string& nombre, const std::string& rutaFile, ProfileList& ListaPerfiles);
