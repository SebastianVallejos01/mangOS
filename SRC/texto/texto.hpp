#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iterator>

void menuPalindromo();
bool esPalindromo(const std::string& entrada);

void conteoTexto(const std::string& ruta);

void menuConteoTexto();