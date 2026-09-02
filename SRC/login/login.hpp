#pragma once
#include "../userManager/userManager.hpp"

/**
 * @brief Variable global que almacena el usuario que inició sesión en la ejecución actual.
 */
inline std::optional<User> loggedUser;

/**
 * @brief Verifica la correctitud del formato de ejecución del programa.
 * @param n Cantidad de argumentos recibidos por el main.
 * @param txt Línea de texto completa recibiba por el main.
 * @return true si el formato es correcto, false sino.
 */
bool validaInicio(const int& n, char* txt[]);

/**
 * @brief Obtiene el nombre de usuario ingresado.
 * @param txt Línea de texto completa recibida por el main.
 * @return String limpio con el nombre de usuario.
 */
std::string getUserName(char* txt[]);

/**
 * @brief Obtiene la contraseña ingresada.
 * @param txt Línea de texto completa recibida por el main.
 * @return String limpio con la contraseña.
 */
std::string getPassWord(char* txt[]);

/**
 * @brief Obtiene la ruta de archivo de texto ingresada por el usuario
 * @param txt Línea de texto completa recibida por el main.
 * @return String limpio con la ruta del archivo.
 */
std::string getUserRuta(char* txt[]);

/**
 * @brief Verifica que las credenciales de inicio de sesión ingresadas sean correctas.
 * @param name Nombre de usuario ingresado.
 * @param pass Contraseña ingresada.
 * @param ListaUsuarios Lista con los usuarios cargados en el sistema.
 * @return Struct User del usuario ingresado si las credenciales son correctas. De lo contrario, std::nullopt.
 */
std::optional<User> validaLogin(const std::string& name, const std::string& pass, const UserList& ListaUsuarios);