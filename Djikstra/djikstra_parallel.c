#include <limits.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NV 1100

int main(int argc, char **argv);
int *dijkstra_distance(int ohd[NV][NV]);

void find_nearest(int s, int e, int mind[NV], int connected[NV], int *d,
                  int *v);
void init(int ohd[NV][NV]);
void update_mind(int s, int e, int mv, int connected[NV], int ohd[NV][NV],
                 int mind[NV]);

void writeTime(float time) {
  FILE *file;
  file = fopen("./data.txt", "w");
  fprintf(file, "# Parallel Serial\n%f ", time);
  fclose(file);
}

int main(int argc, char **argv)

{
  int i;
  int j;
  int *mind;
  int ohd[NV][NV];

  init(ohd);
  float start = omp_get_wtime();
  mind = dijkstra_distance(ohd);
  float end = omp_get_wtime();
  /*
    Print the results.
  */

  free(mind);
  printf("Time taken by parallel: %fs\n", end - start);
  writeTime(end - start);

  return 0;
}
/******************************************************************************/

int *dijkstra_distance(int ohd[NV][NV]) {
  int *connected;
  int i;
  int i4_huge = INT_MAX;
  int md;
  int *mind;
  int mv;
  int my_first;
  int my_id;
  int my_last;
  int my_md;
  int my_mv;
  int my_step;
  int nth;
  /*
    Start out with only node 0 connected to the tree.
  */
  connected = (int *)malloc(NV * sizeof(int));

  connected[0] = 1;
  for (i = 1; i < NV; i++) {
    connected[i] = 0;
  }
  /*
    Initial estimate of minimum distance is the 1-step distance.
  */
  mind = (int *)malloc(NV * sizeof(int));

  for (i = 0; i < NV; i++) {
    mind[i] = ohd[0][i];
  }
  /*
    Begin the parallel region.
  */
#pragma omp parallel num_threads(5) private(my_first, my_id, my_last, my_md,   \
                                            my_mv, my_step)                    \
    shared(connected, md, mind, mv, nth, ohd)
  {
    my_id = omp_get_thread_num();
    nth = omp_get_num_threads();
    // printf("%d\n", nth);
    my_first = (my_id * NV) / nth;
    my_last = ((my_id + 1) * NV) / nth - 1;
    /*
      The SINGLE directive means that the block is to be executed by only
      one thread, and that thread will be whichever one gets here first.
    */

    for (my_step = 1; my_step < NV; my_step++) {
      /*
        Before we compare the results of each thread, set the shared variable
        MD to a big value.  Only one thread needs to do this.
      */
#pragma omp single
      {
        md = i4_huge;
        mv = -1;
      }
      find_nearest(my_first, my_last, mind, connected, &my_md, &my_mv);
      /*
        In order to determine the minimum of all the MY_MD's, we must insist
        that only one thread at a time execute this block!
      */
#pragma omp critical
      {
        if (my_md < md) {
          md = my_md;
          mv = my_mv;
        }
      }
      /*
        This barrier means that ALL threads have executed the critical
        block, and therefore MD and MV have the correct value.  Only then
        can we proceed.
      */
#pragma omp barrier
      /*
        If MV is -1, then NO thread found an unconnected node, so we're done
        early. OpenMP does not like to BREAK out of a parallel region, so we'll
        just have to let the iteration run to the end, while we avoid doing any
        more updates.

        Otherwise, we connect the nearest node.
      */
#pragma omp single
      {
        if (mv != -1) {
          connected[mv] = 1;
        }
      }
      /*
        Again, we don't want any thread to proceed until the value of
        CONNECTED is updated.
      */
#pragma omp barrier
      /*
        Now each thread should update its portion of the MIND vector,
        by checking to see whether the trip from 0 to MV plus the step
        from MV to a node is closer than the current record.
      */
      if (mv != -1) {
        update_mind(my_first, my_last, mv, connected, ohd, mind);
      }
      /*
        Before starting the next step of the iteration, we need all threads
        to complete the updating, so we set a BARRIER here.
      */
#pragma omp barrier
    }
    /*
      Once all the nodes have been connected, we can exit.
    */
  }

  free(connected);

  return mind;
}
/******************************************************************************/

void find_nearest(int s, int e, int mind[NV], int connected[NV], int *d, int *v)

{
  int i;
  int i4_huge = INT_MAX;

  *d = i4_huge;
  *v = -1;

  for (i = s; i <= e; i++) {
    if (!connected[i] && (mind[i] < *d)) {
      *d = mind[i];
      *v = i;
    }
  }
  return;
}
/******************************************************************************/

void init(int ohd[NV][NV]) {
  int i;
  int i4_huge = INT_MAX;
  int j;

  for (i = 0; i < NV; i++) {
    for (j = 0; j < NV; j++) {
      if (i == j)
        ohd[i][i] = 0;
      else
        ohd[i][j] = rand() % 1101;
    }
  }

  for (i = 0; i < NV; i++) {
    for (j = 0; j < NV; j++) {
      ohd[j][i] = ohd[i][j];
    }
  }

  return;
}
/******************************************************************************/

void update_mind(int s, int e, int mv, int connected[NV], int ohd[NV][NV],
                 int mind[NV])

{
  int i;
  int i4_huge = INT_MAX;

  for (i = s; i <= e; i++) {
    if (!connected[i]) {
      if (ohd[mv][i] < i4_huge) {
        if (mind[mv] + ohd[mv][i] < mind[i]) {
          mind[i] = mind[mv] + ohd[mv][i];
        }
      }
    }
  }
  return;
}