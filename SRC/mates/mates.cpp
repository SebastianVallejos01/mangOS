#include "mates.hpp"


void menuPoli();


void polinomio(const int& x){
    std::cout<<"Número ingresado: "<<x<<std::endl;
    std::cout<<"f("<<x<<")= ("<<x<<")^2 + 2*("<<x<<") + 8 = "<<x*x+2*x+8<<std::endl;
}
void polinomio(const double& x){
    std::cout<<"Número ingresado: "<<x<<std::endl;
    std::cout<<"f("<<x<<")= ("<<x<<")^2 + 2*("<<x<<") + 8 = "<<x*x+2*x+8<<std::endl;
}

void menuMatriz();
void productoMatrices();