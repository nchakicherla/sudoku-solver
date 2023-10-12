#include "main.h"

const int32_t TEST_BOARD[81] = {  0 , 7 , 0 , 0 , 2 , 0 , 0 , 4 , 6 , 
                            0 , 6 , 0 , 0 , 0 , 0 , 8 , 9 , 0 , 
                            2 , 0 , 0 , 8 , 0 , 0 , 7 , 1 , 5 , 
                            0 , 8 , 4 , 0 , 9 , 7 , 0 , 0 , 0 , 
                            7 , 1 , 0 , 0 , 0 , 0 , 0 , 5 , 9 , 
                            0 , 0 , 0 , 1 , 3 , 0 , 4 , 8 , 0 ,
                            6 , 9 , 7 , 0 , 0 , 2 , 0 , 0 , 8 , 
                            0 , 5 , 8 , 0 , 0 , 0 , 0 , 6 , 0 ,
                            4 , 3 , 0 , 0 , 8 , 0 , 0 , 7 , 0  };

void
printTEST_BOARD(void) {
    printf("\nSTARTING BOARD\n\n");
    for(size_t i = 0; i < BOARD_LEN; i++) {
        for(size_t j = 0; j < BOARD_LEN; j++) {
            if(TEST_BOARD[(i * BOARD_LEN) + j]) {
                printf("%d ", TEST_BOARD[(i * BOARD_LEN) + j]);
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int
main(void) {

    printTEST_BOARD();

    struct Board* sudoku_board = newBoard(TEST_BOARD);

    while(!checkForWin(sudoku_board)) {
        for(size_t i = 0; i < BOARD_AREA; i++) {
            updateSquarePossibilities(sudoku_board, i);
        }
        validateAllPossibilities(sudoku_board);
    }

    printSolution(sudoku_board);

    freeBoard(sudoku_board);

    return EXIT_SUCCESS;
}
