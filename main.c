#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PRODUCCIONES 10

// Estructura de producción: representa una regla gramatical
// ladoDer puede tener hasta 2 símbolos: 1 terminal seguido opcionalmente por 1 no terminal.
// Se suma +1 para el caracter nulo ('\0').
typedef struct
{
  char ladoIzq;
  char ladoDer[2 + 1];
} produccion;

// Definicion de la estructura de gramatica
typedef struct
{
  char VN[5 + 1];
  char VT[5 + 1];
  char axioma;
  produccion producciones[MAX_PRODUCCIONES];
} gramatica;

// Prototipos de funciones
void inicializarGramatica(gramatica *g);
int estaEnLaLista(char *lista, char simbolo);
void cargarVariables(gramatica *g);
int esSimboloValido(char *N, char *T, char simbolo);
int esProduccionValida(gramatica *g, char *ladoDer);
void cargarProducciones(gramatica *g);
void imprimirCuatrupla(gramatica *g);
void limpiarConsola();
void generarProduccion(gramatica *g);
int contarProducciones(gramatica *g);
void produccionEncontrada (gramatica *g, char noTerminal, produccion *produc);

// TODO: Implementar función para generar palabras válidas a partir de la gramática ingresada.
// Esta función debe recorrer las producciones de forma controlada.
int main()
{
  gramatica g;
  inicializarGramatica(&g);
  cargarVariables(&g);
  limpiarConsola(); // Limpiar pantalla

  cargarProducciones(&g);
  limpiarConsola();

  printf("Gramatica cargada con exito.\n");
  imprimirCuatrupla(&g);
  printf("-----------------------------------------------------------------\n\n");
  generarProduccion(&g);
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { }
  getchar();
  return 0;
}

void inicializarGramatica(gramatica *g)
{
  for (int i = 0; i < MAX_PRODUCCIONES; i++)
  {
    g->producciones[i].ladoIzq = '\0';
    g->producciones[i].ladoDer[0] = '\0';
  }
}

// Verifica si un símbolo está presente en una lista de caracteres.
// Retorna 1 si está, 0 en caso contrario.
int estaEnLaLista(char *lista, char simbolo)
{
  for (int i = 0; i < strlen(lista); i++)
  {
    if (lista[i] == simbolo)
    {
      return 1;
    }
  }
  return 0;
}

// Funcion para cargar las variables de la gramatica
void cargarVariables(gramatica *g)
{
  printf("Ingrese hasta 5 NO TERMINALES de la gramatica (Ej: 'ABCDE'): ");
  scanf("%5s", g->VN);
  printf("\nIngrese hasta 5 TERMINALES de la gramatica (Ej: 'abcdf', el simbolo 'e' se reserva para no terminal vacio): ");
  scanf("%5s", g->VT);
}

// Funcion para verificar si un simbolo(no terminal o terminal) es valido para el lado izq. de la produccion
// Retorna 1 si es valido, 0 si no lo es
int esSimboloValido(char *N, char *T, char simbolo)
{
  return (estaEnLaLista(N, simbolo) || estaEnLaLista(T, simbolo) || simbolo == 'e');
}

// Funcion para verificar si una produccion es valida.
// Retorna 1 si es valida, 0 si no lo es
int esProduccionValida(gramatica *g, char *ladoDer)
{
  int len = strlen(ladoDer);

  // Caso: producción vacía
  // Si la producción está vacía, no es válida.
  if (len == 0)
  {
    printf("Produccion no valida: esta vacia. Si queria referirse a un no terminal vacio, puede usar 'e' (para representar 'ε').\n");
    return 0;
  }

  // Caso: producción con más de 2 símbolos
  // Si la producción tiene más de 2 símbolos, no es válida para una gramática regular.
  if (len > 2)
  {
    printf("Produccion no valida: no genera una gramatica regular. El lado derecho debe tener como maximo 2 simbolos.\n");
    return 0;
  }

  // Caso: producción con un solo símbolo
  // Si la producción tiene un solo símbolo, verifica si es un terminal o el símbolo vacío.
  if (len == 1)
  {
    char c = ladoDer[0];
    if (c == 'e')
      return 1; // Si es el símbolo vacío, es válido
    else if (!estaEnLaLista(g->VT, c))
    {
      printf("Produccion no valida: simbolo '%c' no es un terminal valido.\n", c);
      return 0;
    }
    return 1;
  }

  // Caso: producción con dos símbolos
  // - Si el primero es un terminal, el segundo debe ser un no terminal.
  // - Si el primero es un no terminal, el segundo debe ser un terminal.
  // Retorna 1 si la producción cumple con la forma de una gramática regular.
  // No se permite el símbolo vacío en producciones con dos símbolos
  if (ladoDer[0] == 'e' || ladoDer[1] == 'e' || strcmp(ladoDer, "e") == 0)
  {
    printf("Produccion no valida: no se permite el simbolo vacio en producciones con dos simbolos.\n");
    return 0;
  }
  else if (estaEnLaLista(g->VT, ladoDer[0]) && estaEnLaLista(g->VN, ladoDer[1]))
    return 1;
  else if (estaEnLaLista(g->VN, ladoDer[0]) && estaEnLaLista(g->VT, ladoDer[1]))
    return 1;

  printf("Produccion '%c%c' no valida: Debe ser T + NT o NT + T.\n", ladoDer[0], ladoDer[1]);
  return 0;
}

