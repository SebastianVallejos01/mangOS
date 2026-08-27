# mangOS

Repositorio del proyecto semestral del equipo mangOS de INFO198 - Sistemas operativos. Ingeniería Civil en Informática, Universidad Austral de Chile. Segundo semestre, 2026.

**Docente:** Dr. Luis Veas Castillo.<br>
**Ayudante:** Francisco Labrín.<br>
**Equipo:** Jorge Cárcamo, Sebastián Catalán, Dante Gatica, Sebastián Vallejos y Alonso Véliz.

---

## Propósito del proyecto

El proyecto tiene como propósito principal simular el núcleo de un Sistema Operativo, enfocándose en su primera etapa en la implementación de un módulo de "Administrador de Usuarios y Perfiles". Este módulo interactivo permite gestionar identidades mediante la creación, visualización y eliminación de usuarios y perfiles, administrando credenciales seguras (como ID, username y password) y asignando niveles de acceso y permisos de menú a roles como ADMIN o GENERAL. La arquitectura del sistema está diseñada para sincronizar eficientemente estas estructuras de datos entre la memoria RAM y el almacenamiento físico, simulando operaciones transaccionales reales.

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
- **Configuración por Entorno:**
  Para garantizar la flexibilidad, escalabilidad y seguridad en la gestión de rutas físicas, las rutas de los archivos de texto se configuran de manera segura usando variables de entorno en un archivo `.env`. Durante el arranque, el sistema busca dinámicamente este archivo escalando por los directorios desde la ruta de ejecución actual. Para el correcto funcionamiento del módulo, se deben declarar dentro de este archivo `.env` obligatoriamente dos variables de entorno: USER_FILE y PERFIL_FILE. Estas variables le indican al programa las rutas relativas exactas donde se ubican los archivos de texto que actúan como bases de datos para persistir la información de los usuarios y perfiles.

## Declaración de uso de IA generativa

Se empleó IA generativa, Google Gemini Pro y ChatGPT-5.6 Luna, para asesoría en torno a corrección algorítmica y sintáctica del código fuente y de la correctitud conceptual, de formato y estructura del presente documento y del Manual de Usuario. Los autores asumen la responsabilidad por el contenido y la precisión del trabajo presentado.

---

## Cómo compilar y ejecutar

El proyecto está escrito en C++ estándar y puede compilarse usando herramientas como `g++` (GCC) o cualquier compilador de C++ moderno.

### 1. Requisitos previos

Asegúrese de contar con un compilador de C++ instalado.

### 2. Configuración inicial

El sistema requiere de un archivo `.env` en la raíz del proyecto (o donde se ejecute el binario) con las rutas de los archivos de base de datos.
Asegúrese de que su archivo `.env` contienga información como:

```env
USER_FILE=./USUARIOS.TXT
PERFIL_FILE=./PERFILES.TXT
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
    |   Manual_Usuario.pdf    # Manual de usuario en formato pdf.
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
