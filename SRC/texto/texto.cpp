#include "texto.hpp"


void menuPalindromo() {
    std::string entrada;
    std::cout << "\n--- Verificación de Palíndromo ---\n";
    std::cout << "Ingrese una palabra o frase (0 para terminar): ";
    std::getline(std::cin, entrada);

    while (entrada != "0") {
        if (esPalindromo(entrada))
            std::cout << "'" << entrada << "' es un palíndromo.\n";
        else
            std::cout << "'" << entrada << "' no es un palíndromo.\n";

        std::cout << "Ingrese otra palabra o frase (o '0' para terminar): ";
        std::getline(std::cin, entrada);
    }
}

bool esPalindromo(const std::string& entrada){

    std::string palabra;
    palabra.reserve(entrada.size());

    for (unsigned char c : entrada) {
        if (std::isspace(c)) continue;
        palabra.push_back(static_cast<char>(c));
    }

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

void conteoTexto(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir el archivo en la ruta proporcionada.\n";
        return;
    }

    std::string contenido((std::istreambuf_iterator<char>(archivo)), std::istreambuf_iterator<char>());
    archivo.close();

    int numVocales = 0;
    int numConsonantes = 0;
    int numCaracteresEspeciales = 0;
    int numPalabras = 0;
    bool enPalabra = false;
    std::string opcion;

    for (unsigned char c : contenido) {
        if (std::isalnum(c)) {
            char letra = static_cast<char>(std::tolower(c));
            if (std::isalpha(c)) {
                if (letra == 'a' || letra == 'e' || letra == 'i' || letra == 'o' || letra == 'u')
                    numVocales++;
                else
                    numConsonantes++;
            }
            enPalabra = true;
        } else if (std::isspace(c)) {
            if (enPalabra) {
                numPalabras++;
                enPalabra = false;
            }
        } else {
            numCaracteresEspeciales++;
        }
    }

    if (enPalabra) numPalabras++;

    std::cout << "\n--- Resumen de Conteo de Texto ---\n";
    std::cout << "Ruta: " << ruta << "\n";
    std::cout << "Cantidad de vocales: " << numVocales << "\n";
    std::cout << "Cantidad de consonantes: " << numConsonantes << "\n";
    std::cout << "Cantidad de caracteres especiales: " << numCaracteresEspeciales << "\n";
    std::cout << "Cantidad de palabras: " << numPalabras << "\n";
    std::cout << "--------------------------------\n";

    do{
        std::cout << "\nIngrese '0' para salir: ";
        std::getline(std::cin, opcion);
    } while (opcion != "0");
}

void menuConteoTexto() {
    std::string ruta;

    while (true) {
        std::cout << "\n--- Conteo de Texto ---\n";
        std::cout << "Ingrese la ruta del archivo de texto (o '0' para salir): ";
        std::getline(std::cin, ruta);
        if (ruta == "0") {
            break;
        }

        conteoTexto(ruta);

    }
}