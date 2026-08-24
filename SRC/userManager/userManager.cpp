#include "userManager.hpp"
#include <sstream>


// Métodos / funciones

//Obtener varibales de entorno
std::string getEnvFile(fs::path rutaActual, const std::string& varName){
    while (true) {
        fs::path rutaPosible = rutaActual / varName;
        if (fs::exists(rutaPosible)) {
            return rutaPosible;
        }
        // Si se llega a la raíz y no se encuentra, se detiene la búsqueda
        if (rutaActual == rutaActual.parent_path()) {
            break;
        }
        rutaActual = rutaActual.parent_path(); // Sube un nivel
    }
    std::cout<<"Error. Archivo '"<<varName<<"' no encontrado."<<std::endl;
    return {};
}
// Función auxiliar para trimear string
std::string limpiarString(const std::string& str) {
    // \xEF\xBB\xBF limpia el BOM si el archivo es UTF-8
    size_t inicio = str.find_first_not_of(" \t\r\n\xEF\xBB\xBF");
    if (inicio == std::string::npos) return ""; // El string era puro espacio
    
    size_t fin = str.find_last_not_of(" \t\r\n");
    return str.substr(inicio, fin - inicio + 1);
}

std::optional<std::string> getEnvVar(const fs::path& rutaEnv, const std::string& clave) {
    std::ifstream archivo(rutaEnv);
    
    // Validación del archivo
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo .env en la ruta: " << rutaEnv << "\n";
        return std::nullopt;
    }

    std::string linea;
    std::string claveLimpia = limpiarString(clave); // Por si la clave contiene espacios
    while (std::getline(archivo, linea)) {
        // Limpiar línea completa
        linea = limpiarString(linea);

        // Ignorar líneas vacías o comentarios
        if (linea.empty() || linea[0] == '#') continue;

        size_t posicionSimbolo = linea.find('=');
        if (posicionSimbolo != std::string::npos) {
            // Extraer y limpiar la clave y el valor encontrado
            std::string claveEncontrada = limpiarString(linea.substr(0, posicionSimbolo));
            std::string valorEncontrado = limpiarString(linea.substr(posicionSimbolo + 1));

            if (claveEncontrada == claveLimpia) {
                return valorEncontrado; // Retornar valor
            }
        }
    }
    std::cout<<"Error. Valor de '"<<clave<<"' no encontrado."<<std::endl;
    return std::nullopt;
}

//Submenus
void menuUsuarios(const std::string& rutaFile, UserList& ListaUsuarios) {
    // Molde temporal para que no tire error al compilar
    std::cout << "\n(por implementar)\n";
}
void menuPerfiles(const std::string& rutaFile, ProfileList& ListaPerfiles) {
    int opcion;
    do {
        std::cout << "\n--- GESTION DE PERFILES ---\n";
        std::cout << "1) Ingresar Perfil\n";
        std::cout << "2) Listar Perfiles\n";
        std::cout << "3) Eliminar Perfil\n";
        std::cout << "0) Salir (Volver)\n";
        std::cout << "Opcion: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                creaPerfil(rutaFile, ListaPerfiles);
                break;
            case 2:
                mostrarListaPerfiles(ListaPerfiles);
                break;
            case 3: {
                std::string nombreBorrar;
                std::cout << "Nombre del perfil a borrar: ";
                std::cin >> nombreBorrar;
                for (char& c : nombreBorrar) {
                    c = std::toupper(c);
                }
                borraPerfil(nombreBorrar, rutaFile, ListaPerfiles);
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Opcion invalida.\n";
                break;
        }
    } while (opcion != 0);
}
//Usuarios
//Leer USUARIOS.TXT
bool leeUsuariosTxt(const std::string& rutaFile, UserList& ListaUsuarios) {
    std::ifstream archivo(rutaFile);
    if (!archivo.is_open()) return false;

    ListaUsuarios.users.clear();

    std::string linea;
    while (std::getline(archivo, linea)) {
        linea = limpiarString(linea);
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string token;
        User usuario;

        if (std::getline(ss, token, ';')) usuario.id = std::stoi(token);
        if (std::getline(ss, token, ';')) usuario.nombre = token;
        if (std::getline(ss, token, ';')) usuario.username = token;
        if (std::getline(ss, token, ';')) usuario.password = token;
        if (std::getline(ss, token, ';')) usuario.perfil = token;

        ListaUsuarios.users.push_back(usuario);
    }
    
    ListaUsuarios.txtCargado = true;
    return true;
}

//Listar usuarios
bool mostrarListaUsuarios(UserList& ListaUsuarios);

//Crear usuario
bool creaUsuario(const std::string& rutaFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles);

//Borrar usuario
bool borraUsuario(int idBorrar, const std::string& rutaFile, UserList& ListaUsuarios);


//Perfiles
//Leer PERFILES.TXT
bool leePerfilesTxt(const std::string& rutaFile, ProfileList& ListaPerfiles) {
    std::ifstream archivo(rutaFile);
    if (!archivo.is_open()) return false;

    ListaPerfiles.profiles.clear();

    std::string linea;
    while (std::getline(archivo, linea)) {
        linea = limpiarString(linea);
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string token;
        Profile perfil;

        if (std::getline(ss, token, ';')) {
            perfil.name = token;
        }
        
        std::string permisosStr;
        if (std::getline(ss, permisosStr)) {
            std::stringstream ssPermisos(permisosStr);
            std::string permToken;
            while (std::getline(ssPermisos, permToken, ',')) {
                if (!permToken.empty()) {
                    perfil.permisosMenu.push_back(std::stoi(permToken));
                }
            }
        }

        ListaPerfiles.profiles.push_back(perfil);
    }
    
    ListaPerfiles.txtCargado = true;
    return true;
}

