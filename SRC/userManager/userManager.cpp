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
void menuUsuarios(const std::string& rutaFile, UserList& ListaUsuarios);
void menuPerfiles(const std::string& rutaFile, ProfileList& ListaPerfiles);

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
bool mostrarListaPerfiles(ProfileList& ListaPerfiles);

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
    std::cout<<"Ingrese los permisos que desea darle al perfil";
    std::cout<<"1:Agregar, 2:Enlistar, 3: Eliminar ||(Ingrese '0' para terminar)";
    int permiso;
    do 
    {
        std::cout<<"Ingresar:";
        std::cin>>permiso;
        if (permiso >= 1 && permiso <= 3) 
        {
            bool existe=false;
            for (int valor : newProfile.permisosMenu){
                if (valor==permiso) 
                {
                    existe=true;
                    break;
                }
                
            }
            if (existe) std::cout<<"El permiso ya pertenece al perfil.";
            else newProfile.permisosMenu.push_back(permiso);
        }
        else if (permiso != 0) 
        {
            std::cout << "Permiso invalido." << std::endl;
        }
    }while (permiso!=0);

    newProfile.permisosMenu.push_back(0);

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
bool borraPerfil(const std::string& nombre, const std::string& rutaFile, ProfileList& ListaPerfiles);