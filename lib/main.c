#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estructura para representar el estado de la solución
typedef struct
{
  int task;    // Índice del task
  int cluster; // Índice de la cluster asignada
} State;

// Función para generate una solución inicial aleatoria
void generateInitialSolution(int n, int l, int times[], State assignments[])
{
  for (int i = 0; i < n; i++)
  {
    assignments[i].task = i;
    assignments[i].cluster = rand() % l;
  }
}

// auxiliar
// Función para imprimir la asignación de tasks a clusters
void printAssignment(int n, State assignments[])
{
  printf("Asignación de tasks a clusters:\n");

  for (int i = 0; i < n; i++)
  {
    printf("Task %d -> Cluster %d\n", i, assignments[i].cluster);
  }
}

int *getTimesEachCluster(int l, State assignments[], int n, int times[])
{
  int *timesClusters = (int *)calloc(l, sizeof(int));
  for (int i = 0; i < n; i++)
  {
    timesClusters[assignments[i].cluster] += times[i];
  }
  return timesClusters;
}

void printTimesEachCluster(int l, State assignments[], int n, int times[])
{
  printf("Tiempo total de cada cluster:\n");
  int *timesClusters = getTimesEachCluster(l, assignments, n, times);
  for (int i = 0; i < l; i++)
  {
    printf("Cluster %d -> Tiempo %d\n", i, timesClusters[i]);
  }
}

int getMaxTimesFromCluster(int l, State assignments[], int n, int times[])
{
  int *timesClusters = getTimesEachCluster(l, assignments, n, times);
  int maxTimes = 0;
  for (int i = 0; i < l; i++)
  {
    if (timesClusters[i] > maxTimes)
    {
      maxTimes = timesClusters[i];
    }
  }
  return maxTimes;
}

void printTheoricTimes(int n, int l, int times[])
{
  int totalTime = 0;
  for (int i = 0; i < n; i++)
  {
    totalTime += times[i];
  }
  int theoricTime = totalTime / l;
  // printf("Tiempo Total de los %d tasks-> %d\n", n, totalTime);
  printf("Tiempo teórico-> %d\n", theoricTime);
}

// Función para encontrar una solución vecina mediante la heurística de intercambio de un task
State generateNeighbourExchangeTask(int indexTask, int l, int times[], State assignments[])
{
  State neighbour;
  neighbour.task = indexTask;     // Seleccionar un task al azar
  neighbour.cluster = rand() % l; // Seleccionar una cluster al azar
  return neighbour;
}

// calcula la cluster mas pesada de una asignacion dado un cierto neighbour
// luego retorna las assignments a su estado original
int calculateTimeMaxTemporalAssignment(int l, State assignments[], int n, int times[], State neighbour)
{
  State oldAssignation = assignments[neighbour.task];
  assignments[neighbour.task].cluster = neighbour.cluster;
  int timeMaxCluster = getMaxTimesFromCluster(l, assignments, n, times);
  assignments[neighbour.task].cluster = oldAssignation.cluster;
  return timeMaxCluster;
}

// Función para encontrar la solución vecina con menor time de cluster
State findBestNeighbour(int n, int l, int times[], State assignments[], State (*generateNeighbour)(int, int, int[], State[]))
{
  State bestNeighbour = assignments[0];
  int bestTime = getMaxTimesFromCluster(l, assignments, n, times);

  for (int i = 0; i < n; i++)
  {
    // asignacion "item - cluster" aleatoria
    State neighbour = generateNeighbour(i, l, times, assignments);
    // calculo el time de esa cluster
    int timeNeighbour = calculateTimeMaxTemporalAssignment(l, assignments, n, times, neighbour);
    if (timeNeighbour < bestTime)
    {
      bestNeighbour = neighbour;
      bestTime = timeNeighbour;
    }
  }
  return bestNeighbour;
}

// Función para el algoritmo de búsqueda Local
void localSearch(int n, int l, int times[], State assignments[], State (*generateNeighbour)(int, int, int[], State[]))
{
  int iterations = 0;
  while (iterations < 10000)
  {
    State bestNeighbour = findBestNeighbour(n, l, times, assignments, generateNeighbour);
    assignments[bestNeighbour.task].cluster = bestNeighbour.cluster;
    iterations++;
  }
}

char *solution(int n, int times[], int l)
{
  srand(time(NULL)); // Inicializar generador de números aleatorios

  // Generar una solución inicial aleatoria
  State assignments[n];
  generateInitialSolution(n, l, times, assignments);

  // Imprimir la asignación inicial
  // printAssignment(n, assignments);
  // printTimesEachCluster(l, assignments, n, times);
  // printf("Cluster con más tiempo -> %d\n", getMaxTimesFromCluster(l, assignments, n, times));

  // Ejecutar el algoritmo de Búsqueda Local
  localSearch(n, l, times, assignments, generateNeighbourExchangeTask);
  // localSearch(n, l, times, assignments, generateNeighbourExchangeDosTasks);

  // Imprimir los times de cada cluster
  printTimesEachCluster(l, assignments, n, times);
  printTheoricTimes(n, l, times);
  // printAssignment(n, assignments);
  // printf("Cluster con más tiempo -> %d\n", getMaxTimesFromCluster(l, assignments, n, times));

  // return a string with the assignments variable
  char *assignmentsString = (char *)malloc(n * sizeof(char));
  for (int i = 0; i < n; i++)
  {
    assignmentsString[i] = assignments[i].cluster + '0';
  }
  return assignmentsString;
}

int main()
{
  return 0;
}