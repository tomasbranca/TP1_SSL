#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PRODUCCIONES 10

// Definicion de la estructura de produccion 
typedef struct{
  char ladoIzq;
  char ladoDer[2 + 1]; // En gramática regular, el lado derecho puede tener un máximo de 2 caracteres (1 terminal + 1 no terminal), se suma uno para el caracter nulo
} produccion;

// Definicion de la estructura de gramatica
typedef struct{
  char VN[5 + 1];
  char VT[5 + 1];
  char axioma;
  produccion producciones[MAX_PRODUCCIONES];
} gramatica;

// Prototipos de funciones
int estaEnLaLista(char *lista, char simbolo);
void cargarVariables(gramatica *g);
int esSimboloValido(char *N, char *T, char simbolo);
int esProduccionValida(gramatica *g, char *ladoDer);
void cargarProducciones(gramatica *g);
void imprimirCuatrupla(gramatica *g);
void limpiarConsola();

int main(){
  gramatica g;
  cargarVariables(&g); 
  limpiarConsola(); // Limpiar pantalla
  cargarProducciones(&g);
  limpiarConsola();
  printf("Gramatica cargada con exito.\n");
  imprimirCuatrupla(&g);

  // TODO: Falta realizar la funcion encargada de la generacion de palabras en base a la gramatica ingresada

  printf("\n\nPresione ENTER para salir...");
  getchar(); 
  getchar(); // Espera a que el usuario presione ENTER
  return 0;
}

// Funcion para verificar si un simbolo esta en una lista
int estaEnLaLista(char *lista, char simbolo){
  for (int i = 0; i < strlen(lista); i++){
    if (lista[i] == simbolo){
      return 1; // El simbolo esta en la lista
    }
  }
  return 0; // El simbolo no esta en la lista
}

// Funcion para cargar las variables de la gramatica
void cargarVariables(gramatica *g){
  printf("Ingrese hasta 5 NO TERMINALES de la gramatica (Ej: ABCDE): ");
  scanf("%s", g->VN);
  printf("\nIngrese hasta 5 TERMINALES de la gramatica (Ej: abcdf). (el simbolo 'e' se reserva para no terminal vacío).: ");
  scanf("%s", g->VT);
}

// Funcion para verificar si un simbolo(no terminal o terminal) es valido para el lado izq. de la produccion
int esSimboloValido(char *N, char *T, char simbolo){
  return (estaEnLaLista(N, simbolo) || estaEnLaLista(T, simbolo) || simbolo == 'e'); // 1 si es valido, 0 si no lo es
}

int esProduccionValida(gramatica *g, char *ladoDer){
  int len = strlen(ladoDer);

  // Verifica que el lado derecho de la produccion sea mayor a 0
  if (len == 0) {
    printf("Producción no valida: está vacía. Si quería referirse a un no terminal vacío, puede usar 'e' (para representar 'ε').\n");
    return 0; // No es una producción válida
  }

  /** Verificaciones sobre si es una gramática regular **/
  // Verifica que el lado derecho de la producción tenga como máximo 2 símbolos
  if (len > 2) {
    printf("Producción no válida: no genera una gramática regular. El lado derecho debe tener como máximo 2 simbolos.\n");
    return 0; // No es una producción válida
  }

  // Si hay un solo símbolo, verifica que sea un terminal
  if (len == 1) {
    char c = ladoDer[0];
    if (c == 'e') return 1; // Si es el símbolo vacío, es válido
    if (!estaEnLaLista(g->VT, c)) {
      printf("Producción no válida: símbolo '%c' no es un terminal válido.\n", c);
      return 0; // No es una producción válida
    }
    return 1; // Es una producción válida
  }

  // CHECK: Si queres hacer que tambien pueda ser al reves, implementar con esSimboloValido desde el principio, pero veo problemas para generar las palabras despues
  // Si hay dos símbolos, verifica que el primero sea un terminal y el segundo un no terminal
  if (len == 2) {
    // Verifica que el primer símbolo sea un terminal
    if (!estaEnLaLista(g->VT, ladoDer[0])) {
      printf("Producción no válida: primer símbolo '%c' no es un terminal válido.\n", ladoDer[0]);
      return 0; // No es una producción válida
    }

    // Verifica que el segundo símbolo sea un no terminal
    if (!estaEnLaLista(g->VN, ladoDer[1])) {
      printf("Producción no válida: segundo símbolo '%c' no es un no terminal válido.\n", ladoDer[1]);
      return 0; // No es una producción válida
    }

    return 1; // Es una producción válida
  }

  // Si no cae en ningún caso, no es válida
  printf("Producción no válida: formato incorrecto.\n");
  return 0;
}

