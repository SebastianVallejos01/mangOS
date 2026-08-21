#pragma once 
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <optional>

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
    std::vector<int> permisosMenu; //entre 0, 1, 2, 3, 4
};

struct ProfileList{
    std::vector<Profile> profiles;
    bool txtCargado = false;
};



// Métodos / funciones

//Obtener varibales de entorno
std::string getEnvFile(fs::path rutaActual, const std::string& varName = ".env");
std::string limpiarString(const std::string& str);
std::optional<std::string> getEnvVar(const fs::path& rutaEnv, const std::string& clave);

//Submenus
void menuUsuarios(const std::string& rutaFile, UserList& ListaUsuarios);
void menuPerfiles(const std::string& rutaFile, ProfileList& ListaPerfiles);

//Usuarios
//Leer USUARIOS.TXT
bool leeUsuariosTxt(const std::string& rutaFile, UserList& ListaUsuarios);

//Listar usuarios
bool mostrarListaUsuarios(UserList& ListaUsuarios);

//Crear usuario
bool creaUsuario(const std::string& rutaFile, UserList& ListaUsuarios);

//Borrar usuario
bool borraUsuario(int idBorrar, const std::string& rutaFile, UserList& ListaUsuarios);


//Perfiles
//Leer PERFILES.TXT
bool leePerfilesTxt(const std::string& rutaFile, ProfileList& ListaPerfiles);

//Listar perfiles
bool mostrarListaPerfiles(ProfileList& ListaPerfiles);

//Crear perfil
bool creaPerfil(const std::string& rutaFile, ProfileList& ListaPerfiles);

//Borrar perfil
bool borraPerfil(const std::string& nombre, const std::string& rutaFile, ProfileList& ListaPerfiles);