#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <thread>
#include <semaphore.h> // Semáforos POSIX clásicos para OS
#include <mutex>       // Para proteger secciones críticas simples
#include <filesystem>
#include <fstream>
#include <optional>

namespace fs = std::filesystem;

// 1. Estructura para identificar cada libro
struct Libro {
    int id;
    std::string nombre;
    std::string ruta;
};

// 2. Variables Globales Compartidas (Estado Compartido)
// Nota: Se declaran como 'inline' (C++17) o se usa 'extern' para compartirlas entre los threads
inline std::queue<Libro> colaTrabajo;
inline bool procesamientoActivo = true; // Flag para avisar a los threads cuándo detenerse

// 3. Estructura del Índice Invertido Global
// Mapa de: Palabra -> Mapa de (ID_Libro -> Frecuencia)
inline std::map<std::string, std::map<int, int>> indiceGlobal;

// 4. Semáforos y Mutex (Secciones Críticas)
inline sem_t sem_elementos_cola; // Contador: ¿Cuántos libros hay en la cola?
inline std::mutex mtx_cola;      // Exclusión mutua: Proteger push/pop de la cola
inline std::mutex mtx_indice;    // Exclusión mutua: Proteger escritura en indiceGlobal

// 5. Firmas de Funciones Principales
/**
 * @brief Inicia todo el proceso: lee variables de entorno, mapea libros, 
 *        crea la cola, lanza los threads y guarda los resultados. (Llamada desde Opción 8)
 */
void crearIndiceInvertido();

/**
 * @brief Ciclo de vida del thread trabajador.
 */
void workerThread(int idThread);

/**
 * @brief Limpia el texto, cuenta palabras y actualiza el índice global.
 */
void procesarLibro(const Libro& libro);