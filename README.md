# GraphQuest

¡Bienvenido a GraphQuest! Este es un juego donde tendras que recorrer un laberinto hasta llegar a la salida. Tendras que ir atravesando distintos escenarios hasta poder encontrar la ruta correcta, sumado a tener que llegar en un tiempo limitado. Ademas existe un sistema de objetos y puntaje. Algunos escenarios contienen objetos que tienen un valor el cual aumentaran tu puntaje final, pero tambien tienen un peso que haran que moverte por el escenario sea mas dificil y por lo tanto gaste mas tiempo. ¿Te atreves a aventurarte?

Los escenarios se cargan de un archivo CSV el cual viene incluido.

## Cómo compilar y ejecutar

Este sistema ha sido desarrollado en lenguaje C y puede ejecutarse fácilmente utilizando **Visual Studio Code** junto con una extensión para C/C++, como **C/C++ Extension Pack de Microsoft**. Para comenzar a trabajar con el sistema en tu equipo local, sigue estos pasos:

## Requisitos previos:

- Tener instalado Visual Studio Code.
- Instalar la extensión **C/C++** (Microsoft).
- Tener instalado un compilador de C (como gcc). Si estás en Windows, se recomienda instalar MinGW o utilizar el entorno WSL.

## Pasos para compilar y ejecutar:

1. Descarga y descomprime el archivo `.zip` en una carpeta de tu elección.
2. Abre el proyecto en Visual Studio Code
    -Inicia Visual Studio Code.
    -Selecciona `Archivo > Abrir carpeta...` y elige la carpeta donde descomprimiste el proyecto.
3. Compila el código
    - Abre el archivo principal (`GraphQuest.c`).
    - Abre la terminal integrada (`Terminal > Nueva terminal`).
    - En la terminal, compila el programa con el siguiente comando:
    ```
    gcc tdas/*.c GraphQuest.c -Wno-unused-result -o GraphQuest
    ```
4. Ejecuta el programa
    - Una vez compilado, puedes ejecutar la aplicación con:
    ```
    ./GraphQuest
    ```

## Funcionalidades

### Funcionando correctamente:
- Cargar escenarios de un archivo CSV y guardar su informacion correspondiente.
- Poder moverse por los diferentes escenarios conectados.
- Sistema de objetos e inventario. Se puede recoger y descartar objetos que estan en los escenarios.
- Sistema de tiempo limitado por cada partida (tiempo in-game, no real).

### Problemas conocidos:
- Terminal tiene problemas con el caracter ñ.

### A mejorar:
- Añadir compatibilidad con letras del teclado español
- Implementar una interfaz mas amigable.
- Añadir arte para cada escenario y objetos. 

## Ejemplo de uso
### Paso 1: Cargar el laberinto
En `GraphQuest.c` se ingresa la ruta de la ruta del arhcivo CSV dentro de la carpeta del programa  de donde se quiera cargar el laberinto (por ejemplo `graphquest.csv`), se guardan la informacion de los escenarios.

```
54  FILE *archivo = fopen("RUTA DEL ARCHIVO", "r");
```

### Paso 2: Empezar la partida
Luego de cargar el laberinto, se puede iniciar la partida. Se selecciona la opcion 2 y se empieza en la entrada principal.
```
Empezando el juego...
Estado actual:
Escenario: Entrada principal
Descripcion del escenario: Una puerta rechinante abre paso a esta mansion olvidada por los dioses y los conserjes. El aire huele a humedad y a misterios sin resolver.
Tiempo restante = 10.00
No tienes ningun item en tu inventario
No hay objetos en esta sala...
Direcciones posibles: Abajo

 Opciones:
1) Recoger item
2) Descartar item
3) Avanzar en una direccion
4) Reiniciar partida
5) Salir del juego
Ingresa tu opcion :
```

### Paso 3: Avanzar en una direccion
Ahora puedes empezar a moverte por el laberinto solo por las direcciones posibles. Para esto hay que ingresar la opcion 3 y la direccion a la cual quieres ir.
```
Direcciones:
1) Arriba
2) Abajo
3) Izquierda
4) Derecha
Ingresa para donde quieres avanzar: 2
```
```
Estado actual:
Escenario: Cocina
Descripcion del escenario: Restos de una batalla culinaria. Cuchillos por aqui, migajas por alla. Alguien cocinoÔÇª o fue cocinado.  
Tiempo restante = 9.90
No tienes ningun item en tu inventario
Items en este escenario: [Item: Cuchillo, peso 3, valor 3] [Item: Pan, peso 2, valor 2]
Direcciones posibles: Arriba Abajo Derecha
```

### Paso 4: Recoger objeto
Puedes recoger objetos si un escenario contiene uno o varios de estos. Para esto hay que ingresar la opcion 1 y el item que quieras recoger. Una vez recogido, el item pasa a estar en el inventario.
```
Items: Items en este escenario: [Item: Cuchillo, peso 3, valor 3] [Item: Pan, peso 2, valor 2]
Ingresa el objeto que quieres recoger: Cuchillo
Objeto 'Cuchillo' obtenido! 
Presione una tecla para continuar...
```
### Paso 5: Descartar objeto
Puedes descartar objetos del escenario si lo consideras necesario. Para esto hay que ingresar la opcion 2 y el item que quieras descartar. El item vuelve a su escenario original.
```
Inventario: Inventario: [Item: Cuchillo, peso 3, valor 3] [Item: Moneda, peso 2, valor 2]
Ingresa el objeto que quieres descartar: Cuchillo
Objeto 'Cuchillo' eliminado 
Presione una tecla para continuar...
```
### Paso 6: Ganar el juego
Para ganar GraphQuest solo tienes que encontrar la salida. Se mostrara tu puntaje e inventario finales.
(Pantalla de victoria)
```
Llegaste al final del laberinto. Ganaste!
Inventario final: Inventario: [Item: Moneda, peso 2, valor 2]
Puntaje final: 2 puntos
Fin del juego
Presione una tecla para continuar...
```
### Paso 7: Reiniciar el juego
En medio de una partida si quieres puedes empezar de cero con la opcion 4.
### Paso 8: Salir del juego
Se puede salir del juego con al opcion 5.