void cargarProducciones(gramatica *g)
{
  int i = 0;
  char continuar = 's';
  char noTerminal;
  char inputProduccion[100 + 1]; // para toda la línea con '|'

  while (i < MAX_PRODUCCIONES && (continuar == 's' || continuar == 'S'))
  {
    printf("NO TERMINALES: %s\n", g->VN);
    printf("Ingrese el NO TERMINAL de la produccion %d: ", i + 1);
    scanf(" %c", &noTerminal);

    // Verifica si el NO TERMINAL ingresado es valido
    while (!estaEnLaLista(g->VN, noTerminal))
    {
      printf("El NO TERMINAL no es valido\n\n");
      printf("Ingrese el NO TERMINAL de la produccion %d: ", i + 1);
      scanf(" %c", &noTerminal);
    }

    if (i == 0)
      g->axioma = noTerminal; // El primer NO TERMINAL es el axioma

    limpiarConsola();

    printf("%c -> ...\n\n", noTerminal);
    printf("NO TERMINALES: %s\n", g->VN);
    printf("TERMINALES: %s\n", g->VT);
    printf("Ingrese el lado derecho de la produccion %d (puede contener '|', max. 10 caracteres en total): ", i + 1);
    scanf("%100s", inputProduccion);

    char *token = strtok(inputProduccion, "|");
    while (token != NULL && i < MAX_PRODUCCIONES)
    {
      if (strlen(token) > 2)
      {
        printf("Produccion '%s' demasiado larga (max. 2 caracteres para gramatica regular).\n", token);
      }
      else if (!esProduccionValida(g, token))
      {
        printf("Produccion '%s' no valida. Se descartara.\n", token);
      }
      else
      {
        g->producciones[i].ladoIzq = noTerminal;
        strcpy(g->producciones[i].ladoDer, token);
        printf("Produccion %d cargada: %c -> %s\n", i + 1, noTerminal, token);
        i++;
      }

      token = strtok(NULL, "|");
    }

    if (i >= MAX_PRODUCCIONES)
    {
      printf("\nSe alcanzo el maximo de producciones permitidas (%d).\n", MAX_PRODUCCIONES);
      break;
    }

    printf("Desea ingresar otra produccion? (s/n): ");
    scanf(" %c", &continuar);

    limpiarConsola();
  }
}

void imprimirCuatrupla(gramatica *g)
{
  printf("G = (");

  // Imprime los NO TERMINALES
  printf("{");
  for (int i = 0; g->VN[i] != '\0'; i++)
  {
    printf("%c", g->VN[i]);
    if (g->VN[i + 1] != '\0')
      printf(", ");
  }
  printf("}, ");

  // Imprime los TERMINALES
  printf("{");
  for (int i = 0; g->VT[i] != '\0'; i++)
  {
    printf("%c", g->VT[i]);
    if (g->VT[i + 1] != '\0')
      printf(", ");
  }
  printf("}, ");

  // Imprime las producciones
  printf("{");
  for (int i = 0; i < 10 && g->producciones[i].ladoIzq != '\0'; i++)
  {
    printf("%c -> %s", g->producciones[i].ladoIzq, g->producciones[i].ladoDer);
    if (g->producciones[i + 1].ladoIzq != '\0')
      printf(", ");
  }
  printf("}, ");

  // Imprime el axioma
  printf("%c", g->axioma);
  printf(")\n");
}

// Limpia la consola, adaptado a distintos sistemas operativos (Windows/Linux).
void limpiarConsola()
{
#ifdef _WIN32
  system("cls"); // En Windows se usa "cls"
#else
  system("clear"); // En Linux y MacOS se usa "clear"
#endif
}

// Funcion para generar aleatoriamente palabras del lenguaje
void generarProduccion(gramatica *g)
{
  // inicializar semilla
  srand(time(NULL));
  int cantidadProducciones = contarProducciones(g);
  int random = rand() % cantidadProducciones;
  char axioma = g->axioma;
  printf("Numero Random: %d\n", random); // BORRAR

  // variable palabra
  char palabra[20] = "";
  produccion produccion ;

  // buscar la produccion del axioma (random)
  // Agarramos uno random y verificas que el lado izq sea el axioma
  //produccionEncontrada ( g,  axioma, &produccion);
  do
  {
    produccion = g->producciones[random];
    random = rand() % cantidadProducciones;
  } while (produccion.ladoIzq != axioma);
  
  while (strlen(produccion.ladoDer) != 1)
  {
    char letra[2] = {produccion.ladoDer[0], '\0'};
    printf("Palabra Antes: %s\n", palabra);
    strcat(palabra, letra);
    printf("Elemento Agregado: %c\n", produccion.ladoDer[0]);
    printf("Palabra Despues: %s\n", palabra);
    
    char noTerminal = produccion.ladoDer[1];
    //produccionEncontrada (g, noTerminal, &produccion);
    do
    {
      produccion = g->producciones[random];
      random = rand() % cantidadProducciones;
    } while (produccion.ladoIzq != noTerminal);
    
  }
  char letra[2] = {produccion.ladoDer[0], '\0'};
  strcat(palabra, letra);

  printf("Palabra terminada: %s\n", palabra);
}

void produccionEncontrada (gramatica *g, char noTerminal, produccion *produc){
  int cantidadProducciones = contarProducciones(g);
  int random = rand() % cantidadProducciones;
  do
  {
    *produc = g->producciones[random];
    random = rand() % cantidadProducciones;  
  } while (produc->ladoIzq != noTerminal);
}



int contarProducciones(gramatica *g)
{
  int contador = 0;
  for (int i = 0; i < MAX_PRODUCCIONES; i++)
  {
    if (g->producciones[i].ladoIzq != '\0')
    {
      contador++;
    }
  }

  return contador;
}

/*
S -> aT
S -> b
T -> b

S -> [aT, b]

[S -> aT, S -> b, T -> b]

S -> aT
T -> bX
X -> c


*/