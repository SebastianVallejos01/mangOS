#include "mates.hpp"


void menuPoli(){
    std::string opcion;
    do {
        std::cout << "\n--- Cálculo de función polinómica ---\n";
        std::cout << "Ingrese preimagen x. Entrada no numérica para salir: ";
        std::cin >> opcion;
        try {
            std::size_t index = 0;
            double numero = std::stod(opcion, &index);
            if (index != opcion.size()) {
                throw std::invalid_argument("Entrada no válida");
            }
            polinomio(numero);
        } catch (const std::exception&) {
            std::cout << "Entrada no numérica ingresada. Saliendo del cálculo polinómico." << std::endl;
            break;
        }
    } while (true);

}

void polinomio(const double& x){
    std::cout<<"Número ingresado: "<<x<<std::endl;
    std::cout<<"f("<<x<<")= ("<<x<<")^2 + 2*("<<x<<") + 8 = "<<x*x+2*x+8<<std::endl;
}

void menuMatriz();
void productoMatrices();