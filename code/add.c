#include <stdio.h>

int main() {
  int i, j, k;
  printf("Enter two numbers to add separated by space:\n");
  scanf("%d %d", &i, &j);
  k = i + j;
  printf("%d + %d = %d\n", i, j, k);
}
