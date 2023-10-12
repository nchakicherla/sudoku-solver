#ifndef TEST_SUDOKU_SOLVER_H
#define TEST_SUDOKU_SOLVER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "memory.h"

#define LOCALE_LEN 	3
#define BOARD_LEN	(LOCALE_LEN * LOCALE_LEN)
#define BOARD_AREA	(BOARD_LEN * BOARD_LEN)

struct Board;
struct Square;
struct I32Node;

struct Board*
newBoard(const int32_t* board_start);

int
freeBoard(struct Board* board);

void
initSquare(struct Square* square, int32_t value);

void
addPossToSquare(struct Square* square, int32_t poss);

void
removePossFromSquare(struct Square* square, int32_t poss);

struct I32Node*
newI32Node(int32_t value);

void
updateSquarePossibilities(struct Board* board, size_t square_idx);

void
validateAllPossibilities(struct Board* board);

bool
checkForWin(struct Board* board);

void
updateBoardSolved(struct Board* board);

void
TEST_printAllPossibilities(struct Board* board);

void
printSolution(struct Board* board);

#endif // TEST_SUDOKU_SOLVER_H