#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

// Esta función corre en segundo plano escuchando al servidor
void recibirMensajes(int sock) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_leidos = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_leidos <= 0) {
            std::cout << "\n[SISTEMA] Te has desconectado del servidor.\n";
            exit(0);
        }
        std::cout << buffer;
        std::cout.flush();
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    // 1. Pedir la IP del servidor por pantalla
    std::string ip_servidor;
    std::cout << "Ingresa la IP del servidor (ej. 192.168.1.X o 127.0.0.1): ";
    std::cin >> ip_servidor;
    
    int puerto = 8080; // Asegúrate de que coincida con el puerto del servidor
    
    // 2. Crear el socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error en la creacion del socket\n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(puerto); 

    // 3. Usar la IP que ingresó el usuario
    if (inet_pton(AF_INET, ip_servidor.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Direccion IP invalida\n";
        return -1;
    }

    // 4. Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Fallo en la conexion. ¿Encendiste el servidor y la IP es correcta?\n";
        return -1;
    }

    std::cout << "[SISTEMA] Conectado con exito al servidor.\n";

    // 5. Iniciar el hilo para escuchar mensajes en tiempo real
    std::thread hilo_recepcion(recibirMensajes, sock);
    hilo_recepcion.detach();

    // 6. Bucle principal para enviar comandos
    std::string mensaje;
    // Limpiamos el buffer del cin anterior
    std::cin.ignore(10000, '\n');
    
    while (std::getline(std::cin, mensaje)) {
        if (mensaje == "salir") break;
        send(sock, mensaje.c_str(), mensaje.length(), 0);
    }

    close(sock);
    return 0;
}