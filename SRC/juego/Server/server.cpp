#include "server.hpp"
#include "../Classes/ClassManager.hpp"
#include "../Metodo/time.hpp"
#include <iostream>
#include <fstream> // AÑADIDO: Necesario para leer y escribir archivos
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib> 
#include <csignal> 
#include "../../userManager/userManager.hpp"

// ==========================================
// FUNCIÓN AUXILIAR PARA GUARDAR LOGS
// ==========================================
void guardarLog(const std::string& mensaje) {
    // std::ios::app permite añadir texto sin borrar lo anterior.
    // La carpeta debe existir previamente.
    auto rutalog = getEnvVar(getEnvFile(fs::current_path(), ".env"), "LOG_FILE");

    if (!rutalog) {
        std::cerr << "[ERROR] No se pudo obtener la ruta del archivo de log desde el .env" << std::endl;
        return;
    }
    std::ofstream archivo(*rutalog, std::ios::app);
    if (archivo.is_open()) {
        archivo << mensaje << "\n";
        archivo.close();
    } else {
        std::cerr << "[ERROR] No se pudo escribir en el archivo LOG.TXT" << std::endl;
    }
}

// ==========================================
// CONSTRUCTOR Y DESTRUCTOR
// ==========================================
Server::Server(int puerto_asignado) {
    puerto = puerto_asignado;
    server_fd = -1;
}

Server::~Server() {
    // Cerramos todos los sockets al destruir el servidor para liberar los puertos
    for (int socket_cliente : sockets_clientes) {
        close(socket_cliente);
    }
    if (server_fd != -1) {
        close(server_fd);
    }
}

// ==========================================
// MÉTODOS DE RED INTERNOS
// ==========================================

void Server::enviarMensaje(int cliente_socket, const std::string& mensaje) {
    if (cliente_socket <= 0) return; // Ignorar sockets cerrados
    std::string msj_formateado = mensaje + "\n";
    send(cliente_socket, msj_formateado.c_str(), msj_formateado.length(), MSG_NOSIGNAL);
}

void Server::enviarATodos(const std::string& mensaje) {
    for (int socket_cliente : sockets_clientes) {
        enviarMensaje(socket_cliente, mensaje);
    }
}

std::string Server::recibirMensajeConControl(int socket_actual) {
    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        int max_sd = 0;

        // Añadimos a la lista de vigilancia todos los sockets activos
        for (int sd : sockets_clientes) {
            if (sd > 0) {
                FD_SET(sd, &readfds);
                if (sd > max_sd) max_sd = sd;
            }
        }

        int actividad = select(max_sd + 1, &readfds, nullptr, nullptr, nullptr);
        if (actividad < 0) continue;

        // 1. Revisar si ALGUIEN MÁS se desconectó mientras el jugador actual piensa
        for (size_t i = 0; i < sockets_clientes.size(); i++) {
            int sd = sockets_clientes[i];
            if (sd > 0 && sd != socket_actual && FD_ISSET(sd, &readfds)) {
                char buffer[1];
                int valread = recv(sd, buffer, 1, MSG_PEEK);
                if (valread <= 0) {
                    // ¡Se desconectó un jugador pasivo!
                    Player& des = estado_juego.getJugador(i);
                    if (des.estaVivo()) {
                        des.recibirDano(99999); // Eliminación instantánea
                        enviarATodos("SISTEMA| ¡ALERTA! " + des.getNombre() + " se ha desconectado de la partida y ha sido ELIMINADO.");
                    }
                    close(sd);
                    sockets_clientes[i] = -1; // Desactivar socket
                }
            }
        }

        // 2. Revisar si el jugador del turno actual respondió o se desconectó
        if (FD_ISSET(socket_actual, &readfds)) {
            return recibirMensaje(socket_actual);
        }
    }
}

// ==========================================
// CONFIGURACIÓN E INICIO
// ==========================================

