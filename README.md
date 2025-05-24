# GraphQuest

¡Bienvenido a GraphQuest! Este es un juego donde tendras que recorrer un laberinto hasta llegar a la salida. Tendras que ir recorriendo distintos escenarios hasta poder encontrar la ruta correcta, sumado a tener que llegar en un tiempo limitado. Ademas, existe un sistema de objetos y puntaje. Algunos escenarios contienen objetos que tienen un valor el cual aumentaran tu puntaje final, pero tambien tienen un peso que haran que moverte por el escenario sea mas dificil y por lo tanto gaste mas tiempo. ¿Te atreves a aventurarte?

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
- Cargar canciones de un archivo csv, guardar la informacion relevante de cada cancion y mostrar cada cancion guardada.
- Buscar canciones por genero y mostrar la informacion guardada de cada una.
- Buscar canciones por artista y mostrar la informacion guardada de cada una.
- Buscar canciones por tempo (lentas, moderadas y rapidas) y mostrar la informacion guardada de cada una.

### Problemas conocidos:
- Programa tiene problemas con algunos caracteres especiales como las tildes, causando algunos problemas visuales cuando una cancion contiene estos. 

### A mejorar:
- Compatibilidad con mas tipos de caracteres para evitar problemas con canciones que contengan estos.
- Implementar una interfaz mas amigable

## Ejemplo de uso
### Paso 1: Cargar canciones
En `spotfind.c` se ingresa la ruta de la ruta del arhcivo CSV dentro de la carpeta del programa  de donde se quiera cargar las canciones (por ejemplo `data/song_dataset_.csv`), se guardan la informacion de las canciones y se muestra cada cancion guardada.

```
60  FILE *archivo = fopen("RUTA DEL ARCHIVO", "r");
```

### Paso 2: Buscar por genero
Tras cargar las canciones, el usuario decide buscar todas las canciones de un genero que fueron cargadas.
```
Ingrese el genero de la cancion: acoustic
ID: 0, Titulo: Comedy, Artista: Gen Hoshino, Album: Comedy, Tempo: 87.917000
ID: 1, Titulo: Ghost - Acoustic, Artista: Ben Woodward, Album: Ghost (Acoustic), Tempo: 77.488998
ID: 2, Titulo: To Begin Again, Artista: Ingrid Michaelson;ZAYN, Album: To Begin Again, Tempo: 76.332001
ID: 3, Titulo: Can't Help Falling In Love, Artista: Kina Grannis, Album: Crazy Rich Asians (Original Motion Picture Soundtrack), Tempo: 181.740005
[La lista sigue hasta mostrar todas las canciones guardadas del mismo genero]
```

### Paso 3: Buscar por artista
El usuario busca todas las canciones de un mismo artista que fueron cargadas.
```
Ingrese el artista de la cancion: Weezer
ID : 2027, Titulo: Records, Album: Tek It - New Noise, Genero: alt-rock, Tempo: 100.005997 
ID : 2063, Titulo: Records, Album: Rock - Best of 2022, Genero: alt-rock, Tempo: 100.005997 
ID : 2064, Titulo: Say It Ain't So, Album: Los Cl├ísicos del Pop 90s, Genero: alt-rock, Tempo: 151.779007 
ID : 2065, Titulo: Say It Ain't So, Album: 90's Music Grandi Successi, Genero: alt-rock, Tempo: 151.779007
[La lista sigue hasta mostrar todas las canciones guardadas del mismo artista]
```

### Paso 4: Buscar por tempo
El usuario busca todas las canciones por un mismo rango de beats-per-seocn (BPM). Existen tres rangos, canciones lentas (<80 BPM), moderadas (80-120 BPM) y rapidas (120< BPM).
```
1) Lentas <80 BPM
2) Moderadas 80-120 BPM
3) Rapidas 120< BPM
Ingrese su opcion: 1
ID: 1,Titulo: Ghost - Acoustic, Artista: Ben Woodward, Album: Ghost (Acoustic), Genero: acoustic Tempo: 77.488998 
ID: 2,Titulo: To Begin Again, Artista: Ingrid Michaelson;ZAYN, Album: To Begin Again, Genero: acoustic Tempo: 76.332001 
ID: 9,Titulo: Hunger, Artista: Ross Copperman, Album: Hunger, Genero: acoustic Tempo: 78.899002
ID: 53,Titulo: A Drop in the Ocean, Artista: Ron Pope, Album: The Bedroom Demos, Genero: acoustic Tempo: 73.139000
ID: 102,Titulo: Pieces (Hushed), Artista: Andrew Belle, Album: Black Bear (Hushed), Genero: acoustic Tempo: 69.641998
ID: 103,Titulo: Mirrors, Artista: Boyce Avenue;Fifth Harmony, Album: Cover Sessions, Vol. 3, Genero: acoustic Tempo: 76.971001
[Lista sigue hasta mostrar todas las canciones lentas]
```
