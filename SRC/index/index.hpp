#pragma once
#include <queue>
#include <map>
#include <thread>
#include <semaphore.h> // Semáforos POSIX clásicos para OS
#include <mutex>       // Para proteger secciones críticas simples
#include <cctype>
#include "../userManager/userManager.hpp"

namespace fs = std::filesystem;

// 1. Estructura para identificar cada libro
struct Libro {
    int id;
    std::string nombre;
    std::string ruta;
};



// 2. Variables Globales Compartidas (Estado Compartido)
// Nota: Se declaran como 'inline' para compartirlas entre los threads
inline std::queue<Libro> colaTrabajo;
inline bool procesamientoActivo = true; // Flag para avisar a los threads cuándo detenerse

// 3. Estructura del Índice Invertido Global
// Mapa de: Palabra -> Mapa de (ID_Libro -> Frecuencia)
inline std::map<std::string, std::map<int, int>> indiceGlobal;

// 4. Semáforos y Mutex (Secciones Críticas)
inline sem_t sem_elementos_cola; // Contador: ¿Cuántos libros hay en la cola?
inline std::mutex mtx_cola;      // Exclusión mutua: Proteger push/pop de la cola
inline std::mutex mtx_indice;    // Exclusión mutua: Proteger escritura en indiceGlobal

// 5. Estructura de Configuración del Módulo
struct ConfiguracionIndex {
    fs::path rutaLibros;
    fs::path rutaIdLibros;
    fs::path rutaIndice;
    int nThreads;
};

// 6. Declaraciones de Funciones Principales


/**
 * @brief Muestra el submenú para crear el índice invertido y maneja la interacción con el usuario.
 */
void submenuIndex();

/**
 * @brief Lee y valida las variables de entorno necesarias para el índice.
 * @param rutaEnv Ruta del archivo .env resuelto.
 * @return Struct con las rutas y número de threads validados, o std::nullopt si alguna falla.
 */
std::optional<ConfiguracionIndex> cargarConfiguracionIndex(const fs::path& rutaEnv);

/**
 * @brief Inicia todo el proceso: valida entorno, mapea libros, 
 *        crea la cola, lanza los threads y guarda los resultados. (Llamada desde Opción 8)
 */
void crearIndiceInvertido();

/**
 * @brief Ciclo de vida del thread trabajador (Consumidor).
 * @param idThread Identificador numérico del hilo para trazabilidad (opcional).
 */
void workerThread(int idThread);

/**
 * @brief Lee el texto del libro, extrae y limpia las palabras, y actualiza el índice global.
 * @param libro Struct con la información y ruta del libro a procesar.
 */
void procesarLibro(const Libro& libro);