bool Server::iniciar() {
    // IGNORAR SIGPIPE: Evita que el servidor se cierre si envía datos a un socket desconectado
    signal(SIGPIPE, SIG_IGN);

    struct sockaddr_in direccion;
    int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Error: No se pudo crear el socket." << std::endl;
        return false;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cerr << "Error: Fallo en setsockopt." << std::endl;
        return false;
    }

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(puerto);

    if (bind(server_fd, (struct sockaddr *)&direccion, sizeof(direccion)) < 0) {
        std::cerr << "Error: Fallo en bind." << std::endl;
        return false;
    }

    if (listen(server_fd, 4) < 0) {
        std::cerr << "Error: Fallo al intentar escuchar en el puerto." << std::endl;
        return false;
    }
    
    std::cout << "Servidor de Asedio al Coloso iniciado en el puerto " << puerto << "." << std::endl;
    return true;
}

std::string Server::recibirMensaje(int cliente_socket) {
    if (cliente_socket <= 0) return "ERROR_DESCONEXION";
    
    char buffer[1024] = {0};
    int bytes_leidos = recv(cliente_socket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_leidos <= 0) {
        return "ERROR_DESCONEXION";
    }
    
    std::string mensaje(buffer);
    mensaje.erase(mensaje.find_last_not_of(" \n\r\t") + 1);
    return mensaje;
}

