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

**Arranque y Autenticación del Sistema**
Para ejecutar el programa, debe identificarse obligatoriamente a través de la terminal, usando los flags de argumento requeridos.
Ejecuta el programa con:
`./mangOS -u <usuario> -p <contraseña> -f <ruta_archivo>`

* Ejemplo: `./mangOS -u admin -p 1234 -f Makefile`
  El sistema verificará las credenciales y el archivo; luego cargará silenciosamente la configuración, verificará la base de datos y desplegará el **Menú Principal**.

**Navegación Básica**

* Para moverse por los menús, escriba el **número** de la opción deseada y presione Enter.
* Utilice siempre el número 0 para retroceder al menú anterior o para salir/apagar el sistema.
  * Lo anterior vale para todas las funcionalidades a excepción de las opciones 2 (multiplicación de matrices), que retrocede automáticamente, y 5 (cálculo de función polinómica) ya que acepta la preimagen 0. De este último apartado se puede salir con una entrada no numérica. Por ejemplo, una letra "a".
  * Para salir de cada submenú del menú principal, necesitará un "enter" adicional después del 0.
* El sistema está protegido contra errores de tipeo: si ingresa un símbolo incorrecto por accidente, la consola ignorará la entrada y volverá a iterar. Esto solo aplica para los menús. En caso de ingresar una ruta de archivo inválida, el sistema informará que no fue encontrada y podrá volver a intentar.

**Menú Principal y Nuevas Funciones**

El núcleo del SO se controla a través de un Menú Principal con las siguientes opciones:

**1. Administración de usuarios y perfiles**

_Nota de seguridad:_ Solo los perfiles con la opción 1 dentro de sus permisos tienen acceso a la administración de usuarios y perfiles. A los demás perfiles se les denegará el acceso.

**Módulo de Usuarios**

* **Crear:** El sistema le asignará un ID (número de identificación) de forma automática. Solo debe ingresar el nombre, credenciales y asegurarse de asignarle un perfil que **ya exista** en el sistema (por ejemplo, GENERAL). Finalmente, presione 1 para confirmar y guardar.
* **Eliminar:** Necesitará el ID numérico exacto del usuario. Se recomienda usar primero la opción "Listar Usuarios" para anotar el ID correcto antes de proceder a la eliminación.

**Módulo de Perfiles**

* **Crear:** Ingrese el nombre del nuevo rol; el sistema lo convertirá a mayúsculas automáticamente. Para los permisos, escriba los números correspondientes a los accesos deseados, presionando Enter tras cada uno. Cuando termine de asignar permisos, escriba 0 y presione Enter para finalizar la configuración.
* **Eliminar:** Escriba el nombre exacto del perfil que desea borrar. _Nota de seguridad:_ Los perfiles fundacionales ADMIN y GENERAL están blindados por el núcleo del sistema y no pueden ser eliminados bajo ninguna circunstancia.

**2. Multiplicación de matrices NxM**

Si ingresa a esta opción, se le solicitará ingresar:

1. La ruta de la Matriz A (ej. `BD/Matrices/M1.TXT`).
2. La ruta de la Matriz B (ej. `BD/Matrices/N1.TXT`).
3. El carácter separador (ej. `#`).

El programa procesará esta operación invocando dinámicamente un ejecutable externo (`multi`), logrando así la multiplicación de matrices en un subproceso aislado.

**3. Juego**

En construcción. Funcionalidad futura.

**4. ¿Es palíndromo?**

Recibe una cadena de caracteres ingresada por el usuario. Omite espacios y trata mayúsculas y minúsculas por igual. Conserva caracteres numéricos y símbolos especiales. Recorre la cadena desde los extremos hasta el centro verificando que sea palíndromo. Devuelve falso para una cadena vacía y verdadero para un caracter solo. Ingrese 0 para salir.

**5. Calcular f(x)=x*x + 2x + 8**

Recibe una entrada numérica (preimagen) y devuelve la función evaluada en ella (imagen). Entrada no numérica para salir.

**6. Conteo sobre texto**

Accede a la ruta de archivo de texto proporcionada como último argumento en el inicio de sesión. Si no la encuentra, informa de ello y se devuelve. Si encuentra el archivo, realiza un conteo secuencial de vocales, consonantes, símbolos especiales y palabras totales. Los caracteres numéricos no se cuentan individualmente, pero pertenecen a las palabras válidas. Ingrese 0 para salir.

**7. Conteo sobre archivo**

Realiza el mismo procedimiento de la opción 6, pero sobre una nueva ruta especificada por el usuario en el momento. Ingrese 0 para salir.
