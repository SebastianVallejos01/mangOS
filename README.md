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
  Para garantizar la flexibilidad, escalabilidad y seguridad en la gestión de rutas físicas, las rutas de los archivos de texto se configuran de manera segura usando variables de entorno en un archivo `.env`. Durante el arranque, el sistema busca dinámicamente este archivo escalando por los directorios desde la ruta de ejecución actual. Para el correcto funcionamiento del módulo, se deben declarar dentro de este archivo `.env` obligatoriamente dos variables de entorno: USER_FILE y PERFIL_FILE. Estas variables le indican al programa las rutas relativas exactas donde se ubican los archivos de texto que actúan como bases de datos para persistir la información de los usuarios y perfiles.

## Declaración de uso de IA generativa

Se empleó IA generativa, Google Gemini Pro y ChatGPT-5.6 Luna, para asesoría en torno a correctitud algorítmica y sintáctica del código fuente y de la corrección conceptual, de formato y estructura del presente documento y del Manual de Usuario. Los autores asumen la responsabilidad por el contenido y la precisión del trabajo presentado.

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

Se presenta tabla que describe a detalle las variables de entorno.


| Key           | Value            | Descripción             | Formato del contenido                   |
| ------------- | ---------------- | ------------------------ | --------------------------------------- |
| `USER_FILE`   | `./USUARIOS.TXT` | Ruta archivo de usuarios | `id; nombre; usuario; password; perfil` |
| `PERFIL_FILE` | `./PERFILES.TXT` | Ruta archivo de perfiles | `nombre;permisos`                       |

### 3. Compilación

La forma recomendada y automatizada de compilar el proyecto es utilizando la herramienta `make` (o `mingw32-make` en Windows). El `Makefile` está configurado para compilar tanto el núcleo principal (`mangOS`) como los programas secundarios (ej: `multi`).

Desde la raíz del proyecto, simplemente ejecute:

```bash
make
```

### 4. Ejecución

Una vez compilado y con el archivo `.env` en el mismo directorio (o en la ruta esperada), el programa exige que te identifiques mediante argumentos de consola.

Ejecute el siguiente comando reemplazando con sus credenciales:

```bash
./mangOS -u <usuario> -p <contraseña> -f <ruta_archivo>
```

Ejemplo:

```bash
./mangOS -u admin -p admin123 -f Makefile
```

### 5. Operación

Las instrucciones básicas de operación se encuentran en `Manual_Usuario.md` junto a especificaciones generales del funcionamiento del menú principal y la gestión de procesos.

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
    │   ├───UM                # Datos del módulo User Manager
    │   │       PERFILES.TXT  # Archivo plano que almacena los perfiles y permisos
    │   │       USUARIOS.TXT  # Archivo plano que almacena las credenciales
    │   ├───Matrices          # Matrices almacenadas en formato texto (.TXT)
    │   └───Libros            # ~50 MB de libros de libre acceso de gutenberg.org y memoriachilena.gob.cl, en formato TXT
    │
    └───SRC                   # Código fuente de la aplicación
        ├───SistOpe           # Núcleo principal del Sistema Operativo
        │       SistOpe.cpp   # Archivo principal (Main), enlaza los menús
        │
        ├───login             # Módulo de autenticación
        │       login.cpp      # Lógica de login
        │       login.hpp      # Cabecera del módulo de login
        │
        ├───mainMenu          # Menú principal del sistema
        │       mainMenu.cpp   # Lógica del menú principal
        │       mainMenu.hpp   # Cabecera del módulo del menú principal
        │
        ├───mates             # Módulo matemático / cálculo
        │       mates.cpp      # Lógica del módulo mates
        │       mates.hpp      # Cabecera del módulo mates
        │
        ├───texto             # Módulo de texto
        │       texto.cpp      # Lógica del módulo texto
        │       texto.hpp      # Cabecera del módulo texto
        │
        └───userManager       # Módulo de gestión de identidades y accesos
                userManager.cpp # Lógica interactiva de usuarios y perfiles
                userManager.hpp # Cabecera con declaración de estructuras (User, Profile)
```