// ==========================================
// FASES DEL JUEGO
// ==========================================
void Server::esperarJugadores() {
    struct sockaddr_in direccion_cliente;
    int addrlen = sizeof(direccion_cliente);
    fd_set readfds; // Conjunto de sockets a vigilar

    std::cout << getTimestamp()+" ESPERANDO A QUE 4 JUGADORES SE CONECTEN..." << std::endl;
    guardarLog(getTimestamp() + " ESPERANDO A QUE 4 JUGADORES SE CONECTEN...");

    while (sockets_clientes.size() < 4) {
        // 1. Limpiamos y preparamos la lista
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds); // Vigilamos las nuevas conexiones
        
        int max_sd = server_fd;

        // Añadimos a los clientes que ya están en la sala a la lista
        for (int sd : sockets_clientes) {
            FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        // 2. select() congela el servidor hasta que pase ALGO (alguien entra o alguien se va)
        int actividad = select(max_sd + 1, &readfds, nullptr, nullptr, nullptr);
        
        if (actividad < 0) {
            std::cerr << "Error en select()." << std::endl;
            continue;
        }

        // 3. ¿Alguien nuevo está intentando entrar?
        if (FD_ISSET(server_fd, &readfds)) {
            int nuevo_socket = accept(server_fd, (struct sockaddr *)&direccion_cliente, (socklen_t*)&addrlen);
            if (nuevo_socket >= 0) {
                sockets_clientes.push_back(nuevo_socket);
                
                std::cout<<getTimestamp()+"; Jugador Nuevo; Conectado; " << sockets_clientes.size()<< "/4." << std::endl;
                guardarLog(getTimestamp() + "; Jugador Nuevo; Conectado; " + std::to_string(sockets_clientes.size()) + "/4.");
                
                enviarMensaje(nuevo_socket, "CONECTADO|ESPERANDO A LOS DEMAS DIVINOS...");
            }
        }

        // 4. Revisamos si alguno de los que ya estaba conectado hizo algo (como desconectarse)
        for (auto it = sockets_clientes.begin(); it != sockets_clientes.end(); ) {
            int sd = *it;
            if (FD_ISSET(sd, &readfds)) {
                // Intentamos leer. Si read/recv devuelve 0, significa que el cliente cerró la conexión
                char buffer[1];
                int valread = recv(sd, buffer, 1, MSG_PEEK); // MSG_PEEK lee sin consumir el mensaje real
                
                if (valread == 0) {
                    // ¡Se desconectó!
                    std::cout << "UN DIVINO HA DESAPERECIDO... QUE CRUEL...";
                    close(sd);
                    it = sockets_clientes.erase(it); // Lo sacamos de la lista
                    
                    std::cout<<getTimestamp()+"; Jugador Desconectado;Desconexión"<<sockets_clientes.size()<<"/4."<< std::endl;
                    guardarLog(getTimestamp() + "; Jugador Desconectado;Desconexión" + std::to_string(sockets_clientes.size()) + "/4.");
                    
                    std::cout << "Sala: " << sockets_clientes.size() << "/4." << std::endl;
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }
    }

    std::cout << "Sala llena - Procedemos a la planificacion de equipos" << std::endl;
    enviarATodos("NUESTRA SALA DE JUEGOS HA SIDO COMPLETADA, LOS 4 DIVINOS ESTAN REUNIDOS.");
    enviarATodos("SALA_DE SIMULACION_LLENA| PROCEDEREMOS A ELEGIR BANDOS...");
}

bool Server::faseSeleccionEquipo() {
    std::cout << "\n--- INICIANDO FASE 1: ¿QUIENES BUSCAIS SER?---" << std::endl;
    enviarATodos("EN ESTE MUNDO, ¿QUIENES BUSCAIS SER?");

    // Estructura interna para llevar el estado individual de cada conexión
    enum Paso { ESPERANDO_NOMBRE, ESPERANDO_EQUIPO, LISTO };
    
    struct EstadoJugadorSeleccion {
        std::string nombre = "";
        Equipo equipo = Equipo::AVENTURERO;
        Paso paso = ESPERANDO_NOMBRE;
    };

    std::vector<EstadoJugadorSeleccion> estados(sockets_clientes.size());
    int aventureros_elegidos = 0;
    int colosos_elegidos = 0;
    int jug_listos = 0;

    // 1. Enviamos la primera solicitud a TODOS los jugadores a la vez
    for (size_t i = 0; i < sockets_clientes.size(); i++) {
        enviarMensaje(sockets_clientes[i], "FASE 1\nDIVINO, POR FAVOR IDENTIFICATE");
    }

    // 2. Bucle asíncrono con select()
    while (jug_listos < static_cast<int>(sockets_clientes.size())) {
        fd_set readfds;
        FD_ZERO(&readfds);
        int max_sd = 0;

        for (int sd : sockets_clientes) {
            FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        int actividad = select(max_sd + 1, &readfds, nullptr, nullptr, nullptr);
        if (actividad < 0) continue;

        // Procesamos a cualquiera que haya enviado una respuesta
        for (size_t i = 0; i < sockets_clientes.size(); i++) {
            int sock = sockets_clientes[i];

            if (FD_ISSET(sock, &readfds)) {
                std::string msg = recibirMensaje(sock);

                // Manejo de desconexión
                if (msg == "ERROR_DESCONEXION") {
                    std::cout << "Jugador desconectado durante Fase 1. Abortando..." << std::endl;
                    
                    std::cout<<getTimestamp()+"; Jugador Desconectado;Desconexión"<<sockets_clientes.size()<<"/4."<< std::endl;
                    guardarLog(getTimestamp() + "; Jugador Desconectado;Desconexión" + std::to_string(sockets_clientes.size()) + "/4.");
                    
                    close(sock);
                    sockets_clientes.erase(sockets_clientes.begin() + i);
                    enviarATodos("SISTEMA|UN DIVINO NOS HA ABANDONADO. HEMOS DE VOLVER A PARA ESPERAR POR OTRO...");
                    estado_juego = GameState();
                    return false;
                }

                // PASO A: El jugador envió su nombre
                if (estados[i].paso == ESPERANDO_NOMBRE) {
                    estados[i].nombre = msg;
                    estados[i].paso = ESPERANDO_EQUIPO;
                    std::cout << "Jugador en socket " << sock << "; Registrado como: " << msg << std::endl;
                    
                    std::cout<<getTimestamp()+"; Jugador Sock: "<<sock<<";Nombramiento; "<<" Ahora se llama "<<msg<< std::endl;
                    guardarLog(getTimestamp() + "; Jugador Sock: " + std::to_string(sock) + ";Nombramiento;  Ahora se llama " + msg);
                    
                    // Le enviamos su menú de bando con cupos actuales
                    std::string menu = "MUY BIEN " + msg + ", ELIGE TU DESTINO:\n";
                    if (aventureros_elegidos < 3) menu += "1. Aventurero (" + std::to_string(3 - aventureros_elegidos) + " cupos)\n";
                    if (colosos_elegidos < 1) menu += "2. Coloso (" + std::to_string(1 - colosos_elegidos) + " cupos)\n";
                    enviarMensaje(sock, menu);
                }
                // PASO B: El jugador seleccionó su bando
                else if (estados[i].paso == ESPERANDO_EQUIPO) {
                    if (msg == "1" && aventureros_elegidos < 3) {
                        estados[i].equipo = Equipo::AVENTURERO;
                        aventureros_elegidos++;
                        estados[i].paso = LISTO;
                        jug_listos++;
                        enviarMensaje(sock, "CONFIRMADO|HAS DECIDIDO SER UN AVENTURERO. TU ESPIRITU DE CONQUISTA HABLA POR TI...");
                        
                        std::cout<<getTimestamp()+"; "+msg+"; Union Equipo; Se une a Aventureros"<< std::endl;
                        guardarLog(getTimestamp() + "; " + msg + "; Union Equipo; Se une a Aventureros");
                    } 
                    else if (msg == "2" && colosos_elegidos < 1) {
                        estados[i].equipo = Equipo::COLOSO;
                        colosos_elegidos++;
                        estados[i].paso = LISTO;
                        jug_listos++;
                        enviarMensaje(sock, "CONFIRMADO|POR FAVOR, PROTEGE ESTE MUNDO, COLOSO...");
                        
                        std::cout<<getTimestamp()+"; "+msg+"; Union Equipo; Se une a Colosos"<< std::endl;
                        guardarLog(getTimestamp() + "; " + msg + "; Union Equipo; Se une a Colosos");
                    }
                    else {
                        // Si un jugador elige Coloso o aventurero cuando ya no hay cupos salta este mensaje 
                        std::string menu = "ERROR|ESTE MUNDO NO PUEDE CONCEBIR MAS SERES COMO ESTOS, POR FAVOR ENTIENDALO:\n";
                        if (aventureros_elegidos < 3) menu += "1. Aventurero (" + std::to_string(3 - aventureros_elegidos) + " cupos)\n";
                        if (colosos_elegidos < 1) menu += "2. Coloso (" + std::to_string(1 - colosos_elegidos) + " cupos)\n";
                        enviarMensaje(sock, menu);
                    }
                }
            }
        }
    }

    // 3. REORDENAMIENTO AUTOMÁTICO: Alinear sockets y jugadores (0,1,2 = Aventureros | 3 = Coloso)
    std::vector<int> sockets_ordenados;
    std::vector<EstadoJugadorSeleccion> estados_ordenados;

    // A. Primero filtramos y colocamos a los 3 Aventureros
    for (size_t i = 0; i < sockets_clientes.size(); i++) {
        if (estados[i].equipo == Equipo::AVENTURERO) {
            sockets_ordenados.push_back(sockets_clientes[i]);
            estados_ordenados.push_back(estados[i]);
        }
    }

    // B. Al final colocamos al Coloso
    for (size_t i = 0; i < sockets_clientes.size(); i++) {
        if (estados[i].equipo == Equipo::COLOSO) {
            sockets_ordenados.push_back(sockets_clientes[i]);
            estados_ordenados.push_back(estados[i]);
        }
    }

    // C. Reemplazamos la lista de sockets con la lista reordenada
    sockets_clientes = sockets_ordenados;

    // D. Registramos los jugadores en el GameState en el nuevo orden garantizado
    estado_juego = GameState(); // Limpiamos el tablero por seguridad
    for (size_t i = 0; i < sockets_clientes.size(); i++) {
        Player p(estados_ordenados[i].nombre);
        p.asignarEquipo(estados_ordenados[i].equipo);
        estado_juego.agregarJugador(p);
    }

    std::cout << "Fase 1 completada simultaneamente." << std::endl;
    enviarATodos("\nSISTEMA|TODOS HABEIS ELEGIDO VUESTRO BANDO...");
    enviarATodos("POR FAVOR, AHORA ELIJAN QUIEN QUIEREN SER EN ESTE MUNDO CREADO PARA USTEDES...\n");
    return true;
}


// FASE 2: NUESTROS QUERIDOS DIVINOS SE ENCUENTRAN EN EL DILEMA DE A QUIEN ENCARNAR EN ESTE MUNDO
bool Server::faseSeleccionClase() {
    std::cout << "\n--- INICIANDO FASE 2: SELECCION DE CLASE (SIMULTANEO) ---" << std::endl;

    std::vector<bool> listo(sockets_clientes.size(), false);
    int jug_listos = 0;

    // 1. Enviamos el menú de clase correspondiente a CADA jugador al mismo tiempo
    for (size_t i = 0; i < sockets_clientes.size(); i++) {
        int sock = sockets_clientes[i];
        Player& jugador = estado_juego.getJugador(i);
        std::vector<std::string> opciones = ClassManager::getClasesDisponibles(jugador.getEquipo());

        std::string menu = "ELIGE QUIEN SERAS (ESCRIBE EL NUMERO):\n";
        for (size_t j = 0; j < opciones.size(); j++) {
            menu += std::to_string(j + 1) + ". " + opciones[j] + "\n";
        }
        enviarMensaje(sock, menu);
    }

    // 2. Bucle asíncrono para recibir las elecciones conforme lleguen
    while (jug_listos < static_cast<int>(sockets_clientes.size())) {
        fd_set readfds;
        FD_ZERO(&readfds);
        int max_sd = 0;

        for (size_t i = 0; i < sockets_clientes.size(); i++) {
            if (!listo[i]) {
                FD_SET(sockets_clientes[i], &readfds);
                if (sockets_clientes[i] > max_sd) max_sd = sockets_clientes[i];
            }
        }

        int actividad = select(max_sd + 1, &readfds, nullptr, nullptr, nullptr);
        if (actividad < 0) continue;

        for (size_t i = 0; i < sockets_clientes.size(); i++) {
            int sock = sockets_clientes[i];

            if (!listo[i] && FD_ISSET(sock, &readfds)) {
                std::string respuesta = recibirMensaje(sock);

                if (respuesta == "ERROR_DESCONEXION") {
                    std::cout << "Jugador desconectado durante Fase 2. Abortando..." << std::endl;
                    close(sock);
                    sockets_clientes.erase(sockets_clientes.begin() + i);
                    enviarATodos("SISTEMA|UN DIVINO NOS HA ABANDONADO. HEMOS DE VOLVERA PARA ESPERAR POR OTRO...");
                    
                    std::cout<<getTimestamp()+"; Jugador Desconectado;Desconexión"<<sockets_clientes.size()<<"/4."<< std::endl;
                    guardarLog(getTimestamp() + "; Jugador Desconectado;Desconexión" + std::to_string(sockets_clientes.size()) + "/4.");
                    
                    estado_juego = GameState();
                    return false;
                }

                Player& jugador = estado_juego.getJugador(i);
                std::vector<std::string> opciones = ClassManager::getClasesDisponibles(jugador.getEquipo());

                try {
                    int seleccion = std::stoi(respuesta);
                    if (seleccion >= 1 && seleccion <= static_cast<int>(opciones.size())) {
                        std::string clase_elegida = opciones[seleccion - 1];
                        if (ClassManager::asignarClase(jugador, clase_elegida)) {
                            listo[i] = true;
                            jug_listos++;
                            enviarMensaje(sock, "CONFIRMADO|FINALMENTE OPTASTE POR UNA NUEVA FORMA, UN " + clase_elegida + "..., YA VEO...\n ALGUNOS DIVINOS AUN NO SE DECIDEN...");
                            std::cout << jugador.getNombre() << " eligio " << clase_elegida << std::endl;
                            
                            std::cout<<getTimestamp()+"; "<<jugador.getNombre()<<"; "
                            << (jugador.getEquipo() == Equipo::COLOSO ? "COLOSO" : "AVENTURERO") 
                            <<"; Eligió"+clase_elegida << std::endl;
                            
                            guardarLog(getTimestamp() + "; " + jugador.getNombre() + "; " + 
                                      (jugador.getEquipo() == Equipo::COLOSO ? "COLOSO" : "AVENTURERO") + "; Eligió " + clase_elegida);
                        }
                    } else {
                        enviarMensaje(sock, "ERROR|Numero fuera de rango. Intenta de nuevo:");
                    }
                } catch (...) {
                    enviarMensaje(sock, "ERROR|Ingresa solo el numero de la clase:");
                }
            }
        }
    }

    std::cout << "Fase 2 completada simultaneamente." << std::endl;
    enviarATodos("YA TODOS HABEIS ELEGIDO VUESTRO DESTINO...");
    enviarATodos("¡QUE LA SUERTE ACOMPAÑE A LOS MAS FUERTES!");
    
    estado_juego.iniciarJuego();
    return true;
}

// Loop de la fase 3
void Server::loopCombate() {
    std::cout << "\n--- INICIANDO FASE 3: COMBATE ---" << std::endl;
    srand(time(nullptr));

    bool juego_terminado = false;
    std::cout<<getTimestamp()+"; "<<"Inicio de partida" << std::endl;
    guardarLog(getTimestamp() + "; Inicio de partida");

    while (!juego_terminado) {
        int turno_index = estado_juego.getTurnoActualIndex();
        int socket_actual = sockets_clientes[turno_index];
        Player& atacante = estado_juego.getJugadorActual();

        // Si el personaje actual está muerto, saltamos turno automáticamente
        if (!atacante.estaVivo()) {
            estado_juego.avanzarTurno();
            continue;
        }

        enviarATodos("\n==================================");
        enviarATodos("ES EL TURNO DE:: " + atacante.getNombre() + " (" + atacante.getClase() + ")");
        
        std::cout<<getTimestamp()<<"; "<<atacante.getNombre() <<"; "
        << (atacante.getEquipo() == Equipo::COLOSO ? "COLOSO" : "AVENTURERO") << std::endl;
        
        guardarLog(getTimestamp() + "; " + atacante.getNombre() + "; " + 
                  (atacante.getEquipo() == Equipo::COLOSO ? "COLOSO" : "AVENTURERO"));
        
        enviarATodos("HP: " + std::to_string(atacante.getHp()) + "/" + std::to_string(atacante.getHpMax()));
        
        bool accion_valida = false;
        while (!accion_valida) {
            // ----------------------------------------------------
            // 1. SELECCIÓN DE MOVIMIENTO
            // ----------------------------------------------------
            std::string menu_movs = "Elige tu accion:\n";
            auto& movimientos = atacante.getMovimientos();
            for (size_t i = 0; i < movimientos.size(); i++) {
                menu_movs += std::to_string(i + 1) + ". " + movimientos[i].nombre;
                if (movimientos[i].cooldown_actual > 0) {
                    menu_movs += " (En espera: " + std::to_string(movimientos[i].cooldown_actual) + " turnos)\n";
                } else {
                    menu_movs += " (Listo)\n";
                }
            }
            
            enviarMensaje(socket_actual, menu_movs);
            std::string resp_mov = recibirMensajeConControl(socket_actual);

            // Validamos únicamente resp_mov en este paso
            if (resp_mov == "ERROR_DESCONEXION") {
                atacante.recibirDano(99999);
                enviarATodos("SISTEMA| " + atacante.getNombre() + " se desconecto y fue eliminado.");
                close(socket_actual);
                sockets_clientes[turno_index] = -1; // Marcamos como inactivo
                accion_valida = true;
                continue; 
            }

            int sel_mov = -1;
            try {
                sel_mov = std::stoi(resp_mov) - 1;
                if (sel_mov < 0 || sel_mov >= static_cast<int>(movimientos.size())) {
                    enviarMensaje(socket_actual, "ERROR|Opcion fuera de rango.");
                    continue;
                }
                if (movimientos[sel_mov].cooldown_actual > 0) {
                    enviarMensaje(socket_actual, "ERROR|Esta habilidad esta en enfriamiento.");
                    continue;
                }
            } catch (...) {
                enviarMensaje(socket_actual, "ERROR|Ingresa solo el numero de la opcion.");
                continue;
            }

            // ----------------------------------------------------
            // 2. SELECCIÓN DE OBJETIVO FILTRADO
            // ----------------------------------------------------
            Movimiento mov_elegido = movimientos[sel_mov];
            Player* objetivo = nullptr;

            // CASO A: Curación
            if (mov_elegido.tipo == TipoMovimiento::CURACION) {
                if (atacante.getEquipo() == Equipo::COLOSO) {
                    objetivo = &atacante; // El Coloso se cura a sí mismo
                } else {
                    // Muestra SOLO Aventureros VIVOS (índices 0, 1, 2)
                    std::vector<int> indices_aliados;
                    std::string menu_aliados = "Elige a que aliado curar:\n";
                    int opcion_num = 1;

                    for (int k = 0; k < 3; k++) {
                        Player& p = estado_juego.getJugador(k);
                        if (p.estaVivo()) {
                            indices_aliados.push_back(k);
                            menu_aliados += std::to_string(opcion_num++) + ". " + p.getNombre() + 
                                            " (HP: " + std::to_string(p.getHp()) + "/" + std::to_string(p.getHpMax()) + ")\n";
                        }
                    }

                    enviarMensaje(socket_actual, menu_aliados);
                    std::string resp_obj = recibirMensajeConControl(socket_actual);

                    if (resp_obj == "ERROR_DESCONEXION") {
                        atacante.recibirDano(99999);
                        std::cout<<getTimestamp()<<"; "<< atacante.getNombre() << "; Se desconectó"<<std::endl;
                        guardarLog(getTimestamp() + "; " + atacante.getNombre() + "; Se desconectó");
                        
                        enviarATodos("SISTEMA| A " + atacante.getNombre() + " LE DIO UN PARO CARDIACO, QUE LAMENTABLE...");
                        close(socket_actual);
                        sockets_clientes[turno_index] = -1;
                        accion_valida = true;
                        continue;
                    }

                    try {
                        int sel_obj = std::stoi(resp_obj) - 1;
                        if (sel_obj >= 0 && sel_obj < static_cast<int>(indices_aliados.size())) {
                            objetivo = &estado_juego.getJugador(indices_aliados[sel_obj]);
                        } else {
                            enviarMensaje(socket_actual, "ERROR|Seleccion invalida.");
                            continue;
                        }
                    } catch (...) {
                        enviarMensaje(socket_actual, "ERROR|Entrada invalida.");
                        continue;
                    }
                }
            } 
            // CASO B: Ataque Físico / Mágico
            else {
                if (atacante.getEquipo() == Equipo::COLOSO) {
                    // El Coloso elige entre los Aventureros VIVOS (índices 0, 1, 2)
                    std::vector<int> indices_enemigos;
                    std::string menu_enemigos = "Elige a que Aventurero atacar:\n";
                    int opcion_num = 1;

                    for (int k = 0; k < 3; k++) {
                        Player& p = estado_juego.getJugador(k);
                        if (p.estaVivo()) {
                            indices_enemigos.push_back(k);
                            menu_enemigos += std::to_string(opcion_num++) + ". " + p.getNombre() + 
                                             " (" + p.getClase() + " - HP: " + std::to_string(p.getHp()) + "/" + std::to_string(p.getHpMax()) + ")\n";
                        }
                    }

                    enviarMensaje(socket_actual, menu_enemigos);
                    std::string resp_obj = recibirMensajeConControl(socket_actual);

                    if (resp_obj == "ERROR_DESCONEXION") {
                        atacante.recibirDano(99999);
                        std::cout<<getTimestamp()<<"; "<< atacante.getNombre() << "; Se desconectó"<<std::endl;
                        guardarLog(getTimestamp() + "; " + atacante.getNombre() + "; Se desconectó");
                        
                        enviarATodos("SISTEMA| A " + atacante.getNombre() + " LE DIO UN PARO CARDIACO, QUE LAMENTABLE...");
                        close(socket_actual);
                        sockets_clientes[turno_index] = -1;
                        accion_valida = true;
                        continue;
                    }

                    try {
                        int sel_obj = std::stoi(resp_obj) - 1;
                        if (sel_obj >= 0 && sel_obj < static_cast<int>(indices_enemigos.size())) {
                            objetivo = &estado_juego.getJugador(indices_enemigos[sel_obj]);
                        } else {
                            enviarMensaje(socket_actual, "ERROR|Seleccion invalida.");
                            continue;
                        }
                    } catch (...) {
                        enviarMensaje(socket_actual, "ERROR|Entrada invalida.");
                        continue;
                    }
                } else {
                    // Aventureros atacan al Coloso (índice 3 fijo)
                    objetivo = &estado_juego.getJugador(3);
                }
            }

            // ----------------------------------------------------
            // 3. EJECUCIÓN Y NOTIFICACIÓN DE MUERTE
            // ----------------------------------------------------
            if (objetivo != nullptr) {
                int dado = (rand() % 6) + 1;
                std::string resultado = estado_juego.ejecutarMovimiento(atacante, *objetivo, sel_mov, dado);
                
                enviarATodos(">>> " + resultado);

                // Verificación de muerte tras la acción
                if (!objetivo->estaVivo()) {
                    enviarATodos(">>> ¡ALERTA! " + objetivo->getNombre() + " (" + objetivo->getClase() + ") ha sido ELIMINADO.");
                }

                accion_valida = true;
            }
        }

        // ----------------------------------------------------
        // 4. VERIFICACIÓN DE VICTORIA Y AVANCE DE TURNO
        // ----------------------------------------------------
        Equipo ganador = estado_juego.verificarGanador();
        if (ganador != Equipo::NINGUNO) {
            juego_terminado = true;
            std::string msj_victoria = (ganador == Equipo::AVENTURERO) ? 
                                       "\n ¡LOS AVENTUREROS HAN DERROTADO AL COLOSO, EL MUNDO AHORA ES SUYO!" : 
                                       "\n ¡EL COLOSO HA APLASTADO A LOS AVENTUREROS, EL MUNDO VUELVE A ESTAR EN PAZ! POR AHORA...";
            enviarATodos(msj_victoria);
            
            std::cout << getTimestamp() << "; Partida Finalizada" << std::endl;
            guardarLog(getTimestamp() + "; Partida Finalizada");
            
            // --- AÑADE ESTO PARA CERRAR LA CONEXIÓN Y LIBERAR A LOS CLIENTES ---
            for (int socket_cliente : sockets_clientes) {
                if (socket_cliente > 0) {
                    close(socket_cliente);
                }
            }
            // -----------------------------------------------------------------

            std::cout << "Partida finalizada con exito." << std::endl;
        } else {
            estado_juego.avanzarTurno();
        }
    }
}