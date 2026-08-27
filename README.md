# mangOS

Repositorio del proyecto semestral del equipo mangOS de INFO198 - Sistemas operativos. Ingeniería Civil en Informática, Universidad Austral de Chile. Segundo semestre, 2026.

**Docente:** Dr. Luis Veas Castillo.<br>
**Ayudante:** Francisco Labrín.<br>
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

Se empleó IA generativa, Google Gemini y ChatGPT, para asesoría en torno a corrección algorítmica y sintáctica del código fuente y de la correctitud conceptual, de formato y estructura del presente documento y del Manual de Usuario. Los autores asumen la responsabilidad por el contenido y la precisión del trabajo presentado.---

## Cómo compilar y ejecutar

El proyecto está escrito en C++ estándar y puede compilarse usando herramientas como `g++` (GCC) o cualquier compilador de C++ moderno.

### 1. Requisitos previos

Asegúrese de contar con un compilador de C++ instalado.

### 2. Configuración inicial

El sistema requiere de un archivo `.env` en la raíz del proyecto (o donde se ejecute el binario) con las rutas de los archivos de base de datos.
Asegúrese de que su archivo `.env` contienga información como:

```env
USUARIOS_TXT=./USUARIOS.TXT
PERFILES_TXT=./PERFILES.TXT
```

### 3. Compilación

La forma recomendada y automatizada de compilar el proyecto es utilizando la herramienta `make`. Desde la raíz del proyecto, simplemente ejecute:

```bash
make
```

Nota: El Makefile está configurado para usar g++ por defecto. Si desea utilizar otro compilador (como clang++), puede compilar manualmente, ejecutando:

```bash
clang++ -std=c++17 SRC/SistOpe/SistOpe.cpp SRC/userManager/userManager.cpp -o mangOS
```

### 4. Ejecución

Una vez compilado y con el archivo `.env` en el mismo directorio (o en la ruta esperada), ejecute:

```bash
./mangOS
```
### 5. Operación

Las instrucciones básicas de operación se encuentran en `Manual_Usuario.md` junto a especificaciones generales del funcionamiento del módulo userManager.

---

## Estructura del proyecto

```text
mangOS
    │   .env                  # Archivo de configuración con variables de entorno
    │   Makefile              # Archivo para automatizar la compilación
    │   README.md             # Documentación principal del proyecto
    |   Manual_Usuario.md     # Especificaciones funcionales e instrucciones de uso.
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
