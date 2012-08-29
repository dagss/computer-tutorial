#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

int read_move_from_terminal(int boardsize) {
  char column_char;
  int column, row;
  /* Continue reading until we have legal input */
  while (1) {
    printf("Your move: ");
    while (!scanf(" %c%d", &column_char, &row)) {
      fprintf(stderr, "ERROR: Could not parse move. 'a2' would be a parseable move. \n");
    }
    column = tolower(column_char) - 'a';
    row -= 1;
    if (!(0 <= row && row < boardsize) || !(0 <= column && column < boardsize)) {
      fprintf(stderr, "ERROR: Specify move inside the board (boardsize=%d)\n", boardsize);
    } else {
      break;
    }
  }
  /* translate col/row to row-major indexing and return */
  return row * boardsize + column;
}

void print_board(char *board, int boardsize) {
  /* Header */
  printf("   ");
  for (int column = 0; column != boardsize; ++column) {
    printf("%c  ", 'a' + column);
  }
  printf("\n  +");
  for (int column = 0; column != boardsize; ++column) {
    printf("---");
  }
  printf("\n");
  /* Main board */
  for (int row = 0; row != boardsize; ++row) {
    printf("%2d|", row + 1);
    for (int column = 0; column != boardsize; ++column) {
      printf("%c  ", board[row * boardsize + column]);
    }
    printf("\n");
  }
}

void initialize_board(char *board, int boardsize) {
  for (int i = 0; i != boardsize * boardsize; ++i) {
    board[i] = ' ';
  }
}

int main() {
  char board[9];
  initialize_board(board, 3);
  printf("You are 'x'\n");
  int move = read_move_from_terminal(3);
  board[move] = 'x';
  print_board(board, 3);
}
