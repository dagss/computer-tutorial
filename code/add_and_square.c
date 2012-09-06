#include <stdio.h>

void square_me(int *p) {
  int x = *p;
  *p = x * x;
}

int main() {
  int i, j, k;
  printf("Enter two numbers to add separated by space:\n");
  if (!scanf("%d %d", &i, &j)) return -1;
  k = i + j;
  square_me(&k);
  printf("(%d + %d)^2 = %d\n", i, j, k);
  return 0;
}
