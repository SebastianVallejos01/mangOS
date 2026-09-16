#pragma once
#include "../userManager/userManager.hpp"
#include "../mates/mates.hpp"
#include "../texto/texto.hpp"

#include <limits>


/**
 * @brief Muestra y gestiona el menú principal de la aplicación para el usuario autenticado.
 * @param loggedUser Usuario actualmente autenticado.
 * @param loggedProfile Perfil activo del usuario autenticado.
 * @param userRuta Ruta del directorio especificado por el usuario.
 * @param rutaUserFile Ruta del archivo que almacena la información de usuarios.
 * @param rutaPerfilFile Ruta del archivo que almacena la información de perfiles.
 * @param ListaUsuarios Lista global de usuarios que puede modificarse durante la sesión.
 * @param ListaPerfiles Lista global de perfiles que puede modificarse durante la sesión.
 */
void mainMenu(const User& loggedUser, const Profile& loggedProfile, const std::string& userRuta, const std::string& rutaUserFile, const std::string& rutaPerfilFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles);