#pragma once
#include <vector>
#include <string>
#include "../Classes/GameState.hpp" // Ajusta la ruta según tu estructura de carpetas

class Server {
private:
    int server_fd;
    int puerto;
    std::vector<int> sockets_clientes;
    
    GameState estado_juego; // El tablero que administrará todo

    // ==========================================
    // MÉTODOS DE RED INTERNOS
    // ==========================================
    // Envía un texto a un jugador específico
    void enviarMensaje(int cliente_socket, const std::string& mensaje);
    
    // Envía un texto a los 4 jugadores al mismo tiempo
    void enviarATodos(const std::string& mensaje);
    
    // Espera y lee el mensaje que envíe un jugador
    std::string recibirMensaje(int cliente_socket);

    std::string recibirMensajeConControl(int socket_actual);

public:
    // Inicializa el servidor con el puerto deseado
    Server(int puerto_asignado);
    ~Server();

    // Levanta el servidor y abre el puerto
    bool iniciar();

    // ==========================================
    // FASES DEL JUEGO
    // ==========================================
    // Fase 0: Espera hasta tener exactamente 4 conexiones
    void esperarJugadores();

    /// Fase 1: Pide nombres y equipos (1 Coloso, 3 Aventureros)
    bool faseSeleccionEquipo();

    // Fase 2: Pide la clase específica usando ClassManager
    bool faseSeleccionClase();

    // Fase 3: Bucle principal de turnos y combate
    void loopCombate();
};