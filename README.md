# mangOS

Repositorio del proyecto semestral del equipo mangOS de INFO198 - Sistemas operativos. Ingeniería Civil en Informática, Universidad Austral de Chile. Segundo semestre, 2026.

**Docente:** Dr. Luis Veas Castillo.<br>
**Ayudante:** Francisco Labrín.<br>
**Equipo:** Jorge Cárcamo, Sebastián Catalán, Dante Gatica, Sebastián Vallejos y Alonso Véliz.

---

## Propósito del proyecto

El proyecto tiene como propósito principal simular el núcleo de un Sistema Operativo. En esta etapa de desarrollo, el sistema cuenta con un módulo interactivo de "Administrador de Usuarios y Perfiles" y un **Menú Principal** que ejecuta procesos independientes (como cálculos matemáticos y multiplicación de matrices) mediante llamadas al sistema. El acceso al núcleo está protegido por una validación de credenciales a través de la interfaz de línea de comandos, simulando un entorno seguro donde cada usuario tiene permisos basados en su perfil (roles como ADMIN o GENERAL).

## Características Principales

Actualmente, el sistema implementa un menú principal protegido por credenciales y un completo módulo de gestión de identidades:

- **Autenticación por Consola:** Ingreso seguro al sistema requiriendo usuario, contraseña y un archivo de lectura, mediante argumentos de ejecución.
- **Menú Principal y Multiprocesamiento:** Ejecución de funcionalidades avanzadas (como la multiplicación de matrices NxM) delegando el trabajo a procesos externos (programas hijos) invocados mediante el sistema.
- **Gestión de Usuarios:**
  - Creación, listado y eliminación de usuarios con credenciales únicas.
  - Asignación de perfiles (roles) durante la creación.
- **Gestión de Perfiles:**
  - Creación de perfiles (Ej: `ADMIN`, `GENERAL`) con nombres automáticos en mayúsculas.
  - Asignación de permisos numéricos para el acceso a las funciones del menú.
- **Persistencia de Datos:** Toda la información de usuarios, perfiles y matrices se guarda localmente en archivos de texto, por lo que el sistema recuerda los datos entre ejecuciones.
- **Configuración por Entorno:**
  Para garantizar la flexibilidad, escalabilidad y seguridad en la gestión de rutas físicas, las rutas de los archivos de texto se configuran de manera segura usando variables de entorno en un archivo `.env`. Durante el arranque, el sistema busca dinámicamente este archivo escalando por los directorios desde la ruta de ejecución actual.
- **Procesamiento Concurrente (Índice Invertido):** Generación paralela de un índice invertido de frecuencia de palabras sobre archivos de texto mediante un esquema Productor-Consumidor (cola de trabajo, threads trabajadores y sincronización con semáforos POSIX y mutex).
- **Juego Multijugador en Red (Sockets):** Sistema de juego multijugador cliente-servidor basado en sockets TCP (*Asedio al Coloso*), con estado compartido sincronizado en tiempo real y registro de eventos en archivo LOG. *(Nota: Por el uso de sockets POSIX estándar de Linux, los ejecutables de cliente y servidor del juego requieren ejecutarse en un entorno Linux / WSL)*.

##### Procesamiento Paralelo del Índice Invertido
La arquitectura de indexación sigue un flujo desacoplado para optimizar el uso de CPU:
1. **Cola:** El hilo principal escanea la carpeta de libros, asigna un ID numérico a cada archivo `.txt` y los deposita en una cola compartida (`colaTrabajo`).
2. **Threads:** Se despliegan concurrentemente `N_THREADS` trabajadores encargados de consumir libros de la cola.
3. **Semáforos y Mutex:** Un semáforo POSIX contador (`sem_t`) coordina la disponibilidad de elementos en la cola, mientras que exclusiones mutuas (`std::mutex`) protegen la extracción segura y la escritura acumulada en memoria.
4. **Resultados e Índice Invertido:** Cada hilo genera un mapa local de frecuencias y lo fusiona en `indiceGlobal`. Tras finalizar todos los threads (`join`), el resultado se serializa en el archivo configurado con el formato `palabra;(idLibro,frecuencia)`.

##### Arquitectura del Juego Multijugador (4.4 Pauta)
El flujo arquitectónico del juego cumple con el esquema:
$$\text{clientes} \longrightarrow \text{sockets TCP} \longrightarrow \text{servidor} \longrightarrow \text{estado compartido del juego}$$

