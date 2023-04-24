// Estructura para representar el estado de la solución
class State {
  constructor(task, cluster) {
    this.task = task; // Índice del task
    this.cluster = cluster; // Índice de la cluster asignada
  }
}

// Función para generate una solución inicial aleatoria
function generateInitialSolution(n, l, times) {
  const assignments = [];
  for (let i = 0; i < n; i++) {
    assignments.push(new State(i, Math.floor(Math.random() * l)));
  }
  return assignments;
}

// auxiliar
// Función para imprimir la asignación de tasks a clusters
function printAssignment(assignments) {
  console.log('Asignación de tasks a clusters:');
  assignments.forEach((assignment) => {
    console.log(`Task ${assignment.task} -> Cluster ${assignment.cluster}`);
  });
}

function getTimesEachCluster(l, assignments, times) {
  const timesClusters = new Array(l).fill(0);
  assignments.forEach((assignment, i) => {
    timesClusters[assignment.cluster] += times[i];
  });
  return timesClusters;
}

function printTimesEachCluster(assignments, times, l) {
  console.log('Tiempo total de cada cluster:');
  const timesClusters = getTimesEachCluster(l, assignments, times);
  timesClusters.forEach((time, index) => {
    console.log(`Cluster ${index} -> Tiempo ${time}`);
  });
}

function getMaxTimesFromCluster(assignments, times, l) {
  const timesClusters = getTimesEachCluster(l, assignments, times);
  return Math.max(...timesClusters);
}

function printTheoricTimes(n, l, times) {
  const totalTime = times.reduce((total, time) => total + time, 0);
  const theoricTime = Math.floor(totalTime / l);
  console.log(`Tiempo teórico-> ${theoricTime}`);
}

// Función para encontrar una solución vecina mediante la heurística de intercambio de un task
function generateNeighbourExchangeTask(indexTask, l, times, assignments) {
  const neighbour = new State(indexTask, Math.floor(Math.random() * l));
  return neighbour;
}

// calcula la cluster mas pesada de una asignacion dado un cierto neighbour
// luego retorna las assignments a su estado original
function calculateTimeMaxTemporalAssignment(l, assignments, times, neighbour) {
  const oldAssignation = assignments[neighbour.task];
  const task = oldAssignation.task;
  const oldCluster = oldAssignation.cluster;
  assignments[neighbour.task].cluster = neighbour.cluster;
  const timeMaxCluster = getMaxTimesFromCluster(assignments, times, l);
  assignments[task].cluster = oldCluster;
  return timeMaxCluster;
}

// Función para encontrar la solución vecina con menor time de cluster
function findBestNeighbour(n, l, times, assignments, generateNeighbour) {
  let bestNeighbour = assignments[0];
  let bestTime = getMaxTimesFromCluster(assignments, times, l);

  let neighbour;
  let timeNeighbour;

  for (let i = 0; i < n; i++) {
    // asignacion "item - cluster" aleatoria
    neighbour = generateNeighbourExchangeTask(i, l, times, assignments);
    // calculo el time de esa cluster
    timeNeighbour = calculateTimeMaxTemporalAssignment(
      l,
      assignments,
      times,
      neighbour
    );
    if (timeNeighbour < bestTime) {
      bestNeighbour = neighbour;
      bestTime = timeNeighbour;
    }
  }
  return bestNeighbour;
}

// Función para el algoritmo de búsqueda Local
function localSearch(n, l, times, assignments, generateNeighbour) {
  let iterations = 0;
  let bestNeighbour;
  while (iterations < 10000) {
    bestNeighbour = findBestNeighbour(
      n,
      l,
      times,
      assignments,
      generateNeighbour
    );
    assignments[bestNeighbour.task].cluster = bestNeighbour.cluster;
    iterations++;
  }
}

function solution(n, times, l) {
  // Math.seedrandom(new Date().getTime()); // Inicializar generador de números aleatorios

  // Generar una solución inicial aleatoria
  const assignments = generateInitialSolution(n, l, times);

  // Imprimir la asignación inicial
  // printAssignment(n, assignments);
  // printTimesEachCluster(assignments, times, l);
  // console.log(
  //   'Cluster con más time -> ' + getMaxTimesFromCluster(assignments, times, l)
  // );

  // Ejecutar el algoritmo de Búsqueda Local
  localSearch(n, l, times, assignments, generateNeighbourExchangeTask);
  // localSearch(n, l, times, assignments, generateNeighbourExchangeDosTasks);

  // Imprimir los times de cada cluster
  printTimesEachCluster(assignments, times, l);
  printTheoricTimes(n, l, times);
  // printAssignment(assignments);
  // console.log(
  //   'Cluster con más time -> ' + getMaxTimesFromCluster(assignments, times, l)
  // );

  // return a string with the assignments variable
  let assignmentsString = '';
  for (let i = 0; i < n; i++) {
    assignmentsString += assignments[i].cluster;
  }
  return assignmentsString;
}
