#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define MAX_CITY_LENGTH 50
#define MAX_COUNTRY_LENGTH 50
#define MAX_IATA_LENGTH 4

#define MAX_VERTICES 70

struct AdjListNode {
  int dest;
  int weight;
  struct AdjListNode *next;
};

struct AdjList {
  struct AdjListNode *head;
};

struct Graph {
  int V;
  struct AdjList *array;
  char **IATA_codes;
};

struct AdjListNode *newAdjListNode(int dest, int weight) {
  struct AdjListNode *newNode =
      (struct AdjListNode *)malloc(sizeof(struct AdjListNode));
  newNode->dest = dest;
  newNode->weight = weight;
  newNode->next = NULL;
  return newNode;
}

struct Graph *createGraph(int V) {
  struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
  graph->V = V;
  graph->array = (struct AdjList *)malloc(V * sizeof(struct AdjList));
  graph->IATA_codes = (char **)malloc(V * sizeof(char *));
  for (int i = 0; i < V; ++i) {
    graph->array[i].head = NULL;
    graph->IATA_codes[i] = NULL;
  }
  return graph;
}

void addEdge(struct Graph *graph, int src, int dest, int weight) {
  struct AdjListNode *newNode = newAdjListNode(dest, weight);
  newNode->next = graph->array[src].head;
  graph->array[src].head = newNode;
}

// Function to read airport data from CSV file
void readAirports(const char *filename, struct Graph *graph) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  char line[MAX_NAME_LENGTH + MAX_CITY_LENGTH + MAX_COUNTRY_LENGTH +
            MAX_IATA_LENGTH + 4];
  int index = 0;

  fgets(line, sizeof(line), file);

  while (fgets(line, sizeof(line), file) != NULL && index < graph->V) {
    char name[MAX_NAME_LENGTH], city[MAX_CITY_LENGTH],
        country[MAX_COUNTRY_LENGTH], iata[MAX_IATA_LENGTH];
    sscanf(line, "%[^,],%[^,],%[^,],%s", name, city, country, iata);

    graph->IATA_codes[index] = strdup(iata);
    index++;
  }

  fclose(file);
}

// Function to read route data from CSV file and add edges to the graph
void readRoutes(const char *filename, struct Graph *graph) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  char line[MAX_IATA_LENGTH * 2 + 10];

  fgets(line, sizeof(line), file);

  while (fgets(line, sizeof(line), file) != NULL) {
    char sourceIATA[MAX_IATA_LENGTH], destIATA[MAX_IATA_LENGTH];
    int weight;
    sscanf(line, "%[^,],%[^,],%d", sourceIATA, destIATA, &weight);

    int src = -1, dest = -1;
    for (int i = 0; i < graph->V; ++i) {
      if (graph->IATA_codes[i] != NULL) {
        if (strcmp(graph->IATA_codes[i], sourceIATA) == 0) {
          src = i;
        } else if (strcmp(graph->IATA_codes[i], destIATA) == 0) {
          dest = i;
        }
      }
      if (src != -1 && dest != -1) {
        break;
      }
    }

    if (src != -1 && dest != -1) {
      addEdge(graph, src, dest, weight);
    }
  }

  fclose(file);
}

int minDistance(int dist[], int visited[], int V) {
  int min = INT_MAX, min_index;

  for (int v = 0; v < V; v++)
    if (visited[v] == 0 && dist[v] <= min)
      min = dist[v], min_index = v;

  return min_index;
}

void printPath(int parent[], int j, struct Graph *graph) {
  if (parent[j] == -1)
    return;

  printPath(parent, parent[j], graph);

  printf(" -> %s", graph->IATA_codes[j]);
}

void printSolution(int dist[], int parent[], int src, int dest,
                   struct Graph *graph) {
  printf("Vertex   Distance from Source   Path\n");
  if (dist[dest] == INT_MAX) {
    printf("No path from %s to %s\n", graph->IATA_codes[src],
           graph->IATA_codes[dest]);
    return;
  }
  printf("%s \t\t %d \t\t\t %s", graph->IATA_codes[dest], dist[dest],
         graph->IATA_codes[src]);
  printPath(parent, dest, graph);
  printf("\n");
}

void dijkstra(struct Graph *graph, int src, int dest) {
  int V = graph->V;
  int dist[V];
  int visited[V];
  int parent[V];
  for (int i = 0; i < V; i++) {
    dist[i] = INT_MAX;
    visited[i] = 0;
    parent[i] = -1;
  }

  dist[src] = 0;

  for (int count = 0; count < V - 1; count++) {
    int u = minDistance(dist, visited, V);
    visited[u] = 1;
    struct AdjListNode *pCrawl = graph->array[u].head;
    while (pCrawl != NULL) {
      int v = pCrawl->dest;
      if (!visited[v] && pCrawl->weight != -1 && dist[u] != INT_MAX &&
          dist[u] + pCrawl->weight < dist[v]) {
        dist[v] = dist[u] + pCrawl->weight;
        parent[v] = u;
      }
      pCrawl = pCrawl->next;
    }
  }

  printSolution(dist, parent, src, dest, graph);
}

int findVertexIndex(struct Graph *graph, const char *iata) {
  for (int i = 0; i < graph->V; ++i) {
    if (graph->IATA_codes[i] != NULL &&
        strcmp(graph->IATA_codes[i], iata) == 0) {
      return i;
    }
  }
  return -1;
}

int main() {
  struct Graph *graph = createGraph(MAX_VERTICES);

  readAirports("airports_final.csv", graph);
  readRoutes("routes_final.csv", graph);

  char sourceIATA[MAX_IATA_LENGTH];
  char destIATA[MAX_IATA_LENGTH];

  printf("Enter the source airport IATA code: ");
  scanf("%s", sourceIATA);
  printf("Enter the destination airport IATA code: ");
  scanf("%s", destIATA);

  int src = findVertexIndex(graph, sourceIATA);
  int dest = findVertexIndex(graph, destIATA);

  if (src != -1 && dest != -1) {
    clock_t start_time = clock();
    dijkstra(graph, src, dest);
    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time taken by Dijkstra Algorithem: %f seconds\n", time_taken);
  } else {
    printf("Source or destination airport not found.\n");
  }

  for (int i = 0; i < graph->V; ++i) {
    free(graph->IATA_codes[i]);
  }
  free(graph->IATA_codes);
  free(graph->array);
  free(graph);

  return 0;
}
