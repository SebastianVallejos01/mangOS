# mangOS

Repositorio del proyecto semestral del equipo mangOS de INFO198 - Sistemas operativos. Ingeniería Civil en Informática, Universidad Austral de Chile. Segundo semestre, 2026.

**Docente:** Dr. Luis Veas Castillo.  
**Ayudante:** Francisco Labrín.  
**Equipo:** Jorge Cárcamo, Sebastián Catalán, Dante Gatica, Sebastián Vallejos y Alonso Véliz.

---

## Características Principales

Actualmente, el sistema implementa un completo módulo de gestión de identidades y accesos a través de una interfaz de consola interactiva:

- **Gestión de Usuarios:**
  - Creación de usuarios asignando credenciales únicas (`username`, `password`).
  - Asignación de perfiles (roles) durante la creación.
  - Visualización (listado) de todos los usuarios registrados.
  - Eliminación de usuarios por medio de su ID.
- **Gestión de Perfiles:**
  - Creación de perfiles (Ej: `ADMIN`, `GENERAL`) con nombres en mayúsculas de manera automática.
  - Asignación de permisos numéricos al menú (1: Ingresar/crear, 2: Enlistar, 3: Eliminar, etc.).
  - Listado de los perfiles disponibles y sus respectivos permisos.
  - Eliminación de perfiles del sistema.
- **Persistencia de Datos:** Toda la información se guarda localmente en archivos de texto, por lo que el sistema recuerda los datos entre ejecuciones.
- **Configuración por Entorno:** Las rutas de los archivos de texto se configuran de manera segura usando variables de entorno en un archivo `.env`.

---

## Cómo compilar y ejecutar

El proyecto está escrito en C++ estándar y puede compilarse usando herramientas como `g++` (GCC) o cualquier compilador de C++ moderno.

### 1. Requisitos previos

Asegúrate de contar con un compilador de C++ instalado.

### 2. Configuración inicial

El sistema requiere de un archivo `.env` en la raíz del proyecto (o donde se ejecute el binario) con las rutas de los archivos de base de datos.
Asegúrate de que tu archivo `.env` contiene información como:

```env
USUARIOS_TXT=./USUARIOS.TXT
PERFILES_TXT=./PERFILES.TXT
```

### 3. Compilación (Ejemplo usando `g++`)

Desde la carpeta raíz del proyecto, puedes compilar todos los archivos fuente juntos:

```bash
g++ -std=c++17 SRC/SistOpe/SistOpe.cpp SRC/userManager/userManager.cpp -o mangOS.exe
```

### 4. Ejecución

Una vez compilado y con el archivo `.env` en el mismo directorio (o en la ruta esperada), ejecuta:

```bash
./mangOS.exe
```

---

## Estructura del proyecto

```text
mangOS
    │   .env                  # Archivo de configuración con variables de entorno
    │   Makefile              # Archivo para automatizar la compilación
    │   mangOS.exe            # Ejecutable final compilado
    │   README.md             # Documentación principal del proyecto
    │
    ├───BD                    # Carpeta base para el almacenamiento de datos
    │   └───UM                # Datos del módulo User Manager
    │           PERFILES.TXT  # Archivo plano que almacena los perfiles y permisos
    │           USUARIOS.TXT  # Archivo plano que almacena las credenciales
    │
    └───SRC                   # Código fuente de la aplicación
        ├───SistOpe           # Núcleo principal del Sistema Operativo
        │       SistOpe.cpp   # Archivo principal (Main), enlaza los menús
        │
        └───userManager       # Módulo de gestión de identidades y accesos
                userManager.cpp # Lógica interactiva de usuarios y perfiles
                userManager.hpp # Cabecera con declaración de estructuras (User, Profile)
```
