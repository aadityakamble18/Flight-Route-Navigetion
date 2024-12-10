#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_VERTICES 100
#define INF INT_MAX
#define MAX_STRING_LENGTH 20

int graph[MAX_VERTICES][MAX_VERTICES];
int predecessor[MAX_VERTICES][MAX_VERTICES];

typedef struct {
  char name[MAX_STRING_LENGTH];
  char city[MAX_STRING_LENGTH];
  char country[MAX_STRING_LENGTH];
  char IATA[MAX_STRING_LENGTH];
} Airport;

int readAirports(Airport airports[]) {
  FILE *file = fopen("airports_final.csv", "r");
  if (!file) {
    printf("Error opening airports.csv\n");
    exit(EXIT_FAILURE);
  }

  int numAirports = 0;
  char line[MAX_STRING_LENGTH * 4];
  fgets(line, sizeof(line), file);
  while (fgets(line, sizeof(line), file)) {
    sscanf(line, "%[^,],%[^,],%[^,],%s", airports[numAirports].name,
           airports[numAirports].city, airports[numAirports].country,
           airports[numAirports].IATA);
    numAirports++;
  }

  fclose(file);
  return numAirports;
}

void initializeGraph(int vertices) {
  for (int i = 0; i < vertices; i++) {
    for (int j = 0; j < vertices; j++) {
      if (i == j) {
        graph[i][j] = 0;
      } else {
        graph[i][j] = INF;
      }
    }
  }
}

void addEdge(int u, int v, int weight) { graph[u][v] = weight; }

void floydWarshall(int vertices) {
  for (int i = 0; i < vertices; i++) {
    for (int j = 0; j < vertices; j++) {
      if (graph[i][j] != INF && i != j) {
        predecessor[i][j] = i;
      } else {
        predecessor[i][j] = -1;
      }
    }
  }

  for (int k = 0; k < vertices; k++) {
    for (int i = 0; i < vertices; i++) {
      for (int j = 0; j < vertices; j++) {
        if (graph[i][k] != INF && graph[k][j] != INF &&
            graph[i][k] + graph[k][j] < graph[i][j]) {
          graph[i][j] = graph[i][k] + graph[k][j];
          predecessor[i][j] = predecessor[k][j];
        }
      }
    }
  }
}

int findAirportIndex(Airport airports[], char IATA[]) {
  for (int i = 0; i < MAX_VERTICES; i++) {
    if (strcmp(airports[i].IATA, IATA) == 0) {
      return i;
    }
  }
  return -1;
}

void printSolution(Airport airports[], int numAirports) {
  int maxDigits = 0;
  for (int i = 0; i < numAirports; i++) {
    for (int j = 0; j < numAirports; j++) {
      if (graph[i][j] != -1) {
        int numDigits = snprintf(NULL, 0, "%d", graph[i][j]);
        if (numDigits > maxDigits) {
          maxDigits = numDigits;
        }
      }
    }
  }
  maxDigits++;

  // Print the matrix
  printf("Shortest distances between every pair of airports:\n");
  for (int i = 0; i < numAirports; i++) {
    for (int j = 0; j < numAirports; j++) {
      if (graph[i][j] == INF) {
        printf("%*s", maxDigits, "-1");
      } else {
        printf("%*d", maxDigits, graph[i][j]);
      }
      if (j != numAirports - 1) {
        printf(" ");
      }
    }
    printf("\n");
  }
}

void readRoutesAndConstructGraph(Airport airports[], int numAirports) {
  FILE *file = fopen("routes_final.csv", "r");
  if (!file) {
    printf("Error opening routes.csv\n");
    exit(EXIT_FAILURE);
  }

  char line[MAX_STRING_LENGTH * 4] = "";
  int numEdges = 0;

  fgets(line, sizeof(line), file);

  while (fgets(line, sizeof(line), file)) {

    char sourceIATA[4] = "", destinationIATA[4] = "";
    int weight;

    int result =
        sscanf(line, "%[^,],%[^,],%d", sourceIATA, destinationIATA, &weight);
    if (result != 3) {
      printf("Error reading route data from routes.csv\n");
      exit(EXIT_FAILURE);
    }

    int u = findAirportIndex(airports, sourceIATA);
    int v = findAirportIndex(airports, destinationIATA);

    addEdge(u, v, weight);
    numEdges++;
  }

  fclose(file);
  printf("Number of airports: %d\n", numAirports);
  printf("Number of edges added: %d\n", numEdges);
}

void printPath(Airport airports[], int source, int destination) {
  if (source == destination) {
    printf("%s ", airports[source].IATA);
  } else if (predecessor[source][destination] == -1) {
    printf("No path exists between %s and %s\n", airports[source].IATA,
           airports[destination].IATA);
  } else {
    printPath(airports, source, predecessor[source][destination]);
    printf("-> ");
    printf("%s ", airports[destination].IATA);
  }
}

int main() {
  Airport airports[MAX_VERTICES];

  int numAirports = readAirports(airports);

  initializeGraph(numAirports);

  readRoutesAndConstructGraph(airports, numAirports);

  char sourceIATA[4] = "", destinationIATA[4] = "";
  printf("Enter source IATA: ");
  scanf("%s", sourceIATA);
  printf("Enter destination IATA: ");
  scanf("%s", destinationIATA);

  int sourceIndex = findAirportIndex(airports, sourceIATA);
  int destinationIndex = findAirportIndex(airports, destinationIATA);

  if (sourceIndex == -1 || destinationIndex == -1) {
    printf("Invalid source or destination IATA.\n");
    return 1;
  }

  clock_t start_time = clock(); // Start timer

  floydWarshall(numAirports);

  clock_t end_time = clock(); // end timer
  double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

  printf("Time taken by Floyd-Warshall algorithm: %f seconds\n", time_taken);

  printSolution(airports, numAirports);

  if (graph[sourceIndex][destinationIndex] == INF) {
    printf("No path found between %s and %s.\n", sourceIATA, destinationIATA);
  } else {
    printf("Shortest distance between %s and %s: %d\n", sourceIATA,
           destinationIATA, graph[sourceIndex][destinationIndex]);
    printf("Shortest path: ");
    printPath(airports, sourceIndex, destinationIndex);
    printf("\n");
  }

  return 0;
}
