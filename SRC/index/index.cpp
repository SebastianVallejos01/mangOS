#include "index.hpp"

void submenuIndex(){
    int opcion;
    std::cout << "¿Seguro de que desea crear el índice invertido?" << std::endl;
    std::cout << "Esta operación puede tardar dependiendo de la cantidad de libros y el tamaño de los mismos." << std::endl;
    std::cout << "1. Sí, crear índice invertido" << std::endl;
    std::cout << "0. No, volver al menú principal" << std::endl;
    std::cin >> opcion;
    if (opcion == 1) {
        crearIndiceInvertido();
    } else if (opcion==0) {
        std::cout << "Operación cancelada. Volviendo al menú principal..." << std::endl;
    }
}



std::optional<ConfiguracionIndex> cargarConfiguracionIndex(const fs::path& rutaEnv) {
    ConfiguracionIndex config;

    // Obtener y validar ruta de la carpeta de libros
    auto varLibros = getEnvVar(rutaEnv, "LIBROS");
    if (!varLibros) return std::nullopt;
    config.rutaLibros = varLibros.value();
    
    // Comprobar que la carpeta exista y sea directorio
    if (!fs::exists(config.rutaLibros) || !fs::is_directory(config.rutaLibros)) {
        std::cerr << "Error: La carpeta de libros '" << config.rutaLibros.string() << "' no existe o no es un directorio válido.\n";
        return std::nullopt;
    }

    // Obtener ruta del archivo de IDs de libros
    auto varIdLibros = getEnvVar(rutaEnv, "ID_LIBROS");
    if (!varIdLibros) return std::nullopt;
    config.rutaIdLibros = varIdLibros.value();

    // Obtener ruta del archivo de salida del índice
    auto varIndexLibros = getEnvVar(rutaEnv, "INDEX_LIBROS");
    if (!varIndexLibros) return std::nullopt;
    config.rutaIndice = varIndexLibros.value();

    // Obtener y validar el número de threads
    auto varThreads = getEnvVar(rutaEnv, "N_THREADS");
    if (!varThreads) return std::nullopt;
    
    try {
        config.nThreads = std::stoi(varThreads.value());
        if (config.nThreads <= 0) {
            std::cerr << "Error: N_THREADS debe ser un número entero mayor a 0.\n";
            return std::nullopt;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: El valor de N_THREADS en el .env no es un número válido.\n";
        return std::nullopt;
    }

    return config;
}



void crearIndiceInvertido() {
    std::cout << "\n--- INICIANDO CREACIÓN DE ÍNDICE INVERTIDO ---\n";

    // Cargar y validar variables de entorno
    // Pasamos la ruta actual para buscar el .env dinámicamente
    auto configOpt = cargarConfiguracionIndex(getEnvFile(fs::current_path(), ".env"));
    if (!configOpt) {
        std::cout << "Operación cancelada debido a errores de configuración.\n";
        return; 
    }
    ConfiguracionIndex config = configOpt.value();

    // Limpiar estructuras globales por si se ejecuta múltiples veces en la misma sesión
    while (!colaTrabajo.empty()) {
        colaTrabajo.pop();
    }
    indiceGlobal.clear();

    // Abrir archivo de mapeo ID;Nombre
    std::ofstream archivoIds(config.rutaIdLibros, std::ios::trunc);
    if (!archivoIds.is_open()) {
        std::cout << "Error: No se pudo crear el archivo " << config.rutaIdLibros.string() << "\n";
        return;
    }

    // Leer directorio, poblar la cola de trabajo y el archivo de IDs
    int contadorId = 1;
    for (const auto& entry : fs::directory_iterator(config.rutaLibros)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            Libro nuevoLibro;
            nuevoLibro.id = contadorId;
            nuevoLibro.nombre = entry.path().filename().string();
            nuevoLibro.ruta = entry.path().string();

            colaTrabajo.push(nuevoLibro); // Encolar para los threads
            archivoIds << nuevoLibro.id << ";" << nuevoLibro.nombre << "\n"; // Escribir en txt
            
            contadorId++;
        }
    }
    archivoIds.close();

    int totalLibros = colaTrabajo.size();
    if (totalLibros == 0) {
        std::cout << "Aviso: No se encontraron archivos .txt en " << config.rutaLibros.string() << "\n";
        return;
    }
    std::cout << "Se encontraron " << totalLibros << " libros. Iniciando " << config.nThreads << " threads...\n";

    // Inicializar Semáforo: Su valor inicial es la cantidad de libros en la cola
    sem_init(&sem_elementos_cola, 0, totalLibros);

    // Lanzar Threads Trabajadores
    std::vector<std::thread> trabajadores;
    for (int i = 0; i < config.nThreads; ++i) {
        trabajadores.push_back(std::thread(workerThread, i + 1));
    }

    // Barrera de Sincronización: El main thread espera a que todos terminen
    for (auto& t : trabajadores) {
        if (t.joinable()) {
            t.join();
        }
    }

    // Construir el archivo de Índice Invertido final
    std::cout << "Threads finalizados. Escribiendo indice en disco...\n";
    std::ofstream archivoIndice(config.rutaIndice, std::ios::trunc);
    if (!archivoIndice.is_open()) {
        std::cout << "Error: No se pudo crear el archivo " << config.rutaIndice.string() << "\n";
    } else {
        // Recorrer el mapa global: <Palabra, Mapa<IdLibro, Frecuencia>>
        for (const auto& parPalabra : indiceGlobal) {
            archivoIndice << parPalabra.first; // Escribir la palabra
            
            // Recorrer los libros en los que aparece esa palabra
            for (const auto& parLibro : parPalabra.second) {
                archivoIndice << ";(" << parLibro.first << "," << parLibro.second << ")";
            }
            archivoIndice << "\n";
        }
        archivoIndice.close();
        std::cout << "¡Exito! Indice invertido generado en: " << config.rutaIndice.string() << "\n";
    }

    // Destruir el semáforo al terminar de usarlo
    sem_destroy(&sem_elementos_cola);
}



