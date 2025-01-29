/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * Processes - part2.c
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
 */

#include "./libprocesslab/libprocesslab.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define m 1000000
#define nb 8

long sum[nb];

void *contribution(void *p)
{
  int no = *(int *)p;
  long start = (no * m / nb) + 1;
  long end = (no + 1) * m / nb;
  sum[no] = 0;
  for (long i = start; i <= end; i++)
  {
    sum[no] += i;
  }
  return NULL;
}

void question2()
{
  pthread_t threads[nb];
  int threadIds[nb];

  for (int i = 0; i < nb; i++)
  {
    threadIds[i] = i;
    pthread_create(&threads[i], NULL, contribution, &threadIds[i]);
  }
  for (int i = 0; i < nb; i++)
  {
    pthread_join(threads[i], NULL);
  }

  long totalSum = 0;
  for (int i = 0; i < nb; i++)
  {
    totalSum += sum[i];
  }

  long theoSum = (long)m * (m + 1) / 2;

  printf("sum[0] + sum[1] + ... + sum[nb-1] = %ld\n", totalSum);
  printf("m * (m + 1) / 2 = %ld\n", theoSum);


}
