#include <limits.h>
#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define V 1100

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

void writeTime(float time) {
  FILE *file;
  file = fopen("./data.txt", "a");
  fprintf(file, "%f ", time);
  fclose(file);
}

int main() {
  srand(time(NULL));
  double start, end;
  int graph[V][V];

  for (int i = 0; i < V; i++) {
    for (int j = 0; j < V; j++) {
      if (i != j)
        graph[i][j] = rand() % 10;
      else
        graph[i][j] = 0;
    }
  }

  for (int i = 0; i < V; i++) {
    for (int j = 0; j < V; j++) {
      graph[j][i] = graph[i][j];
    }
  }

  start = omp_get_wtime();
  dijkstra(graph, 0);
  end = omp_get_wtime();
  printf("Time taken by serial code: %f s\n", end - start);
  writeTime(end - start);

  system("gnuplot -p plot.gp");

  printf("Press ENTER to return to main menu...");
  getchar();
  execl("./main", "./main", NULL);
  return 0;
}
