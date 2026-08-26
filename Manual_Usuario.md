**1\. Funcionamiento General de los Métodos**
----------------------------------------------

* **Navegación Continua (Ciclos de control):** La interfaz de usuario opera internamente mediante ciclos do-while. Esto garantiza que los menús se mantengan activos y a la espera de una instrucción válida (como la opción 0 para salir), asegurando un flujo de trabajo ininterrumpido.
* **Sincronización Híbrida:** Cada método de modificación actúa primero sobre la memoria temporal RAM (modificando las estructuras UserList y ProfileList). Tras la confirmación del operador, el sistema invoca una reescritura directa en el disco utilizando std::ofstream con el modo std::ios::trunc para sobreescribir los archivos con la versión actualizada.
* **Eficiencia de Procesos:** Los archivos físicos se leen y cargan en memoria exclusivamente durante el arranque del programa en el archivo principal. Posteriormente, todas las funciones de listado y modificación interactúan directamente con los vectores en memoria, optimizando drásticamente los tiempos de respuesta.

**2\. Manejo y Arquitectura de Datos**
--------------------------------------

* **Búsqueda Dinámica de Entorno:** La función getEnvFile localiza el archivo de configuración .env implementando un ciclo while (true). Si el archivo no está en la ruta de ejecución inicial, el algoritmo sube iterativamente un nivel en el árbol de directorios hasta encontrarlo o alcanzar la ruta raíz.
* **Auto-incremento de Identificadores:** Al crear un nuevo usuario, el sistema evita colisiones iterando sobre la lista completa de usuarios mediante un ciclo for. Identifica el valor maxId actual y asigna de forma automática el siguiente número disponible al nuevo registro.
* **Self-Healing (Auto-recuperación):** El método autoCrearPerfilesBase revisa las estructuras de datos y cuenta con banderas de estado (adminExiste, generalExiste) para restaurar los perfiles base si estos fueron alterados.

**3\. Gestión de Casos de Error y Seguridad**
----------------------------------------------

* **Filtro de Entradas por Omisión:** El sistema maneja las entradas inesperadas en los menús de forma pasiva y resiliente. Si el usuario ingresa un dato no reconocido, el menú simplemente no reacciona a la entrada inválida y vuelve a iterar el ciclo do-while, repitiendo la consulta sin colapsar ni requerir bloques complejos de manejo de excepciones.
* **Protección Estricta de Roles Base:** El método borraPerfil implementa un bloqueo de seguridad directo mediante una validación condicional (if (nombre == "ADMIN" || nombre == "GENERAL")). Esto deniega instantáneamente cualquier intento de eliminar las cuentas con mayores privilegios en el sistema.
* **Limpieza de Cadenas (Sanitización):** Para prevenir corrupción por manipulación manual de los archivos .txt, el sistema utiliza la función limpiarString, la cual remueve activamente marcas de orden de bytes (BOM) de archivos UTF-8 y recorta espacios en blanco residuales al inicio y final de las cadenas.

**4\. Instrucciones Rápidas de Uso**
-------------------------------------

**Arranque del Sistema** Al ejecutar el programa (mediante ./mangOS), el sistema cargará silenciosamente la configuración, verifica la base de datos y restaura los perfiles esenciales si es necesario. Inmediatamente después, se despliega el menú principal.

**Navegación Básica**

* Para moverse por los menús, escriba el **número** de la opción deseada y presione Enter.
* Utilice siempre el número 0 para retroceder al menú anterior o para apagar el sistema.
* El sistema está protegido contra errores de tipeo: si ingresa una letra o símbolo por accidente, la consola ignorará la entrada y volverá a mostrar la pregunta sin cerrarse.

**Módulo de Usuarios**

* **Crear:** El sistema le asignará un ID (número de identificación) de forma automática. Solo debe ingresar el nombre, credenciales y asegurarse de asignarle un perfil que **ya exista** en el sistema (por ejemplo, GENERAL). Finalmente, presione 1 para confirmar y guardar.
* **Eliminar:** Necesitará el ID numérico exacto del usuario. Se recomienda usar primero la opción "Listar Usuarios" para anotar el ID correcto antes de proceder a la eliminación.

**Módulo de Perfiles**

* **Crear:** Ingrese el nombre del nuevo rol; el sistema lo convertirá a mayúsculas automáticamente. Para los permisos, escriba los números correspondientes a los accesos deseados, presionando Enter tras cada uno. Cuando termine de asignar permisos, escriba 0 y presione Enter para finalizar la configuración.
* **Eliminar:** Escriba el nombre exacto del perfil que desea borrar. _Nota de seguridad:_ Los perfiles fundacionales ADMIN y GENERAL están blindados por el núcleo del sistema y no pueden ser eliminados bajo ninguna circunstancia.
