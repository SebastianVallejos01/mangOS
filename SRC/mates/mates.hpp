#pragma once
#include <string>
#include <iostream>

/**
 * @brief Muestra el menú para el cálculo de un polinomio y gestiona la interacción con el usuario.
 */
void menuPoli();

/**
 * @brief Calcula el valor de un polinomio en un punto dado
 * @param x El valor en el que se evalúa el polinomio
 */
void polinomio(const double& x);

// Declaraciones previas
struct User;
struct Profile;

/**
 * @brief Muestra el menú para la multiplicación de matrices NxM.
 * Ofrece la opción 0 para volver, 1 para trabajar con los valores del .env y 2 para ingresar las rutas manualmente.
 * @param username Nombre del usuario autenticado (opcional).
 * @param profile Nombre del perfil autenticado (opcional).
 */
void menuMatrices(const std::string& username = "", const std::string& profile = "");

/**
 * @brief Sobrecarga de menuMatrices para interactuar con las estructuras User y Profile.
 * @param loggedUser Estructura del usuario autenticado.
 * @param loggedProfile Estructura del perfil del usuario autenticado.
 */
void menuMatrices(const User& loggedUser, const Profile& loggedProfile);

/**
 * @brief Alias para mantener compatibilidad con invocaciones bajo el nombre menuMatriz.
 */
void menuMatriz(const std::string& username = "", const std::string& profile = "");