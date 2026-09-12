#pragma once
#include "../userManager/userManager.hpp"
#include "../mates/mates.hpp"
#include "../texto/texto.hpp"

void mainMenu(const User& loggedUser, const std::string& userRuta, const std::string& rutaUserFile, const std::string& rutaPerfilFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles);