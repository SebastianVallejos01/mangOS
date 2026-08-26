#include "userManager.hpp"


// Métodos / funciones
std::string getEnvFile(fs::path rutaActual, const std::string& varName){
    while (true) {
        // Buscar el archivo en la ruta actual. Si lo encuentra, retorna la ruta completa.
        fs::path rutaPosible = rutaActual / varName;
        if (fs::exists(rutaPosible)) {
            return rutaPosible.string();
        }
        // Si se llega a la raíz y no se encuentra, se detiene la búsqueda.
        if (rutaActual == rutaActual.parent_path()) {
            break;
        }
        rutaActual = rutaActual.parent_path(); // Subir un nivel en el directorio.
    }
    std::cout<<"Error. Archivo '"<<varName<<"' no encontrado."<<std::endl;
    return {};
}

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
void menuUserManager(const std::string& rutaUserFile, const std::string& rutaPerfilFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles){
    int opcion;
    do {
        std::cout << "\n--- mangOS User Manager ---\n";
        std::cout << "1) Gestión de Usuarios\n";
        std::cout << "2) Gestión de Perfiles\n";
        std::cout << "0) Salir\n";
        std::cout << "Opción: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                menuUsuarios(rutaUserFile, ListaUsuarios, ListaPerfiles);
                break;
            case 2:
                menuPerfiles(rutaPerfilFile, ListaPerfiles);
                break;
            case 0:
                break;
            default:
                std::cout << "Opción inválida.\n";
                break;
        }
    } while (opcion != 0);
}
void menuUsuarios(const std::string& rutaFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles){
    int opcion;
    do {
        std::cout << "\n--- GESTIÓN DE USUARIOS ---\n";
        std::cout << "1) Ingresar Usuarios\n";
        std::cout << "2) Listar Usuarios\n";
        std::cout << "3) Eliminar Usuarios\n";
        std::cout << "0) Salir\n";
        std::cout << "Opción: ";
        
        if (!(std::cin >> opcion)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            opcion = -1;
            continue;
        }

        switch (opcion) {
            case 1: {
                creaUsuario(rutaFile, ListaUsuarios, ListaPerfiles);
                break;
            }
            case 2:
                mostrarListaUsuarios(rutaFile, ListaUsuarios, ListaPerfiles);
                break;
            case 3:
                int idBorrar;
                std::cout << "Ingrese el ID del usuario a borrar: ";
                std::cin >> idBorrar;
                borraUsuario(idBorrar, rutaFile, ListaUsuarios);
                break;
            case 0:
                break;
        }
    } while (opcion != 0);
}
void menuPerfiles(const std::string& rutaFile, ProfileList& ListaPerfiles){
    int opcion;
    do {
        std::cout << "\n--- GESTIÓN DE PERFILES ---\n";
        std::cout << "1) Ingresar Perfil\n";
        std::cout << "2) Listar Perfiles\n";
        std::cout << "3) Eliminar Perfil\n";
        std::cout << "0) Salir (Volver)\n";
        std::cout << "Opción: ";
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
                std::cin.ignore(10000, '\n'); //Limpiar buffer
                std::getline(std::cin, nombreBorrar); //Leer con espacios
                for (char& c : nombreBorrar) {
                    c = std::toupper(c);
                }
                borraPerfil(nombreBorrar, rutaFile, ListaPerfiles);
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Opción inválida.\n";
                break;
        }
    } while (opcion != 0);
}

//Usuarios
bool leeUsuariosTxt(const std::string& rutaFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles) {
    // Abrir archivo de usuarios. Si no se puede abrir, retornar false.
    std::ifstream archivo(rutaFile);
    if (!archivo.is_open()) return false;

    ListaUsuarios.users.clear();

    // Leer línea por línea, limpiar espacios y separar por ';'.
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

        // Validación de existencia del perfil del usuario en la lista de perfiles.
        bool perfilValido = false;
        for (const auto& perfil : ListaPerfiles.profiles) {
            if (perfil.name == usuario.perfil) {
                perfilValido = true;
                break;
            }
        }

        // Si el perfil no es válido, asignar "GENERAL" y mostrar advertencia.
        if (!perfilValido) {
            std::cerr << "Advertencia: Usuario '" << usuario.username << "' tiene un perfil inválido. Asignando 'GENERAL' por defecto.\n";
            usuario.perfil = "GENERAL";
        }
        // Agregar usuario a la lista
        ListaUsuarios.users.push_back(usuario);
    }
    // Marcar que el archivo de usuarios fue cargado correctamente.
    ListaUsuarios.txtCargado = true;
    return true;
}

//Listar usuarios
bool mostrarListaUsuarios(const std::string& rutaFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles) {
    bool requiereGuardar = false;

    //Sincronización en memoria: Si un usuario tiene un perfil que ya no existe en la lista de perfiles, se le asigna "GENERAL" y se marca que requiere guardar.
    for (auto& u : ListaUsuarios.users) {
        bool perfilValido = false;
        for (const auto& p : ListaPerfiles.profiles) {
            if (u.perfil == p.name) {
                perfilValido = true;
                break;
            }
        }
        
        //Si el perfil ya no existe en la lista de perfiles, se cambia a GENERAL.
        if (!perfilValido) {
            u.perfil = "GENERAL";
            requiereGuardar = true;
        }
    }

    //Si hubo modificaciones, se actualiza el archivo TXT.
    if (requiereGuardar) {
        std::ofstream file(rutaFile, std::ios::trunc);
        if (file.is_open()) {
            for (const auto& u : ListaUsuarios.users) {
                file << u.id << ";" << u.nombre << ";" << u.username << ";" << u.password << ";" << u.perfil << "\n";
            }
        }
    }
    
    // Mostrar la lista de usuarios
    std::cout << "\n--- LISTA DE USUARIOS ---\n";
    for (const auto& u : ListaUsuarios.users) {
        std::cout << "ID: " << u.id << " | Nombre: " << u.nombre 
                  << " | Username: " << u.username << " | Perfil: " << u.perfil << "\n";
    }
    std::cout << "-------------------------\n";
    return true;
}

bool creaUsuario(const std::string& rutaFile, UserList& ListaUsuarios, ProfileList& ListaPerfiles) {
    User newUser;
    //Establecer ID único para el nuevo usuario
    int maxId = 0;
    for (const auto& u : ListaUsuarios.users) {
        if (u.id > maxId) maxId = u.id;
    }
    newUser.id = maxId + 1;

    //Recibir datos de usuario nuevo
    std::cout << "Ingrese nombre: ";
    std::cin.ignore(10000, '\n');
    std::getline(std::cin, newUser.nombre);
    std::cout << "Ingrese username: ";
    std::getline(std::cin, newUser.username);
    std::cout << "Ingrese password: ";
    std::getline(std::cin, newUser.password);
    std::cout << "Ingrese perfil: ";
    std::getline(std::cin, newUser.perfil);

    // Validación de unicidad del username ingresado
    for (const auto& u : ListaUsuarios.users) {
        if (u.username == newUser.username) {
            std::cout << "Error: El username '" << newUser.username << "' ya existe en el sistema.\n";
            return false; // Cancela la creación
        }
    }

    //Validación de existencia del perfil ingresado
    for (char& c : newUser.perfil) {
        c = std::toupper(c);
    }

    bool perfilExiste = false;
    for (const auto& p : ListaPerfiles.profiles) {
        if (p.name == newUser.perfil) {
            perfilExiste = true;
            break;
        }
    }

    if (!perfilExiste) {
        std::cout << "Error: El perfil '" << newUser.perfil << "' no existe en el sistema.\n";
        return false; // Cancela la creación
    }

    //Guardar usuario en memoria y en archivo. O cancelar la acción
    int opc;
    std::cout << "1) guardar 2) cancelar\n";
    std::cin >> opc;
    if (opc == 1) {
        ListaUsuarios.users.push_back(newUser);
        std::ofstream file(rutaFile, std::ios::app);
        if (file.is_open()) {
            file << newUser.id << ";" << newUser.nombre << ";" << newUser.username << ";" << newUser.password << ";" << newUser.perfil << "\n";
        }
        return true;
    }
    return false;
}

bool borraUsuario(int idBorrar, const std::string& rutaFile, UserList& ListaUsuarios) {
    // Buscar el usuario por ID y eliminarlo si se encuentra
    for (auto it = ListaUsuarios.users.begin(); it != ListaUsuarios.users.end(); ++it) {
        if (it->id == idBorrar) {
            if (it->perfil == "ADMIN") {
                std::cout << "ALERTA: El usuario a eliminar tiene el perfil ADMIN.\n";
            }
            // Confirmación antes de borrar
            int opc;
            std::cout << "1) guardar 2) cancelar\n";
            std::cin >> opc;
            if (opc == 1) {
                ListaUsuarios.users.erase(it);
                std::ofstream file(rutaFile, std::ios::trunc);
                if (file.is_open()) {
                    for (const auto& u : ListaUsuarios.users) {
                        file << u.id << ";" << u.nombre << ";" << u.username << ";" << u.password << ";" << u.perfil << "\n";
                    }
                }
                return true;
            }
            return false;
        }
    }
    // Si no se encuentra el usuario, mostrar mensaje de error
    std::cout << "Usuario no encontrado.\n";
    return false;
}


//Perfiles
bool leePerfilesTxt(const std::string& rutaFile, ProfileList& ListaPerfiles) {
    std::ifstream archivo(rutaFile);
    
    ListaPerfiles.profiles.clear();

    // Leer línea por línea
    if (archivo.is_open()) {
        std::string linea;
        while (std::getline(archivo, linea)) {
            linea = limpiarString(linea);
            if (linea.empty()) continue;

            std::stringstream ss(linea);
            std::string token;
            Profile perfil;

            //Limpiar espacios basura y forzar mayúsculas
            if (std::getline(ss, token, ';')) {
                perfil.name = limpiarString(token);
                for (char& c : perfil.name) {
                    c = std::toupper(c);
                }
            }
            // Leer y validar permisos y convertirlos a enteros
            std::string permisosStr;
            if (std::getline(ss, permisosStr)) {
                std::stringstream ssPermisos(permisosStr);
                std::string permToken;
                while (std::getline(ssPermisos, permToken, ',')) {
                    if (!permToken.empty()) {
                        try {
                            int permiso = std::stoi(permToken);
                            if (permiso >= 0 && permiso <= 4) {
                                perfil.permisosMenu.push_back(permiso);
                            } else {
                                std::cerr << "Advertencia: Permiso '" << permiso << "' ignorado por estar fuera del rango permitido (0-4).\n";
                            }
                        } catch (const std::exception& e) {
                            std::cerr << "Advertencia: Dato corrupto '" << permToken << "' en permisos ignorado.\n";
                        }
                    }
                }
            }
        // Agregar perfil a la lista y cerrar el archivo
            ListaPerfiles.profiles.push_back(perfil);
        }
        archivo.close();
    }
    // Crear automáticamente los perfiles base si no existen y forzar sus permisos originales
    autoCrearPerfilesBase(rutaFile, ListaPerfiles);
    // Marcar que el archivo de perfiles fue cargado correctamente
    ListaPerfiles.txtCargado = true;
    return true;
}

void autoCrearPerfilesBase(const std::string& rutaFile, ProfileList& ListaPerfiles) {
    bool adminExiste = false;
    bool generalExiste = false;
    bool requiereGuardar = false; //Flag que avisa si debemos reescribir el TXT
    
    //Revisar perfiles cargados y forzar permisos

    //Revisar si ADMIN y GENERAL existen y si sus permisos son correctos
    for (auto& p : ListaPerfiles.profiles) { 
        if (p.name == "ADMIN") {
            adminExiste = true;
            // Si alguien alteró el TXT, lo corregimos
            if (p.permisosMenu != std::vector<int>{0, 1, 2, 3, 4}) {
                p.permisosMenu = {0, 1, 2, 3, 4};
                requiereGuardar = true;
            }
        }
        if (p.name == "GENERAL") {
            generalExiste = true;
            if (p.permisosMenu != std::vector<int>{0, 1, 3}) {
                p.permisosMenu = {0, 1, 3};
                requiereGuardar = true;
            }
        }
    }

    //Crear ADMIN si no existe en absoluto
    if (!adminExiste) {
        Profile pAdmin;
        pAdmin.name = "ADMIN";
        pAdmin.permisosMenu = {0, 1, 2, 3, 4}; 
        ListaPerfiles.profiles.push_back(pAdmin);
        requiereGuardar = true;
        std::cout << "Aviso del Sistema: Perfil 'ADMIN' restaurado por defecto.\n";
    }
    
    //Crear GENERAL si no existe en absoluto
    if (!generalExiste) {
        Profile pGeneral;
        pGeneral.name = "GENERAL";
        pGeneral.permisosMenu = {0, 1, 3}; 
        ListaPerfiles.profiles.push_back(pGeneral);
        requiereGuardar = true;
        std::cout << "Aviso del Sistema: Perfil 'GENERAL' restaurado por defecto.\n";
    }

    //Reescribir todos los perfiles si hubo alguna alteración
    if (requiereGuardar) {
        // Usar std::ios::trunc para borrar el archivo viejo y escribir la versión corregida
        std::ofstream archivoOut(rutaFile, std::ios::trunc); 
        if (archivoOut.is_open()) {
            // Guardar todos los perfiles en el archivo recién vaciado
            for (const auto& perfil : ListaPerfiles.profiles) {
                archivoOut << perfil.name << ";";
                for (size_t i = 0; i < perfil.permisosMenu.size(); ++i) {
                    archivoOut << perfil.permisosMenu[i];
                    if (i != perfil.permisosMenu.size() - 1) {
                        archivoOut << ",";
                    }
                }
                archivoOut << "\n";
            }
        }
    }
}

