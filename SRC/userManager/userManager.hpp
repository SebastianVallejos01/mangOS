#include <string>
#include <vector>

//Usuario
struct User {
    int id;
    std::string nombre;
    std::string username;
    std::string password;
    std::string perfil;
};

//Lista de usuarios
struct UserList{
    std::vector<User> users;
    bool txtCargado = false; //flag que indica si se leyó o no el txt
};

//Perfil
struct Profile{
    std::string name; //ADMIN, GENERAL
    std::vector<int> permisosMenu; //entre 0, 1, 2, 3, 4
};

struct ProfileList{
    std::vector<Profile> profiles;
    bool txtCargado;
};