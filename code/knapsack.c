/* Solve the Knapsack problem by brute force.

See http://en.wikipedia.org/wiki/Knapsack_problem

Input file format:

<capacity>
<number of items>
<name> <price> <weight>
<name> <price> <weight>
...

*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 10

typedef struct {
  double weight;
  double value;
  char name[MAXNAME];
} item_t;

void knapsack_search(int n, item_t *items, int *has_taken, int *solution, double capacity,
                     double current_weight, double current_value, double *highest_value) {
  /* Is this the best solution so far? If so, record it. */
  if (current_value > *highest_value) {
    for (int i = 0; i != n; ++i) {
      solution[i] = has_taken[i];
    }
    *highest_value = current_value;
  }

  /* See if we can carry more! */
  for (int index_to_try = 0; index_to_try != n; ++index_to_try) {
    if (has_taken[index_to_try]) {
      continue;
    }

    double new_weight = items[index_to_try].weight + current_weight;
    double new_value = items[index_to_try].value + current_value;

    /* If we can't carry it, we try the next one */
    if (new_weight > capacity) {
      continue;
    }

    /* Recurse! */
    has_taken[index_to_try] = 1;
    knapsack_search(n, items, has_taken, solution, capacity, new_weight, new_value, highest_value);
    has_taken[index_to_try] = 0;
  }
}

double solve_knapsack(int n, item_t *items, int *solution, double capacity) {
  int *has_taken = malloc(sizeof(int) * n);
  double highest_value = -1;
  memset(has_taken, 0, sizeof(int) * n);
  knapsack_search(n, items, has_taken, solution, capacity, 0, 0, &highest_value);
  free(has_taken);
  return highest_value;
}

int syntax_error(void) {
  fprintf(stderr, "Cannot parse problem description\n");
  return 3;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s problem_file\n", argv[0]);
    return 1;
  }
  FILE *f = fopen(argv[1], "r");
  if (!f) {
    fprintf(stderr, "File %s not found\n", argv[1]);
    return 2;
  }

  /* Read "header" information */
  double capacity;
  int n;
  if (!fscanf(f, "%lf\n", &capacity)) return syntax_error();
  if (!fscanf(f, "%d\n", &n)) return syntax_error();

  /* Allocate memory for the items from the heap */
  item_t *items = malloc(sizeof(item_t) * n);

  /* Parse the items and load them into the `items` array/memory area */
  for (int i = 0; i != n; ++i) {
        if (!fscanf(f, "%10s %lf %lf\n", items[i].name, &items[i].value, &items[i].weight)) {
      free(items);
      return syntax_error();
    }
  }
  fclose(f);

  /* Solve the problem. This also requires allocating an output array\
     and some scratch memory. */
  int *solution = malloc(sizeof(int) * n);
  double highest_value = solve_knapsack(n, items, solution, capacity);
  printf("You can carry goods with a value of %.2f NOK, namely:\n", highest_value);
  for (int i = 0; i != n; ++i) {
    if (solution[i]) {
      printf("  %s, %.2f NOK, %.2f kg\n", items[i].name, items[i].value, items[i].weight);
    }
  }
  free(solution);
  free(items);
}

