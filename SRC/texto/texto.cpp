#include "texto.hpp"
#include "../userManager/userManager.hpp"

#include <algorithm>
#include <cctype>

bool esPalindromo(const std::string& entrada){
    std::string palabra = limpiarString(entrada);

    std::transform(palabra.begin(), palabra.end(), palabra.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    if (palabra.empty()) return false;
    if (palabra.length() == 1) return true;

    int init = 0;
    int fin = palabra.length() - 1;

    while (init <= fin) {
        if (palabra.at(init) != palabra.at(fin)) return false;
        init++;
        fin--;
    }
    return true;
}