#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estructura para representar el estado de la solución
typedef struct
{
  int objeto;  // Índice del objeto
  int mochila; // Índice de la mochila asignada
} Estado;

// Función para generar una solución inicial aleatoria
void generarSolucionInicial(int n, int l, int pesos[], Estado asignaciones[])
{
  for (int i = 0; i < n; i++)
  {
    asignaciones[i].objeto = i;
    asignaciones[i].mochila = rand() % l;
  }
}

// auxiliar
// Función para imprimir la asignación de objetos a mochilas
void imprimirAsignacion(int n, Estado asignaciones[])
{
  printf("Asignación de objetos a mochilas:\n");

  for (int i = 0; i < n; i++)
  {
    printf("Objeto %d -> Mochila %d\n", i, asignaciones[i].mochila);
  }
}

int *obtenerPesosCadaMochila(int l, Estado asignaciones[], int n, int pesos[])
{
  int *peso_mochilas = (int *)calloc(l, sizeof(int));
  for (int i = 0; i < n; i++)
  {
    peso_mochilas[asignaciones[i].mochila] += pesos[i];
  }
  return peso_mochilas;
}
void imprimirPesosCadaMochila(int l, Estado asignaciones[], int n, int pesos[])
{
  printf("peso total de cada mochila:\n");
  int *pesos_mochilas = obtenerPesosCadaMochila(l, asignaciones, n, pesos);
  for (int i = 0; i < l; i++)
  {
    printf("Mochila %d -> Peso %d\n", i, pesos_mochilas[i]);
  }
}

int obtenerPesoDeMochilaMasPesada(int l, Estado asignaciones[], int n, int pesos[])
{
  int *pesos_mochilas = obtenerPesosCadaMochila(l, asignaciones, n, pesos);
  int peso_maximo = 0;
  for (int i = 0; i < l; i++)
  {
    if (pesos_mochilas[i] > peso_maximo)
    {
      peso_maximo = pesos_mochilas[i];
    }
  }
  return peso_maximo;
}

void imprimirPesoTeorico(int n, int l, int pesos[])
{
  int peso_total = 0;
  for (int i = 0; i < n; i++)
  {
    peso_total += pesos[i];
  }
  int peso_teorico = peso_total / l;
  // printf("Peso Total de los %d objetos-> %d\n", n, peso_total);
  printf("Peso Teorico-> %d\n", peso_teorico);
}

// Función para encontrar una solución vecina mediante la heurística de intercambio de un objeto
Estado generarVecinoIntercambioObjeto(int indice_objeto, int l, int pesos[], Estado asignaciones[])
{
  Estado vecino;
  vecino.objeto = indice_objeto; // Seleccionar un objeto al azar
  vecino.mochila = rand() % l;   // Seleccionar una mochila al azar
  return vecino;
}

// calcula la mochila mas pesada de una asignacion dado un cierto vecino
// luego retorna las asignaciones a su estado original
int calcularPesoMaximoAsignacionTemporal(int l, Estado asignaciones[], int n, int pesos[], Estado vecino)
{
  Estado asignacion_antigua = asignaciones[vecino.objeto];
  asignaciones[vecino.objeto].mochila = vecino.mochila;
  int pesoMochilaMasPesada = obtenerPesoDeMochilaMasPesada(l, asignaciones, n, pesos);
  asignaciones[vecino.objeto].mochila = asignacion_antigua.mochila;
  return pesoMochilaMasPesada;
}

// Función para encontrar la solución vecina con menor peso de mochila
Estado encontrarMejorVecino(int n, int l, int pesos[], Estado asignaciones[], Estado (*generarVecino)(int, int, int[], Estado[]))
{
  Estado mejorVecino = asignaciones[0];
  int mejorPeso = obtenerPesoDeMochilaMasPesada(l, asignaciones, n, pesos);

  for (int i = 0; i < n; i++)
  {
    // asignacion "item - mochila" aleatoria
    Estado vecino = generarVecino(i, l, pesos, asignaciones);
    // calculo el peso de esa mochila
    int pesoVecino = calcularPesoMaximoAsignacionTemporal(l, asignaciones, n, pesos, vecino);
    if (pesoVecino < mejorPeso)
    {
      mejorVecino = vecino;
      mejorPeso = pesoVecino;
    }
  }
  return mejorVecino;
}

// Función para el algoritmo de Búsqueda Local
void busquedaLocal(int n, int l, int pesos[], Estado asignaciones[], Estado (*generarVecino)(int, int, int[], Estado[]))
{
  int iteraciones = 0;
  while (iteraciones < 10000)
  {
    Estado mejorVecino = encontrarMejorVecino(n, l, pesos, asignaciones, generarVecino);
    asignaciones[mejorVecino.objeto].mochila = mejorVecino.mochila;
    iteraciones++;
  }
}

char *solution(int n, int pesos[], int l)
{
  srand(time(NULL)); // Inicializar generador de números aleatorios

  // Generar una solución inicial aleatoria
  Estado asignaciones[n];
  generarSolucionInicial(n, l, pesos, asignaciones);

  // Imprimir la asignación inicial
  // imprimirAsignacion(n, asignaciones);
  imprimirPesosCadaMochila(l, asignaciones, n, pesos);
  printf("mochila mas pesada -> %d\n", obtenerPesoDeMochilaMasPesada(l, asignaciones, n, pesos));

  // Ejecutar el algoritmo de Búsqueda Local
  busquedaLocal(n, l, pesos, asignaciones, generarVecinoIntercambioObjeto);
  // busquedaLocal(n, l, pesos, asignaciones, generarVecinoIntercambioDosObjetos);

  // Imprimir los pesos de cada mochila
  imprimirPesosCadaMochila(l, asignaciones, n, pesos);
  imprimirPesoTeorico(n, l, pesos);
  imprimirAsignacion(n, asignaciones);
  printf("mochila mas pesada -> %d\n", obtenerPesoDeMochilaMasPesada(l, asignaciones, n, pesos));

  // return a string with the asignaciones variable
  char *asignaciones_string = (char *)malloc(n * sizeof(char));
  for (int i = 0; i < n; i++)
  {
    asignaciones_string[i] = asignaciones[i].mochila + '0';
  }
  return asignaciones_string;
}

int main()
{
  return 0;
}