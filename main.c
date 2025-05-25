#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definicion de la estructura de produccion 
typedef struct{
  char ladoIzq;
  char ladoDer[10 + 1];
} produccion;

// Definicion de la estructura de gramatica
typedef struct{
  char VN[5 + 1];
  char VT[5 + 1];
  char axioma;
  produccion producciones[10];
} gramatica;

// Prototipos de funciones
int estaEnLaLista(char *lista, char simbolo);
void cargarVariables(gramatica *g);
int esSimboloValido(char *N, char *T, char simbolo);
int esProduccionValida(gramatica *g, char *ladoDer);
void cargarProducciones(gramatica *g);
void imprimirCuatrupla(gramatica *g);

int main(){
  gramatica g;
  cargarVariables(&g); 
  system("cls"); // Limpiar pantalla
  cargarProducciones(&g);
  system("cls");
  printf("Gramatica cargada con exito.\n");
  imprimirCuatrupla(&g);

  // Falta realizar la funcion encargada de la generacion de palabras en base a la gramatica ingresada
  // system("cls") solo funciona en Windows, si se quiere usar en Linux o MacOS, cambiar por "clear". Hay que realizar una funcion para ello
  // (Opcional) Hacer una funcion para analizar si es GR o GIC

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
  printf("Ingrese hasta 5 NO TERMINALES max. de la gramatica (Ej: ABCDE): ");
  scanf("%s", g->VN);
  printf("\nIngrese hasta 5 TERMINALES max. de la gramatica (Ej: abcde): ");
  scanf("%s", g->VT);
}

// Funcion para verificar si un simbolo(no terminal o terminal) es valido para el lado izq. de la produccion
int esSimboloValido(char *N, char *T, char simbolo){
  return (estaEnLaLista(N, simbolo) || estaEnLaLista(T, simbolo)); // 1 si es valido, 0 si no lo es
}

int esProduccionValida(gramatica *g, char *ladoDer){
  for (int i = 0; i < strlen(ladoDer); i++){
    if (!esSimboloValido(g->VN, g->VT, ladoDer[i])){
      return 0; // No es una producción válida
    }
  }
  return 1; // Es una producción válida
}

void cargarProducciones(gramatica *g){
  int i = 0, termino = 0;
  char continuar = 's';
  char noTerminal, ladoDer[11];

  while (i < 10 && !termino){
    printf("NO TERMINALES: %s\n", g->VN);
    printf("Ingrese el NO TERMINAL de la produccion %d: ", i + 1);
    scanf(" %c", &noTerminal);

    // Verifica si el NO TERMINAL ingresado es valido
    while (!estaEnLaLista(g->VN, noTerminal)){
      printf("El NO TERMINAL no es valido\n\n");
      printf("Ingrese el NO TERMINAL de la produccion %d: ", i + 1);
      scanf(" %c", &noTerminal);
    }
    g->producciones[i].ladoIzq = noTerminal; // Asigna el lado izquierdo de la produccion
    if (i == 0){
      g->axioma = noTerminal; // El primer NO TERMINAL es el axioma
    }
    system("cls");

    printf("%c -> ...\n\n", g->producciones[i].ladoIzq);
    printf("NO TERMINALES: %s\n", g->VN);
    printf("TERMINALES: %s\n", g->VT);
    printf("Ingrese el lado derecho de la produccion %d (max. 10 caracteres): ", i + 1);
    scanf("%10s", ladoDer);

    // Verifica si el lado derecho de la produccion es valido
    while(!esProduccionValida(g, ladoDer) || strlen(ladoDer) > 10 || strlen(ladoDer) == 0){
      printf("Produccion no valida. Intente de nuevo.\n\n");
      printf("Ingrese el lado derecho de la produccion %d (max. 10 caracteres): ", i + 1);
      scanf("%10s", ladoDer);
    }
    strcpy(g->producciones[i].ladoDer, ladoDer);

    i++; // Incrementa el contador de producciones

    printf("Produccion %d cargada: %c -> %s\n", i, g->producciones[i - 1].ladoIzq, g->producciones[i - 1].ladoDer);
    printf("Desea ingresar otra produccion? (s/n): ");
    scanf(" %c", &continuar);
    if (continuar != 's' && continuar != 'S'){
      termino = 1; // Termina el ciclo si no se desea continuar
    }

    system("cls");
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