void cargarProducciones(gramatica *g){
  int i = 0, termino = 0;
  char continuar = 's';
  char noTerminal;
  char ladoDer[2 + 1];
  char inputProduccion[MAX_PRODUCCIONES + 8]; // para toda la línea con '|'

  while (i < MAX_PRODUCCIONES && !termino){
    printf("NO TERMINALES: %s\n", g->VN);
    printf("Ingrese el NO TERMINAL de la produccion %d: ", i + 1);
    scanf(" %c", &noTerminal);

    // Verifica si el NO TERMINAL ingresado es valido
    while (!estaEnLaLista(g->VN, noTerminal)){
      printf("El NO TERMINAL no es valido\n\n");
      printf("Ingrese el NO TERMINAL de la produccion %d: ", i + 1);
      scanf(" %c", &noTerminal);
    }

    if (i == 0){
      g->axioma = noTerminal; // El primer NO TERMINAL es el axioma
    }

    limpiarConsola();

    printf("%c -> ...\n\n", noTerminal);
    printf("NO TERMINALES: %s\n", g->VN);
    printf("TERMINALES: %s\n", g->VT);
    printf("Ingrese el lado derecho de la produccion %d (puede contener '|', máx. 10 caracteres en total): ", i + 1);
    scanf("%50s", inputProduccion);

    char *token = strtok(inputProduccion, "|");
    while (token != NULL && i < MAX_PRODUCCIONES) {
      if (strlen(token) > 2) {
        printf("Producción '%s' demasiado larga (máx. 2 caracteres para gramática regular).\n", token);
      } else if (!esProduccionValida(g, token)) {
        printf("Producción '%s' no válida. Se descartará.\n", token);
      } else {
        g->producciones[i].ladoIzq = noTerminal;
        strcpy(g->producciones[i].ladoDer, token);
        printf("Produccion %d cargada: %c -> %s\n", i + 1, noTerminal, token);
        i++;
      }

      token = strtok(NULL, "|");
    }

    if (i >= MAX_PRODUCCIONES) {
      printf("\nSe alcanzó el máximo de producciones permitidas (%d).\n", MAX_PRODUCCIONES);
      break;
    }

    printf("Desea ingresar otra produccion? (s/n): ");
    scanf(" %c", &continuar);
    if (continuar != 's' && continuar != 'S'){
      termino = 1;
    }

    limpiarConsola();
  }
}

void imprimirCuatrupla(gramatica *g){
  printf("G = (");

  // Imprime los NO TERMINALES
  printf("{");
  for (int i = 0; g->VN[i] != '\0'; i++) {
      printf("%c", g->VN[i]);
      if (g->VN[i + 1] != '\0') printf(", ");
  }
  printf("}, ");

  // Imprime los TERMINALES
  printf("{");
  for (int i = 0; g->VT[i] != '\0'; i++) {
    printf("%c", g->VT[i]);
    if (g->VT[i + 1] != '\0') printf(", ");
  }
  printf("}, ");

  // Imprime las producciones
  printf("{");
  for (int i = 0; i < 10 && g->producciones[i].ladoIzq != '\0'; i++) {
    printf("%c->%s", g->producciones[i].ladoIzq, g->producciones[i].ladoDer);
    if (g->producciones[i+1].ladoIzq != '\0') printf(", ");
  }
  printf("}, ");

  // Imprime el axioma
  printf("%c", g->axioma);

  printf(")\n");
}

// Esta función es una especie de wrapper para limpiar la consola
void limpiarConsola() {
    #ifdef _WIN32
        system("cls"); // En Windows se usa "cls"
    #else
        system("clear"); // En Linux y MacOS se usa "clear"
    #endif
}