void workerThread([[maybe_unused]]int idThread) {
    while (true) {
        // Uso del Semáforo
        // sem_trywait intenta decrementar el semáforo. Si su valor es 0 (no quedan libros), 
        // retorna distinto de 0 y el ciclo se rompe, permitiendo que el hilo finalice de forma segura
        if (sem_trywait(&sem_elementos_cola) != 0) {
            break; 
        }

        // Sección Crítica 1: Extracción de la cola
        mtx_cola.lock();
        Libro libroActual = colaTrabajo.front();
        colaTrabajo.pop();
        mtx_cola.unlock();

        //descomentar para debug
        //std::cout << "[Thread " << idThread << "] Procesando: " << libroActual.nombre << "\n";

        // Procesamiento paralelo (Fuera de los mutex para no bloquear a otros hilos)
        procesarLibro(libroActual);
    }
}

void procesarLibro(const Libro& libro) {
    std::ifstream archivo(libro.ruta);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el libro " << libro.nombre << "\n";
        return;
    }

    // Mapa local para contar las palabras de este libro específico
    // Hacerlo local evita bloquear el índice global durante la lectura del disco
    std::map<std::string, int> conteoLocal;
    std::string palabra;

    // Lectura palabra por palabra
    while (archivo >> palabra) {
        std::string palabraLimpia = "";
        
        // Limpieza de puntuación y conversión a minúsculas
        for (char c : palabra) {
            if (std::isalnum(c)) {
                palabraLimpia += std::tolower(c);
            }
        }

        if (!palabraLimpia.empty()) {
            conteoLocal[palabraLimpia]++; // Incrementa la frecuencia local de la palabra
        }
    }
    archivo.close();

    // Sección Crítica 2: Volcado de resultados al Índice Global
    // Una vez terminado el libro completo, se bloquea el índice global para actualizarlo
    mtx_indice.lock();
    for (const auto& par : conteoLocal) {
        // par.first = palabra, par.second = frecuencia
        // Inserción en la estructura: std::map<std::string, std::map<int, int>>
        indiceGlobal[par.first][libro.id] += par.second; 
    }
    mtx_indice.unlock();
}