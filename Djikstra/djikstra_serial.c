#include <limits.h>
#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define V 8

int minDistance(int dist[], bool sptSet[]) {
  // Initialize min value
  int min = INT_MAX, min_index;

  for (int v = 0; v < V; v++)
    if (sptSet[v] == false && dist[v] <= min)
      min = dist[v], min_index = v;

  return min_index;
}

void printSolution(int dist[]) {
  printf("\nFOR SERIAL CODE\n");

  for (int i = 0; i < V; i++)
    printf("Path to Vertex %d is %d\n", i, dist[i]);
}

void dijkstra(int graph[V][V], int src) {
  int dist[V];

  bool sptSet[V];

  for (int i = 0; i < V; i++)
    dist[i] = INT_MAX, sptSet[i] = false;

  dist[src] = 0;

  for (int count = 0; count < V - 1; count++) {
    int u = minDistance(dist, sptSet);

    sptSet[u] = true;

    for (int v = 0; v < V; v++)
      if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX &&
          dist[u] + graph[u][v] < dist[v])
        dist[v] = dist[u] + graph[u][v];
  }

  //   printSolution(dist);
}

int main() {
  double start, end;
  int graph[V][V] = {{0, 0, 30, 0, 10, 0, 90, 50}, {0, 0, 10, 20, 0, 0, 0, 0},
                     {30, 10, 0, 0, 0, 10, 0, 0},  {0, 20, 0, 0, 0, 10, 0, 10},
                     {10, 0, 0, 0, 0, 0, 20, 20},  {0, 0, 10, 10, 0, 0, 0, 0},
                     {90, 0, 0, 0, 20, 0, 0, 0},   {50, 0, 0, 10, 20, 0, 0, 0}};

  start = omp_get_wtime();
  dijkstra(graph, 0);
  end = omp_get_wtime();
  printf("Time taken by serial code: %f s\n", end - start);
  return 0;
}