bool mostrarListaPerfiles(ProfileList& ListaPerfiles) {
    std::cout << "\n--- LISTA DE PERFILES ---\n";
    
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

bool creaPerfil(const std::string& rutaFile, ProfileList& ListaPerfiles){

    // Abrir archivo de perfiles en modo append para agregar un nuevo perfil
    std::ofstream archivo(rutaFile, std::ios::app);
    // Validación del archivo
    if (!archivo.is_open()) return false;

    //Crear nuevo perfil
    std::cout<<"Por favor Ingrese los datos para el nuevo perfil"<<std::endl;
    Profile newProfile;
    std::cout<<"Name: ";
    std::cin.ignore(10000, '\n'); //Limpiar buffer
    std::getline(std::cin, newProfile.name); //Leer con espacios
    for (char& c : newProfile.name) 
    {   //Mayusculizar
    c = std::toupper(c);
    }
    bool existe=false;
    do{
        if (existe)
        {
            std::cout<<"Perfil ya existente, Por favor ingrese un nombre diferente:";
            std::cin.ignore(10000, '\n'); //Limpiar buffer
            std::getline(std::cin, newProfile.name); //Leer con espacios
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

        //Permisos
        std::cout << "\nIngrese los permisos que desea darle al perfil:\n";
        std::cout << "1: Ingresar, 2: Listar, 3: Eliminar, 4: Otra opcion futura || (Ingrese '0' para terminar)\n";
        int permiso;
        do 
        {
            std::cout << "Ingresar permiso: ";
            std::cin >> permiso;
            
            // Defensa contra letras
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Error: Por favor ingresa solo numeros.\n";
                permiso = -1;
                continue;
            }
    
            // Validación de permisos
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
                std::cout << "Permiso inválido.\n";
            }
        } while (permiso != 0);

    // newProfile.permisosMenu.push_back(0);

    //Agregar nuevo perfil a listaPerfiles
    int opc;
    std::cout << "1) guardar 2) cancelar\n";
    std::cin >> opc;
    
    if (opc == 1) {
        ListaPerfiles.profiles.push_back(newProfile);

        archivo<<newProfile.name<<";0";
        for (int proceso:newProfile.permisosMenu)
        {
            archivo<<","<<proceso;
        }
        archivo<<std::endl;
        return true;
    }
    return false;
}

bool borraPerfil(const std::string& nombre, const std::string& rutaFile, ProfileList& ListaPerfiles) {
    //Proteger perfiles originales del sistema
    if (nombre == "ADMIN" || nombre == "GENERAL") {
        std::cout << "Error: No se puede eliminar el perfil '" << nombre << "'.\n";
        return false;
    }
    bool encontrado = false;
    auto it = ListaPerfiles.profiles.begin();
    
    //Buscar el perfil de la lista en memoria
    for (; it != ListaPerfiles.profiles.end(); ++it) {
        if (it->name == nombre) {
            encontrado = true;
            break; 
        }
    }
    //Si no se encuentra el perfil, mostrar mensaje de error
    if (!encontrado) {
        std::cout << "Error: No se encontro el perfil '" << nombre << "'.\n";
        return false;
    }

    //Confirmación antes de borrar
    int opc;
    std::cout << "1) guardar 2) cancelar\n";
    std::cin >> opc;
    
    if (opc == 1) {
        ListaPerfiles.profiles.erase(it);
        
        //Reescribir el archivo PERFILES.TXT completo con la lista actualizada
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
    return false;
}