1. **Clientes:** Cada jugador ejecuta un proceso cliente (`cliente_juego`). Este proceso solicita la dirección IP del servidor e inicializa un socket TCP (`AF_INET`, `SOCK_STREAM`), además de desplegar un thread en segundo plano dedicado a escuchar y renderizar los mensajes emitidos por el servidor en tiempo real.
2. **Sockets:** La comunicación bidireccional entre cada cliente y el servidor se canaliza a través de descriptores de sockets individuales (`send`/`recv` sobre TCP en el puerto 8080).
3. **Servidor:** El servidor (`servidor_juego`) administra las conexiones entrantes en un socket maestro pasivo (`listen`/`accept`), manteniendo un pool de sockets de clientes conectados (`std::vector<int> sockets_clientes`).
4. **Estado Compartido del Juego:** El servidor instancia un objeto central `GameState` (`SRC/juego/Classes/GameState.hpp`), el cual centraliza el tablero y la lógica del juego:
   - Controla las entidades de los jugadores (`Player`), sus atributos (HP, Atk, Def, Mag, Cooldowns) y sus clases seleccionadas (`ClassManager`).
   - Gobierna las fases de la partida (lobby de conexión, selección de equipo, selección de clase y ciclo de combate por turnos).
   - Valida la legalidad de los movimientos, evalúa fórmulas de daño/curación con tiradas de dados de efectividad y verifica las condiciones de término de la partida.
   - Registra de forma persistente cada evento relevante (conexiones, acciones, resultados, desconexiones) en el archivo de auditoría indicado por `LOG_FILE`.

> [!IMPORTANT]
> **Compatibilidad del Juego (Linux vs Windows):**  
> El módulo del juego (*servidor_juego* y *cliente_juego*) hace uso directo de las cabeceras de sockets estándar de POSIX (`<sys/socket.h>`, `<netinet/in.h>`, `<arpa/inet.h>`, `<unistd.h>`). Por esta razón, **el juego solo se puede compilar y ejecutar en sistemas operativos Linux o entornos WSL (Windows Subsystem for Linux)**. Si el sistema general `mangOS` se ejecuta en Windows nativo, las funciones base funcionarán correctamente, pero la invocación a los binarios del juego requerirá el ambiente Linux.

##### Reglas del Juego (4.5 Pauta)
<!-- Rellenar a mano según lo especificado en la pauta -->

## Declaración de uso de IA generativa

Se empleó IA generativa, Google Gemini Flash 3.8, Pro 3.1, Argon 4 y ChatGPT-5.6 Luna, para asesoría en torno a correctitud algorítmica y sintáctica del código fuente y de la corrección conceptual, de formato y estructura del presente documento y del Manual de Usuario. Los autores asumen la responsabilidad por el contenido y la precisión del trabajo presentado.

---

## Cómo compilar y ejecutar

El proyecto está escrito en C++ estándar y puede compilarse usando herramientas como `g++` (GCC) o cualquier compilador de C++ moderno con soporte para C++17 e hilos (`-pthread`).

### 1. Requisitos previos

- Compilador de C++ (`g++` versión 9+ o `clang++` con soporte C++17).
- Utilidad `make` (o `mingw32-make` en Windows).
- Para el juego multiplayer: Sistema operativo **Linux** o **WSL**.

### 2. Configuración inicial

El sistema requiere de un archivo `.env` en la raíz del proyecto (o donde se ejecute el binario) con las rutas de los archivos de base de datos y parámetros de ejecución.
Asegúrese de que su archivo `.env` contenga la siguiente información:

```env
USER_FILE=./BD/UM/USUARIOS.TXT
PERFIL_FILE=./BD/UM/PERFILES.TXT
LOG_FILE=./BD/GAME/LOGS/LOG.TXT
MATRIZ_A=./BD/Matrices/M1.TXT
MATRIZ_B=./BD/Matrices/N1.TXT
SEPARADOR_MATRICES=#
LIBROS=./BD/Libros
ID_LIBROS=./BD/Index/id_libros.txt
INDEX_LIBROS=./BD/Index/indice_invertido.idx
N_THREADS=4
```

Se presenta tabla que describe a detalle las variables de entorno:

| Key                  | Value                             | Descripción                                  | Formato del contenido                   |
| -------------------- | --------------------------------- | -------------------------------------------- | --------------------------------------- |
| `USER_FILE`          | `./BD/UM/USUARIOS.TXT`            | Ruta archivo de usuarios                     | `id; nombre; usuario; password; perfil` |
| `PERFIL_FILE`        | `./BD/UM/PERFILES.TXT`            | Ruta archivo de perfiles                     | `nombre;permisos`                       |
| `LOG_FILE`           | `./BD/GAME/LOGS/LOG.TXT`          | Archivo de registro de eventos del juego     | Archivo de log de eventos               |
| `MATRIZ_A`           | `./BD/Matrices/M1.TXT`            | Ruta de la primera matriz                    | Archivo de matriz con delimitador       |
| `MATRIZ_B`           | `./BD/Matrices/N1.TXT`            | Ruta de la segunda matriz                    | Archivo de matriz con delimitador       |
| `SEPARADOR_MATRICES` | `#`                               | Delimitador usado en matrices                | Carácter delimitador (ej.`#`)           |
| `LIBROS`             | `./BD/Libros`                     | Directorio con los libros a procesar         | Directorio con archivos `.txt`          |
| `ID_LIBROS`          | `./BD/Index/id_libros.txt`        | Mapeo identificador - nombre de libro        | `id;nombre_archivo`                     |
| `INDEX_LIBROS`       | `./BD/Index/indice_invertido.idx` | Archivo de salida del índice invertido       | `palabra;(id,frecuencia);`              |
| `N_THREADS`          | `4`                               | Cantidad de hilos trabajadores para indexar  | Número entero mayor a 0                 |

