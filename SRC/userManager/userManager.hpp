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

/**
 * @brief Estructura que representa un usuario en el sistema.
 */
struct User {
    int id;
    std::string nombre;
    std::string username;
    std::string password;
    std::string perfil;
};

/**
 * @brief Estructura que representa una lista de usuarios en el sistema.
 */
struct UserList{
    std::vector<User> users;
    bool txtCargado = false; //flag que indica si se leyó o no el txt
};

/**
 * @brief Estructura que representa un perfil en el sistema, incluyendo su nombre y los permisos asociados al menú.
 */
struct Profile{
    std::string name; //ADMIN, GENERAL
    std::vector<int> permisosMenu; //entre 0, 1, 2, 3, 4 (0: Salir, las demás representan opciones futuras, no relacionadas con las de userManager)
};

/**
 * @brief Estructura que representa una lista de perfiles en el sistema.
 */
struct ProfileList{
    std::vector<Profile> profiles;
    bool txtCargado = false; //flag que indica si se leyó o no el txt
};



// Métodos / funciones

//Variables de entorno

/**
 * @brief Variable global que almacena el valor de la variable de entorno "USER_FILE" si es encontrada en el archivo .env.
 */
inline std::optional<std::string> valorEnvUsuario;

/**
 * @brief Variable global que almacena el valor de la variable de entorno "PERFIL_FILE" si es encontrada en el archivo .env.
 */
inline std::optional<std::string> valorEnvPerfil;


/**
 * @brief Busca un archivo de entorno (.env) en la ruta actual y subiendo directorios hasta la raíz.
 * @param rutaActual La ruta desde donde se inicia la búsqueda.
 * @param varName El nombre del archivo de entorno a buscar.
 * @return La ruta completa del archivo de entorno si es encontrado, de lo contrario una cadena vacía.
 */
std::string getEnvFile(fs::path rutaActual, const std::string& varName = ".env");

/**
 * @brief Elimina los espacios en blanco al inicio y al final de un string, así como el BOM si el archivo es UTF-8.
 * @param str El string a limpiar.
 * @return El string limpio.
 */
std::string limpiarString(const std::string& str);

/**
 * @brief Obtiene el valor de una variable de entorno desde un archivo .env.
 * @param rutaEnv La ruta del archivo .env.
 * @param clave El nombre de la variable de entorno a buscar.
 * @return El valor de la variable de entorno si es encontrada, de lo contrario std::nullopt.
 */
std::optional<std::string> getEnvVar(const fs::path& rutaEnv, const std::string& clave);



//Menú/Submenus
/**
 * @brief Muestra el menú principal del User Manager y permite al usuario navegar entre las opciones de gestión de usuarios y perfiles.
 * @param rutaUserFile La ruta del archivo de usuarios.
 * @param rutaPerfilFile La ruta del archivo de perfiles.
 * @param ListaUsuarios Referencia a la lista de usuarios en memoria.
 * @param ListaPerfiles Referencia a la lista de perfiles en memoria.
 */
void menuUserManager(const std::string& rutaUserFile, const std::string& rutaPerfilFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles);

/**
 * @brief Muestra el submenú de gestión de usuarios, permitiendo listar, crear y borrar usuarios.
 * @param rutaFile La ruta del archivo de usuarios.
 * @param ListaUsuarios Referencia a la lista de usuarios en memoria.
 * @param ListaPerfiles Referencia a la lista de perfiles en memoria.
 */
void menuUsuarios(const std::string& rutaFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles);

/**
 * @brief Muestra el submenú de gestión de perfiles, permitiendo listar, crear y borrar perfiles.
 * @param rutaFile La ruta del archivo de perfiles.
 * @param ListaPerfiles Referencia a la lista de perfiles en memoria.
 */
void menuPerfiles(const std::string& rutaFile, ProfileList& ListaPerfiles);



//Usuarios
/**
 * @brief Lee los usuarios desde un archivo de texto.
 * @param rutaFile La ruta del archivo de usuarios.
 * @param ListaUsuarios Referencia a la lista de usuarios en memoria.
 * @param ListaPerfiles Referencia a la lista de perfiles en memoria.
 * @return true si se lee correctamente, false en caso contrario.
 */
bool leeUsuariosTxt(const std::string& rutaFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles);

/**
 * @brief Muestra la lista de usuarios. Actualiza el archivo de usuarios si algún usuario tiene un perfil que ya no existe en la lista de perfiles, asignándole "GENERAL" por defecto.
 * @param rutaFile La ruta del archivo de usuarios.
 * @param ListaUsuarios Referencia a la lista de usuarios en memoria.
 * @param ListaPerfiles Referencia a la lista de perfiles en memoria.
 * @return true si se muestra correctamente, false en caso contrario.
 */
bool mostrarListaUsuarios(const std::string& rutaFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles);

/**
 * @brief Crea un nuevo usuario.
 * @param rutaFile La ruta del archivo de usuarios.
 * @param ListaUsuarios Referencia a la lista de usuarios en memoria.
 * @param ListaPerfiles Referencia a la lista de perfiles en memoria.
 * @return true si se crea correctamente, false en caso contrario.
 */
bool creaUsuario(const std::string& rutaFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles);

/**
 * @brief Borra un usuario existente.
 * @param idBorrar El ID del usuario a borrar.
 * @param rutaFile La ruta del archivo de usuarios.
 * @param ListaUsuarios Referencia a la lista de usuarios en memoria.
 * @return true si se borra correctamente, false en caso contrario.
 */
bool borraUsuario(int idBorrar, const std::string& rutaFile, UserList& ListaUsuarios);


//Perfiles

/**
 * @brief Lee los perfiles desde un archivo de texto.
 * @param rutaFile La ruta del archivo de perfiles.
 * @param ListaPerfiles Referencia a la lista de perfiles en memoria.
 * @return true si se lee correctamente, false en caso contrario.
 */
bool leePerfilesTxt(const std::string& rutaFile, ProfileList& ListaPerfiles);

/**
 * @brief Crea automáticamente los perfiles base (ADMIN y GENERAL) si no existen y fuerza sus permisos originales.
 * @param rutaFile La ruta del archivo de perfiles.
 * @param ListaPerfiles Referencia a la lista de perfiles en memoria.
 */
void autoCrearPerfilesBase(const std::string& rutaFile, ProfileList& ListaPerfiles);

/**
 * @brief Muestra la lista de perfiles.
 * @param ListaPerfiles Referencia a la lista de perfiles en memoria.
 * @return true si se muestra correctamente, false en caso contrario.
 */
bool mostrarListaPerfiles(ProfileList& ListaPerfiles);

/**
 * @brief Crea un nuevo perfil y lo guarda en el archivo de perfiles.
 * @param rutaFile La ruta del archivo de perfiles.
 * @param ListaPerfiles Referencia a la lista de perfiles en memoria.
 * @return true si se crea correctamente, false en caso contrario.
 */
bool creaPerfil(const std::string& rutaFile, ProfileList& ListaPerfiles);

/**
 * @brief Borra un perfil existente, protegiendo los perfiles originales del sistema (ADMIN y GENERAL).
 * @param nombre El nombre del perfil a borrar.
 * @param rutaFile La ruta del archivo de perfiles.
 * @param ListaPerfiles Referencia a la lista de perfiles en memoria.
 * @return true si se borra correctamente, false en caso contrario.
 */
bool borraPerfil(const std::string& nombre, const std::string& rutaFile, ProfileList& ListaPerfiles);
