#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iterator>

/**
 * @brief Función que muestra el menú de opciones para verificar si una palabra o frase es un palíndromo.
 */
void menuPalindromo();

/**
 * @brief Función que verifica si una palabra o frase es un palíndromo.
 * @param entrada La cadena de texto a verificar.
 * @return true si es un palíndromo, false en caso contrario.
 */
bool esPalindromo(const std::string& entrada);

/**
 * @brief Función que realiza el conteo de vocales, consonantes, caracteres especiales y palabras en un archivo de texto.
 * @param ruta La ruta del archivo de texto a analizar.
 */
void conteoTexto(const std::string& ruta);

/**
 * @brief Función que muestra el menú de opciones para realizar el conteo de texto en un archivo nuevo, distinto al del login.
 */
void menuConteoTexto();