//Listar perfiles
bool mostrarListaPerfiles(ProfileList& ListaPerfiles) {
    std::cout << "\n--- LISTA DE PERFILES ---\n";
    if (ListaPerfiles.profiles.empty()) {
        std::cout << "No hay perfiles registrados en el sistema.\n";
        return false;
    }
    
    for (const auto& perfil : ListaPerfiles.profiles) {
        std::cout << "Perfil: " << perfil.name << " | Permisos Menu: ";
        for (size_t i = 0; i < perfil.permisosMenu.size(); ++i) {
            std::cout << perfil.permisosMenu[i];
            if (i != perfil.permisosMenu.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "-------------------------\n";
    return true;
}

//Crear perfil
bool creaPerfil(const std::string& rutaFile, ProfileList& ListaPerfiles){
    std::ofstream archivo(rutaFile, std::ios::app);
    if (!archivo.is_open()) return false;


// ----------------------CREAR NUEVO PERFIL----------------------
        //---------------------Nombre
    std::cout<<"Por favor Ingrese los datos para el nuevo perfil"<<std::endl;
    Profile newProfile;
    std::cout<<"Name: ";
    std::cin>>newProfile.name;
    for (char& c : newProfile.name) 
    {   //Mayusculizar
    c = std::toupper(c);
    }
    bool existe=false;
    do{
        if (existe)
        {
            std::cout<<"Perfil ya existente, Por favor ingrese un nombre diferente:";
            std::cin>>newProfile.name;
            for (char& c : newProfile.name)
            {
                c = std::toupper(c);
            }
        }
        existe=false;
        for (Profile perfil: ListaPerfiles.profiles)
        {
            if (perfil.name==newProfile.name)
            {
                existe=true;
                break;
            }
        }
    }while (existe);

        //------------Permisos
        std::cout << "\nIngrese los permisos que desea darle al perfil:\n";
        std::cout << "1: Ingresar, 2: Listar, 3: Eliminar, 4: Otra opcion futura || (Ingrese '0' para terminar)\n";
        int permiso;
        do 
        {
            std::cout << "Ingresar permiso: ";
            std::cin >> permiso;
            
            // Defensa contra letras (que vimos antes)
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Error: Por favor ingresa solo numeros.\n";
                permiso = -1;
                continue;
            }
    
            // AQUI ESTA EL CAMBIO: Ahora permite hasta el 4
            if (permiso >= 1 && permiso <= 4) 
            {
                bool existe = false;
                for (int valor : newProfile.permisosMenu){
                    if (valor == permiso) 
                    {
                        existe = true;
                        break;
                    }
                }
                if (existe) std::cout << "El permiso ya pertenece al perfil.\n";
                else newProfile.permisosMenu.push_back(permiso);
            }
            else if (permiso != 0) 
            {
                std::cout << "Permiso invalido.\n";
            }
        } while (permiso != 0);

    // newProfile.permisosMenu.push_back(0);

//-----------Adicion a la lista y el TXT--------------------------
    //----------------Agregar nuevo perfil a listaPerfiles

    ListaPerfiles.profiles.push_back(newProfile);
    
    //---------------Agregarlo al TXT

    archivo<<newProfile.name<<";0";
    for (int proceso:newProfile.permisosMenu)
    {
        archivo<<","<<proceso;
    }
    archivo<<std::endl;

    return true;
}

//Borrar perfil
bool borraPerfil(const std::string& nombre, const std::string& rutaFile, ProfileList& ListaPerfiles) {
    bool encontrado = false;
    
    // 1. Buscar y eliminar el perfil de la lista en memoria
    for (auto it = ListaPerfiles.profiles.begin(); it != ListaPerfiles.profiles.end(); ) {
        if (it->name == nombre) {
            it = ListaPerfiles.profiles.erase(it);
            encontrado = true;
            break; 
        } else {
            ++it;
        }
    }

    if (!encontrado) {
        std::cout << "Error: No se encontro el perfil '" << nombre << "'.\n";
        return false;
    }

    // 2. Reescribir el archivo PERFILES.TXT completo con la lista actualizada
    // std::ios::trunc limpia el archivo antes de empezar a escribir
    std::ofstream archivo(rutaFile, std::ios::trunc); 
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir el archivo para guardar los cambios.\n";
        return false;
    }

    for (const auto& perfil : ListaPerfiles.profiles) {
        archivo << perfil.name << ";";
        
        // Escribimos los permisos separados por coma
        for (size_t i = 0; i < perfil.permisosMenu.size(); ++i) {
            archivo << perfil.permisosMenu[i];
            if (i != perfil.permisosMenu.size() - 1) {
                archivo << ",";
            }
        }
        archivo << "\n";
    }
    
    archivo.close();
    std::cout << "Perfil '" << nombre << "' eliminado correctamente de la memoria y del archivo.\n";
    return true;
}