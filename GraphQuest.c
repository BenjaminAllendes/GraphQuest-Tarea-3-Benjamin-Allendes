#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct 
{
  char name[100] ;
  int peso ;
  int valor ;
} Item;


typedef struct  {
  char id[10];
  char name[100] ;
  char left[10], right[10], up[10], down[10] ;
  char descripcion[1000] ;
  Map *items ;
  char esFinal ;
  Map *states_adj;
} State;

typedef struct {
  Map *carry_items ;
  int time ;
  int peso ;
  int pts ;
  
} Character ;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("               GraphQuest               ");
  puts("========================================");

  puts("1) Cargar laberinto") ;
  puts("2) Iniciar partida") ;
  puts("3) Salir del juego") ;
}

int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}


// Carga el grafo desde un archivo CSV y las almacena en un mapa por ID.
void cargar_laberinto(Map *grafo) {
  FILE *archivo = fopen("data/graphquest.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }
  char **campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  List *grafo_list = list_create() ; // Se crea una lista aparte para guardar los nodos
  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    State *estado = (State *)malloc(sizeof(State)); // Se almacena memoria para cada escenario (nodo)
    // Se guardan datos en el estado 
    strcpy(estado->id, campos[0]) ;
    strcpy(estado->name, campos[1]) ;
    strcpy(estado->descripcion, campos[2]) ;
    strcpy(estado->up, campos[4]) ;
    strcpy(estado->down, campos[5]) ;
    strcpy(estado->left, campos[6]) ;
    strcpy(estado->right ,campos[7]) ;
    if (strcmp(campos[8], "Si") == 0) {
      estado->esFinal = 's' ;
    }
    else estado->esFinal = 'n' ;

    // Se crea el mapa de nodos adyacentes
    estado->states_adj = map_create(is_equal_str)  ;
    char *string ;

    // Se crea el mapa de objetos la cual tendra todos los objetos del escenario.
    Map *lista_objetos = map_create(is_equal_str) ;
    // Se crea una lista de strings los cuales van a ser divididos por el caracter ';'.
    List *objetos_throw = split_string(campos[3], ";") ;
    string = list_first(objetos_throw) ; // Se revisa el primer string de esta lista creada
     
    // Loop para revisar todos los objetos
    while (string != NULL){
      Item *objeto = (Item *) malloc(sizeof(Item)) ; // Se reserva memoria para el objeto
      List *objetos_throw_dos = split_string(string, ",") ; // Lista dividida por el caracter ','
      char *string_dos = list_first(objetos_throw_dos) ; 
      // Si el string de la posicion en 3 no hay ningun objeto y se termina el ciclo
      if (strcmp(campos[3], "nan") == 0){
        free(objeto) ;
        free(objetos_throw_dos) ; 
        lista_objetos = NULL ;
        break ;
      }
      // De lo contrario, se va "recorriendo" la lista para guardar los 3 datos del objeto.
      map_insert(lista_objetos, string_dos, objeto) ;
      string_dos = list_next(objetos_throw_dos) ;
      objeto->peso = atoi(string_dos) ;
      string = list_next(objetos_throw_dos) ;
      objeto->valor = atoi(string_dos) ;
      
      string = list_next(objetos_throw) ;
      free(objetos_throw_dos) ;
    }
    free(objetos_throw) ;

    estado->items = lista_objetos ;
    map_insert(grafo, estado->id, estado) ; // Se inserta el escenario en el grafo.
    list_pushBack(grafo_list, estado) ; 
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas
  State *est_aux = list_first(grafo_list) ; // Primer escenario de la lista aparte 

  MapPair *par ;

  // Loop para recorrer lista de escenarios y llenar el mapa de escenarios adyacentes de cada uno
  while(est_aux != NULL){
    
    if (atoi(est_aux->left) != -1) {
      par = map_search(grafo, est_aux->left) ;
      map_insert(est_aux->states_adj, "Izqui", par->value) ;
    }
    if (atoi(est_aux->right) != -1) {
      par = map_search(grafo, est_aux->right) ;
      State *holi = par->value ;
      map_insert(est_aux->states_adj,"Dere",holi) ;
      par = map_search(est_aux->states_adj, "Dere");
      
    }
    if (atoi(est_aux->up) != -1) {
      par = map_search(grafo, est_aux->up) ;
      map_insert(est_aux->states_adj, "Arri", par->value) ;
    }
    if (atoi(est_aux->down) != -1) {
      par = map_search(grafo, est_aux->down) ;
      map_insert(est_aux->states_adj, "Aba", par->value) ;
    }
    est_aux = list_next(grafo_list) ;
    
  } 
  printf("Laberinto cargado correctamente! \n") ;
}

// Funcion donde se reserva memoria para el personaje y se le asignan los valores iniciales
// Tiempo parte en 10, se crea un mapa vacio de objetos (inventario) y peso y puntaje parten en 0
Character * estado_incial(){
  Character *pj = (Character *) malloc(sizeof(Character)) ;
  pj->time = 10 ;
  pj->carry_items = map_create(is_equal_str) ;
  pj->peso = 0 ;
  pj->pts = 0;
  return pj ;
}

/*
Funcion para mostrar los objetos que el personaje lleva actualmenete. Si no tiene nada simplemente
se muestra esto, si no se muestran todos los objetos que tiene con su informacion correspondiente
*/
void mostrar_items_pj(Character *pj) {
  MapPair *par = map_first(pj->carry_items) ;
  
  if (par == NULL) {
    printf("No tienes ningun item en tu inventario \n") ;
    return ;
  }
  Item * aux = par->value ;
  printf("Inventario: ") ;
  while (par != NULL){
    printf("[Item: %s, peso %d, valor %d] ", par->key, aux->peso, aux->valor) ;
    par = map_next(pj->carry_items) ;
    if (par != NULL) aux = par->value ;
  }
  printf("\n") ;
  return ;
}



void mostrar_items_escenario(State* act, Character *pj) {
  if (act ->items == NULL){
    printf("No hay objetos en esta sala... \n") ;
    return ;
  }

  MapPair *par = map_first(act->items) ;
  int carry_all = 1 ; 
  printf("Items en este escenario: ") ;
  while (par != NULL){
    if (map_search(pj->carry_items, par->key) != NULL) { 
      par = map_next(act->items) ;
      continue ;
    }
    carry_all = 0 ; // El jugador no tiene todos los objetos del escenario
    Item *aux = par->value ;
    printf("[Item: %s, peso %d, valor %d] ", par->key, aux->peso, aux->valor) ;
    par = map_next(act->items) ;
  }
  if (carry_all) printf("Ya tienes todos los items de esta sala.") ;
  printf("\n") ;
}

void posibles_direcciones(State *act){
  if (atoi(act->up) != -1) printf("Arriba ") ;
  if (atoi(act->down) != -1) printf("Abajo ") ;
  if (atoi(act->left) != -1) printf("Izquierda ") ;
  if (atoi(act->right) != -1) printf("Derecha") ;
  printf("\n") ;
  return ;
}

void game_win(State *act, Character *pj){
  printf("Llegaste al final del laberinto. Ganaste! \n") ;
  printf("Inventario final: ") ;
  mostrar_items_pj(pj) ;
  printf("Puntaje final: %d puntos \n", pj->pts) ;
  
  return ;
}

void menuOpciones(){
  puts("1) Recoger item") ;
  puts("2) Descartar item") ;
  puts("3) Avanzar en una direccion") ;
  puts("4) Reiniciar partida") ;
  puts("5) Salir del juego") ;
}
/*
Fucnion para recoger un objeto insertado en el teclado solo si existe el objeto en el escenario actual, 
si la sala tienes objetos todavia sin agarrar, y si esta no estaba vacia desde un principio.
*/
void recoger_objeto(State *act,Character *pj) {
  limpiarPantalla() ;
  if (act->items == NULL) {
    printf("No hay objetos en esta sala... \n") ;
    return ; 
  }
  printf("Items: ") ;
  mostrar_items_escenario(act, pj) ;
  printf("Ingresa el objeto que quieres recoger: ") ;
  char nombre_item[100] ;
  scanf(" %[^\n]s", nombre_item) ;
  MapPair *par = map_search(act->items, nombre_item) ;
  
  if (par != NULL){
    Item *objeto = par->value ;
    if (map_search(pj->carry_items, par->key) != NULL){
      printf("Ya tienes ese objeto en tu inventario! \n") ;
    }
    else {
      map_insert(pj->carry_items, par->key, objeto) ;
      printf("Objeto '%s' obtenido! \n", par->key) ;
      pj->time -= 1 ;
      pj->peso += objeto->peso ;
      pj->pts += objeto->valor ;
    }
  }
  else printf("Ese objeto no esta en el escenario \n") ;

}

/*
Funcion para descartar el objeto que esta el invetario del personaje. 
*/
void descartar_item(State *act,Character *pj) {
  limpiarPantalla() ;
  if (act->items == NULL) {
    printf("No hay objetos en esta sala... \n") ;
    return ; 
  }
  printf("Inventario: ") ;
  mostrar_items_pj(pj) ;
  printf("Ingresa el objeto que quieres descartar: ") ;
  char nombre_item[100] ;
  scanf(" %[^\n]s", nombre_item) ;
  MapPair *par = map_search(pj->carry_items, nombre_item) ;
  
  if (par != NULL){
    MapPair *par_dos = map_remove(pj->carry_items, nombre_item) ;
    printf("Objeto '%s' eliminado \n", nombre_item) ;
    Item *objeto = par->value ;
    pj->peso -= objeto->peso ;
    pj->pts -= objeto->valor ;
  }
  else {
    printf("No tienes este objeto... \n") ;
  }
}

void menu_direcciones(){
  puts("Direcciones:") ;
  puts("1) Arriba") ;
  puts("2) Abajo") ;
  puts("3) Izquierda") ;
  puts("4) Derecha") ;
}
// Funcion para cambiar el escenario actual a uno adyacente. (Es decir, moverse en el laberinto).
State * avanzar_direccion(State *act,Character *pj){
  limpiarPantalla() ;
  char direccion ;
  menu_direcciones() ;
  printf("Ingresa para donde quieres avanzar: ") ;
  scanf(" %c", &direccion) ;
  int se_movio = 0 ;
  MapPair *par ;

  // Si la direccion es valida el nodo act se actualiza y se retorna. De lo contrario queda igual
  switch (direccion)
  {
  case '1':
    if (atoi(act->up) != -1) {
      par = map_search(act->states_adj, "Arri") ;
      act = par->value ;
      se_movio = 1 ;
    }
    else {
      printf("No hay camino en esa direccion...") ;
    }
    break ;
  case '2' :
    if (atoi(act->down) != -1) {
      par = map_search(act->states_adj, "Aba") ;
      act = par->value ;
      se_movio = 1 ;
    }
    else {
      printf("No hay camino en esa direccion...") ;
    }
    break ;
  
  case '3' :
    if (atoi(act->left) != -1) {
      par = map_search(act->states_adj, "Izqui") ;
      act = par->value ;
      se_movio = 1 ;
    }
    else {
      printf("No hay camino en esa direccion...") ;
    }
    break ;
  case '4' :
    if (atoi(act->right) != -1) {
      par = map_search(act->states_adj,"Dere") ;
      act = par->value ;
      se_movio = 1 ;
    }
    else {
      printf("No hay camino en esa direccion...") ;
    }
    break ;
  default :
    printf("Opcion no valida") ;
    break ;
  }

  // Si el escenario cambia se resta el tiempo
  if (se_movio == 1) {
    double resta = ceil((1 + pj->peso) / 10.0) ;
    pj->time -= (int) resta  ;
  }
  return act ;
}

// Funcion para volver al estado incicial. Personaje y escenario pasan a su estado incial. 
// Se retorna escenario inicial
State * reiniciar_partida(Map *grafo, State *act,Character *pj){
  free(pj->carry_items) ;
  MapPair *par = map_search(grafo,"1") ;
  act = par->value ;
  free(pj) ;
  return act ;
}

/* Funcion donde se inicia la partida y se realiza el menu principal del juego hasta
que se llegue a una condiciones de fin del juego.
*/
void iniciar_partida(Map *grafo) {
  MapPair *par = map_search(grafo, "1") ;
  State *actual = par->value ;

  char opcion ;
  int game_on = 1 ; // Funcion para terminar el juego.

  Character *personaje = estado_incial() ;
  limpiarPantalla(); 
  puts("Empezando el juego...") ;
  
  while (game_on == 1){
    if (personaje->time <= 0) {
      printf("Te quedaste sin tiempo! \n") ;
      game_on = 0 ;
      break ;
    }
    if (actual->esFinal == 's') {
      game_win(actual, personaje) ;
      break ;
    }
    
    
    do {
      puts("Estado actual:") ;
      printf("Escenario: %s \n", actual->name) ;
      printf("Descripcion del escenario: %s \n", actual->descripcion) ;
      printf("Tiempo restante = %d \n", personaje->time) ;
      mostrar_items_pj(personaje) ;
      mostrar_items_escenario(actual, personaje) ;
      printf("Direcciones posibles: ") ;
      posibles_direcciones(actual) ;
      printf("\n Opciones: \n") ;
      
      menuOpciones() ;
      printf("Ingresa tu opcion : ") ;
      scanf(" %c", &opcion) ;
      
      switch(opcion){
        case '1' :
          recoger_objeto(actual, personaje) ;
          break ;
        
        case '2' :
          descartar_item(actual, personaje) ;
          break ;
        case '3' :
          actual = avanzar_direccion(actual, personaje) ;
          break ;
        case '4' :
          actual = reiniciar_partida(grafo, actual, personaje) ;
          personaje = estado_incial() ;
          break ;
        case '5' :
          game_on = 0 ;
          break ;
      }
      presioneTeclaParaContinuar() ;
      limpiarPantalla() ;
    }while (opcion != '5' && opcion != '3' && opcion != '4') ;
    
  
  }
  puts("Fin del juego") ;
  return ;
}
int main() {
  char opcion; 

  // Crea un mapa el cual tendra todos los escenarios (grafo).
  Map *grafo = map_create(is_equal_str) ;
  

  int cargado = 0 ;
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opcion: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_laberinto(grafo);
      cargado = 1 ;
      break;
    case '2':
      if (cargado == 0) printf("El laberinto todavia no ha sido cargado. \n") ;
      else iniciar_partida(grafo);
      break;
    case '3':
      printf("Saliendo...") ;
      break;
    }
    presioneTeclaParaContinuar();
    limpiarPantalla() ;

  } while (opcion != '3');

  return 0;
}
