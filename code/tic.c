#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

/* Conventions:

 n - board size
 */

/*
  Boring utilities for user input/output and board utilities
*/
int read_move_from_terminal(int n) {
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
    if (!(0 <= row && row < n) || !(0 <= column && column < n)) {
      fprintf(stderr, "ERROR: Specify move inside the board (n=%d)\n", n);
    } else {
      break;
    }
  }
  /* translate col/row to row-major indexing and return */
  return row * n + column;
}

void print_board(char *board, int n) {
  /* Header */
  printf("   ");
  for (int column = 0; column != n; ++column) {
    printf("%c  ", 'a' + column);
  }
  printf("\n  +");
  for (int column = 0; column != n; ++column) {
    printf("---");
  }
  printf("\n");
  /* Main board */
  for (int row = 0; row != n; ++row) {
    printf("%2d|", row + 1);
    for (int column = 0; column != n; ++column) {
      printf("%c  ", board[row * n + column]);
    }
    printf("\n");
  }
}

void initialize_board(char *board, int n) {
  for (int i = 0; i != n * n; ++i) {
    board[i] = ' ';
  }
}

/* Returns BOARD_PLAYING, BOARD_HUMAN_WIN, BOARD_AI_WIN
 */
#define BOARD_PLAYING 0
#define BOARD_HUMAN_WIN 1
#define BOARD_AI_WIN 2

int check_stride_win(char *board, int stride, int n) {
  char first = board[0];
  if (first == ' ') {
    return BOARD_PLAYING;
  }
  for (int i = 1; i != n; i += stride) {
    if (board[i] != first) return 0;
  }
  if (first == 'x') {
    return BOARD_HUMAN_WIN;
  } else {
    return BOARD_AI_WIN;
  }
}

int is_winning_board(char *board, int n) {
  int ret;
  for (int i = 0; i != n; ++i) {
    /* Row i */
    ret = check_stride_win(board + i * n, 1, n);
    /* Column i */
    ret = check_stride_win(board + i, n, n);
  }
  /* Diagonals */
  ret = check_stride_win(board, n + 1, n);
  if (ret != 0) return ret;
  ret = check_stride_win(board, n - 1, n);
  if (ret != 0) return ret;  
}

/*
   The game search

   Return value: Best move for the computer.
    
 */
typedef struct {
  int slot;
  int ai_can_win;
  int ai_can_loose;
} move_t;

move_t simulate_move(char *board, int n, int computer_has_turn) {
  move_t result = {0, 0, 0};
  int board_status = is_winning_board(board, n);
  if (board_status == BOARD_HUMAN_WIN) {
    result.ai_can_win = 0;
    result.ai_can_loose = 1;
    return result;
  } else if (board_status == BOARD_AI_WIN) {
    result.ai_can_win = 1;
    result.ai_can_loose = 0;
    return result;
  }

  move_t winning_move;
  move_t second_move;

  for (int slot = 0; slot != n * n; ++slot) {
    if (board[slot] != ' ') {
      /* This slot is already taken, jump to next slot */
      continue;
    }
    /* Pretend to take this slot (temporarily) */
    if (computer_has_turn) {
      board[slot] = 'o';
    } else {
      board[slot] = 'x';
    }

    /* Recurse! */
    move_t next_move = simulate_move(board, n, !computer_has_turn);
    if (computer_has_turn) {
      /* We are happy once we find a move where we are guaranteed a win */
      if (next_move.ai_can_win && !next_move.ai_can_loose) {
        result.slot = slot;
        result.ai_can_win = 1;
        result.ai_can_loose = 0;
        /* Remember to reset board */
        board[slot] = ' ';
        return result;
      }
    } else {
    }
    if (next_move.ai_can_win
      
      /* We have tried this slot, reset board to the original position */
      board[slot] = ' ';
    }
  }

}



/*
   Main program
*/
int main() {
  char board[9];
  initialize_board(board, 3);
  printf("You are 'x'\n");
  int move = read_move_from_terminal(3);
  board[move] = 'x';
  print_board(board, 3);
}
