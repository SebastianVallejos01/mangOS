#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
//#include <windows.h>

// Función para leer y parsear una matriz desde un archivo de texto
std::vector<std::vector<double>> leerMatriz(const std::string& ruta, const char delimitador) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + ruta);
    }

    std::vector<std::vector<double>> matriz;
    std::string linea;

    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        std::vector<double> fila;
        std::stringstream ss(linea);
        std::string valor;

        while (std::getline(ss, valor, delimitador)) {
            // Limpiar espacios en blanco al inicio y final (opcional pero seguro)
            size_t start = valor.find_first_not_of(" \t\r\n");
            size_t end = valor.find_last_not_of(" \t\r\n");
            if (start != std::string::npos && end != std::string::npos) {
                valor = valor.substr(start, end - start + 1);
            }

            if (!valor.empty()) {
                try {
                    fila.push_back(std::stod(valor));
                } catch (const std::exception&) {
                    throw std::runtime_error("Dato no numérico encontrado en la matriz: '" + valor + "'");
                }
            }
        }
        if (!fila.empty()) {
            matriz.push_back(fila);
        }
    }
    archivo.close();

    // Validar que todas las filas tengan la misma longitud (matriz bien formada)
    if (!matriz.empty()) {
        size_t cols = matriz[0].size();
        for (const auto& fila : matriz) {
            if (fila.size() != cols) {
                throw std::runtime_error("La matriz en " + ruta + " no tiene un número uniforme de columnas.");
            }
        }
    }

    return matriz;
}

// Función para imprimir una matriz
void imprimirMatriz(const std::vector<std::vector<double>>& matriz) {
    for (const auto& fila : matriz) {
        std::cout << "[ ";
        for (double val : fila) {
            std::cout << val << " ";
        }
        std::cout << "]" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // SetConsoleOutputCP(CP_UTF8);
    // Validar la cantidad de argumentos
    // ./multiplicador <rutaA> <rutaB> <separador> <username> <profile>
    if (argc != 6) {
        std::cerr << "Error: Uso incorrecto del programa multiplicador." << std::endl;
        std::cerr << "Uso esperado: " << argv[0] << " <ruta_matriz_A> <ruta_matriz_B> <separador> <usuario> <perfil>" << std::endl;
        return 1;
    }

    std::string rutaA = argv[1];
    std::string rutaB = argv[2];
    std::string sepArg = argv[3];
    std::string username = argv[4];
    std::string profile = argv[5];

    // Obtener el delimitador (asumimos el primer carácter del argumento)
    if (sepArg.empty()) {
        std::cerr << "Error: Separador vacío." << std::endl;
        return 1;
    }
    char delimitador = sepArg[0];

    std::cout << "\n========================================" << std::endl;
    std::cout << "      MULTIPLICADOR DE MATRICES NxM       " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Usuario : " << username << std::endl;
    std::cout << "Perfil  : " << profile << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    try {
        // 1. Leer las matrices
        std::vector<std::vector<double>> matrizA = leerMatriz(rutaA, delimitador);
        std::vector<std::vector<double>> matrizB = leerMatriz(rutaB, delimitador);

        if (matrizA.empty() || matrizB.empty()) {
            throw std::runtime_error("Una o ambas matrices están vacías.");
        }

        int filasA = matrizA.size();
        int colsA = matrizA[0].size();
        int filasB = matrizB.size();
        int colsB = matrizB[0].size();

        std::cout << "Matriz A cargada con éxito (" << filasA << "x" << colsA << ")." << std::endl;
        std::cout << "Matriz B cargada con éxito (" << filasB << "x" << colsB << ")." << std::endl;

        // 2. Validar que se puedan multiplicar (columnas de A == filas de B)
        if (colsA != filasB) {
            std::cerr << "\nERROR MATEMÁTICO: No es posible multiplicar las matrices." << std::endl;
            std::cerr << "El número de columnas de la Matriz A (" << colsA << ") debe ser igual al número de filas de la Matriz B (" << filasB << ")." << std::endl;
            return 1;
        }

        std::cout << "Multiplicación posible. Procesando..." << std::endl;

        // 3. Multiplicar matrices
        // Resultado C será de dimensión (filasA x colsB)
        std::vector<std::vector<double>> matrizC(filasA, std::vector<double>(colsB, 0.0));

        for (int i = 0; i < filasA; ++i) {
            for (int j = 0; j < colsB; ++j) {
                for (int k = 0; k < colsA; ++k) {
                    matrizC[i][j] += matrizA[i][k] * matrizB[k][j];
                }
            }
        }

        // 4. Mostrar el resultado
        std::cout << "\nResultado de la Multiplicación (Matriz " << filasA << "x" << colsB << "):" << std::endl;
        imprimirMatriz(matrizC);
        std::cout << "========================================\n" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "\nExcepción atrapada: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
