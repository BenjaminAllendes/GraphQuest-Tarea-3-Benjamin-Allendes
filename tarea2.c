#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct 
{
  char name[100] ;
  int peso ;
  int valor ;
} Item;


typedef struct  {
  char id[100];
  char name[100] ;
  char left[5], right[5], up[5], down[5] ;
  char descripcion[1000] ;
  Map *items ;
  char esFinal ;
  List *states_adj;
} State;

typedef struct {
  Map *carry_items ;
  int time ;
  int peso ;
  
} Character ;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("geimu");
  puts("========================================");

  puts("1) Cargar laberinto") ;
  puts("2) Iniciar partida") ;
  puts("3) Salir del juego") ;
}

int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Carga películas desde un archivo CSV y las almacena en un mapa por ID.
 */
void cargar_laberinto(Map *grafo) {
  // Intenta abrir el archivo CSV que contiene datos de películas
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

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    State *estado = (State *)malloc(sizeof(State));
    strcpy(estado->id, campos[0]) ; // Asigna ID
    strcpy(estado->name, campos[1]) ;
    strcpy(estado->descripcion, campos[2]) ;
    strcpy(estado->up, campos[4]) ;
    strcpy(estado->down, campos[5]) ;
    strcpy(estado->left, campos[6]) ;
    strcpy(estado->right, campos[7]) ;
    if (strcmp(campos[8], "Si") == 0) {
      estado->esFinal = 's' ;
      map_insert(grafo, estado->id, estado) ;      
      continue ;
    }
    else estado->esFinal = 'n' ;
    printf("Xd") ;
    List *list_adjs = list_create() ;
    estado->states_adj = list_adjs ;
    
    Item *objeto = (Item *) malloc(sizeof(Item)) ;
    Map *lista_objetos = map_create(is_equal_str) ;
    
    List *objetos_throw = split_string(campos[3], ";") ;
    char *string = list_first(objetos_throw) ;
    while (string != NULL){
      List *objetos_throw_dos = split_string(string, ",") ;
      char *string_dos = list_first(objetos_throw_dos) ; 
      map_insert(lista_objetos, string_dos, objeto) ;
      string_dos = list_next(objetos_throw_dos) ;
      objeto->peso = atoi(string_dos) ;
      string = list_next(objetos_throw) ;
      objeto->valor = atoi(string_dos) ;

      

      string = list_next(objetos_throw) ;
      free(objetos_throw_dos) ;
    }
    free(objetos_throw) ;

    estado->items = lista_objetos ;
    
    map_insert(grafo, estado->id, estado) ;
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas
  MapPair *par_grafo = map_first(grafo) ;
  State *est_aux = par_grafo->value ;

  MapPair *par ;
  while(par_grafo != NULL){
    printf("%s", est_aux->descripcion) ;
    if (atoi(est_aux->left) != -1) {
      par = map_search(grafo, est_aux->left) ;
      list_pushBack(est_aux->states_adj, par->value) ;
    }
    if (atoi(est_aux->right) != -1) {
      par = map_search(grafo, est_aux->right) ;
      list_pushBack(est_aux->states_adj, par->value) ;
    }
    if (atoi(est_aux->up) != -1) {
      par = map_search(grafo, est_aux->up) ;
      list_pushBack(est_aux->states_adj, par->value) ;
    }
    if (atoi(est_aux->down) != -1) {
      par = map_search(grafo, est_aux->down) ;
      list_pushBack(est_aux->states_adj, par->value) ;
    }
    par_grafo = map_next(grafo) ;
    if (par_grafo != NULL) est_aux = par_grafo->value ; ;
    
  } 
  printf("Laberinto cargado correctamente!") ;

}

Character * estado_incial(){
  Character *pj = (Character *) malloc(sizeof(Character)) ;
  pj->time = 10 ;
  pj->carry_items = map_create(is_equal_str) ;
  pj->peso = 0 ;
  return pj ;
}

void mostrar_items(Character *pj) {
  MapPair *par = map_first(pj->carry_items) ;
  
  if (par == NULL) {
    printf("No tienes ningun item en tu inventario ") ;
    return ;
  }
  Item * aux = par->value ; 
  while (par != NULL){
    printf("[Item: %s, peso %d, valor %d] ", aux->name, aux->peso, aux->valor) ;
    par = map_next(pj->carry_items) ;
    if (par != NULL) aux = par->value ;
  }
  printf("\n") ;
  return ;
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
  return ;
}

void menuOpciones(){
  puts("1) Recoger item") ;
  puts("2) Descartar item(s)") ;
  puts("3) Avanzar en una direccion") ;
  puts("4) Reiniciar partida") ;
  puts("5) Salir del juego") ;
}

void recoger_objeto(State *act,Character *pj) {
  char nombre_item[100] ;
  scanf(" %[^\n]s", nombre_item) ;
  MapPair *par = map_search(act->items, nombre_item) ;
  if (par != NULL){
    if (map_search(pj->carry_items, par->key) != NULL){
      printf("Ya tienes ese objeto en tu inventario!") ;
      presioneTeclaParaContinuar() ;
      limpiarPantalla() ;
    }
    map_insert(pj->carry_items, par->key, par->value) ;
    pj->time -= 1 ;
  }
  
}

void descartar_item(State *act,Character *pj) {
  char nombre_item[100] ;
  scanf(" %[^\n]s", nombre_item) ;
  MapPair *par = map_search(pj->carry_items, nombre_item) ;
  if (par != NULL){
    MapPair *par_dos =  map_remove(pj->carry_items, nombre_item) ;
    printf("Objeto '%s' eliminado", nombre_item) ;
  }
  else {
    printf("No tienes este objeto...") ;
  }
  presioneTeclaParaContinuar() ;
  limpiarPantalla() ;
}

void avanzar_direccion(State *act,Character *pj){
  
}

void iniciar_partida(Map *grafo) {
  MapPair *par = map_search(grafo,"1") ;
  State *actual = par->value ;

  char opcion ;
  int game_on = 1 ;

  Character *personaje = estado_incial() ;
  puts("Empezando el juego...") ;
  
  while (game_on == 1){
    if (actual->esFinal) game_win(actual, personaje) ;
    puts("Estado actual:") ;
    printf("Escenario: %s \n", actual->name) ;
    printf("Descripcion del escenario: %s \n", actual->descripcion) ;
    printf("Tiempo restante = %d \n", personaje->time) ;
    mostrar_items(personaje) ;
    printf("Direcciones posibles: ") ;
    posibles_direcciones(actual) ;
    
    do {
      printf("\n Opciones: \n") ;
      
      menuOpciones() ;
      scanf(" %c", &opcion) ;
      switch(opcion){
        case '1' :
          recoger_objeto(actual, personaje) ;
          break ;
        
        case '2' :
          descartar_item(actual, personaje) ;
          break ;
        case '3' :
          avanzar_direccion(actual, personaje) ;
          break ;
        case '4' :
          break ;
        case '5' :
          game_on = 0 ;
          break ;
      }
    }while (opcion != '5' && opcion != '3') ;
    
  
  }
  puts("Fin del juego") ;
  return ;
}
int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
               // (sin uso en este fragmento)

  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  Map *grafo = map_create(is_equal_str) ;
  

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_laberinto(grafo);
      break;
    case '2':
      iniciar_partida(grafo);
      break;
    case '3':
      printf("Saliendo...") ;
      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '3');

  return 0;
}
