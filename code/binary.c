#include <stdint.h>
#include <stdio.h>

/* Format the contents stored at a pointer as a binary number
   in the ASCII representation ready for printing. The output
   butter `out` must have room for exactly `9 * nbytes`
   characters.
 */
void format_binary(void *p, char *out, int nbytes) {
  int iout = 0;
  char *data = p;
  for (int ibyte = 0; ibyte != nbytes; ibyte++) {
    for (int ibit = 7; ibit != -1; ibit--) {
      if (((data[ibyte] >> ibit) & 1) == 1) {
        out[iout++] = '1';
      } else {
        out[iout++] = '0';
      }
    }
    out[iout++] = ' ';
  }
  out[iout] = '\0';
}

void print_double(double x) {
  char buf[8 * 9];
  format_binary(&x, buf, 8);
  if (x >= 0) printf(" ");
  printf("%02.3f in binary: %s\n", x, buf);
}

int main() {
  printf("All 8-bit values:\n");
  for (uint8_t i = 0; ; ++i) {
    char buf[9];
    format_binary(&i, buf, 1);
    /* Print representations: unsigned, signed, hex, ASCII character, binary */
    char i_or_space;
    if (i >= 32 && i < 127) {
      i_or_space = i;
    } else {
      i_or_space = ' ';
    }
    printf("%4d    %4d    0x%02x   %c    %s\n", i, (int8_t)i, i, i_or_space, buf);

    /* cannot break the usual way due to overflow, so do it before the ++i */
    if (i == 255) break; 
  }
  printf("\n");
  
  printf("Some 16-bit values:\n");
  for (uint16_t i = 250; i != 265; ++i) {
    char buf[2 * 9];
    format_binary(&i, buf, 2);
    /* Print representations: unsigned, hex, binary */
    printf("%4d    0x%04x    %s\n", i, i, buf);
  }
  printf("\n");

  /* Print some floating point numbers. To understand this, see

  http://en.wikipedia.org/wiki/Double-precision_floating-point_format

  and keep in mind big vs. little-endian. */
  printf("Double-precision floating point (64 bits = 8 bytes):\n");
  print_double(1.0);
  print_double(-1);
  print_double(2.0);
  print_double(4.0);
  print_double(1.5);
  print_double(1.75);
  print_double(3.1415);
  printf("\n");

  /* Print a pointer */
  double x = 2.71828182846;
  printf("Finally, a pointer:\n");
  printf(" x contains %f and its address is 0x%016lx\n", x, (uintptr_t)&x);
  printf("\n");
  
}