### 3. Compilación

La forma recomendada y automatizada de compilar el proyecto es utilizando la herramienta `make`. Desde la raíz del proyecto, simplemente ejecute:

```bash
make
```

- **En Linux / WSL:** El comando compila automáticamente todos los componentes: el núcleo `mangOS`, el multiplicador de matrices `multi`, el servidor de juego `servidor_juego` y el cliente de juego `cliente_juego`.
- **En Windows:** `mingw32-make` compilará el núcleo `mangOS.exe` y el binario `multi.exe` (excluyendo cliente y servidor por requerir sockets POSIX).

Para limpiar binarios compilados y archivos objeto:
```bash
make clean
```

### 4. Ejecución

Una vez compilado y con el archivo `.env` configurado, el sistema se ejecuta identificándose mediante argumentos de consola:

```bash
./mangOS -u <usuario> -p <contraseña> -f <ruta_archivo>
```

Ejemplo:

```bash
./mangOS -u Seba -p Seba2006 -f Makefile
```

#### Ejecución del Juego (Linux / WSL)
El juego se puede lanzar de dos formas:
1. **Desde el Menú Principal:** Ingrese a la opción `3. Juego` dentro de `mangOS` (seleccione crear servidor o unirse a servidor).
2. **De forma independiente desde terminales separadas:**
   - Terminal 1 (Servidor): `./servidor_juego`
   - Terminales 2 a 5 (Clientes): `./cliente_juego` (e ingresar la IP del servidor).

### 5. Operación

Las instrucciones básicas de operación se encuentran en `Manual_Usuario.md` junto a especificaciones generales del funcionamiento del menú principal y la gestión de procesos.

---

## Estructura del proyecto

```text
mangOS
    │   .env                  # Archivo de configuración con variables de entorno
    │   Makefile              # Archivo para automatizar la compilación
    │   Manual_Usuario.md     # Especificaciones funcionales e instrucciones de uso.
    │   Manual_Usuario.pdf    # Manual de usuario en formato pdf.
    │   README.md             # Documentación principal del proyecto
    │
    ├───BD                    # Carpeta base para el almacenamiento de datos
    │   ├───GAME              # Datos y registros del juego
    │   │   └───LOGS
    │   │           LOG.TXT   # Archivo de logs y auditoría de partidas del juego
    │   ├───Index             # Archivos generados por el módulo de indexación
    │   │       id_libros.txt     # Mapeo de identificadores y nombres de libros
    │   │       indice_invertido.idx # Índice invertido final serializado
    │   ├───Libros            # Libros de libre acceso en formato TXT
    │   ├───Matrices          # Matrices almacenadas en formato texto (.TXT)
    │   └───UM                # Datos del módulo User Manager
    │           PERFILES.TXT  # Archivo plano que almacena los perfiles y permisos
    │           USUARIOS.TXT  # Archivo plano que almacena las credenciales
    │
    └───SRC                   # Código fuente de la aplicación
        ├───index             # Módulo de procesamiento concurrente e índice
        │       index.cpp     # Lógica de threads, sincronización y parseo
        │       index.hpp     # Cabecera, estructuras y primitivas de concurrencia
        │
        ├───juego             # Módulo del juego multijugador Asedio al Coloso
        │   │   menuJuego.cpp # Submenú del juego dentro de mangOS
        │   │   menuJuego.hpp
        │   ├───Classes       # Clases de estado del juego y jugadores
        │   │       ClassManager.cpp
        │   │       ClassManager.hpp
        │   │       GameState.cpp
        │   │       GameState.hpp
        │   │       player.cpp
        │   │       player.hpp
        │   ├───Cliente       # Código del cliente socket
        │   │       cliente.cpp
        │   ├───Metodo
        │   │       time.hpp  # Utilidad de formateo de fecha y hora para logs
        │   └───Server        # Código del servidor socket y orquestador
        │           server.cpp
        │           server.hpp
        │           serverStart.cpp
        │
        ├───login             # Módulo de autenticación
        │       login.cpp     # Lógica de login
        │       login.hpp     # Cabecera del módulo de login
        │
        ├───mainMenu          # Menú principal del sistema
        │       mainMenu.cpp  # Lógica del menú principal
        │       mainMenu.hpp  # Cabecera del módulo del menú principal
        │
        ├───mates             # Módulo matemático / cálculo
        │       mates.cpp     # Lógica del módulo mates
        │       mates.hpp     # Cabecera del módulo mates
        │       matrices.cpp  # Programa independiente multiplicador de matrices (multi)
        │
        ├───SistOpe           # Núcleo principal del Sistema Operativo
        │       SistOpe.cpp   # Archivo principal (Main), enlaza los menús
        │
        ├───texto             # Módulo de texto
        │       texto.cpp     # Lógica del módulo texto
        │       texto.hpp     # Cabecera del módulo texto
        │
        └───userManager       # Módulo de gestión de identidades y accesos
                userManager.cpp # Lógica interactiva de usuarios y perfiles
                userManager.hpp # Cabecera con declaración de estructuras (User, Profile)
```
