#include "sudoku_solver.h"

struct Board {
	struct Square* squares;
	size_t num_cycles;
	size_t** squares_in_locale;
};

struct Square {
	struct I32Node* possibilities;
	int32_t soln;
};

struct I32Node {
	struct I32Node* next;
	int32_t value;
};

struct Board*
newBoard(const int32_t* board_start) {
	struct Board* new_board = wcalloc(1, sizeof(struct Board));
	new_board->squares = wcalloc(BOARD_AREA, sizeof(struct Square));
	for(size_t i = 0; i < BOARD_AREA; i++) {
		initSquare(&(new_board->squares[i]), board_start[i]);
	}
	new_board->squares_in_locale = wcalloc(BOARD_LEN, sizeof(size_t *));
	for(size_t i = 0; i < BOARD_LEN; i++) {
		new_board->squares_in_locale[i] = wcalloc(BOARD_LEN, sizeof(size_t));
	}
	size_t square_idx = 0;
	for(size_t i = 0; i < BOARD_LEN; i++) {
		for(size_t j = 0; j < BOARD_LEN; j++) {
			new_board->squares_in_locale[i][j] = square_idx;
			square_idx++;
			if((j + 1) % LOCALE_LEN == 0) {
				square_idx += BOARD_LEN - LOCALE_LEN;
			}
		}
		if((square_idx - (BOARD_LEN - LOCALE_LEN)) % (BOARD_LEN * LOCALE_LEN) == 0) {
			square_idx -= (BOARD_LEN - LOCALE_LEN);
		} else {
			square_idx -= (BOARD_LEN * LOCALE_LEN) - LOCALE_LEN;
		}
	}
	new_board->num_cycles = 0;
	return new_board;
}

int
freeBoard(struct Board* board) {
	for(size_t i = 0; i < BOARD_AREA; i++) {
		if(board->squares[i].possibilities) {
			struct I32Node* curr_node = board->squares[i].possibilities;
			struct I32Node* next_node = curr_node;
			while(curr_node) {
				next_node = curr_node->next;
				free(curr_node);
				curr_node = next_node;
			}
		}
	}
	free(board->squares);
	for(size_t i = 0; i < BOARD_LEN; i++) {
		free(board->squares_in_locale[i]);
	}
	free(board->squares_in_locale);
	free(board);
	return 0;
}

void
initSquare(struct Square* square, int32_t value) {
	square->possibilities = NULL;
	square->soln = value;
	return;
}

void
addPossToSquare(struct Square* square, int32_t poss) {
	struct I32Node* new_node = NULL;
	struct I32Node* curr_node = square->possibilities;
	while(curr_node) {
		if(curr_node->value == poss) {
			return;
		}
		curr_node = curr_node->next;
	}
	new_node = newI32Node(poss);
	new_node->next = square->possibilities;
	square->possibilities = new_node;
	return;
}

void
removePossFromSquare(struct Square* square, int32_t poss) {
	if(!square->possibilities) return;
	struct I32Node* prev_node = NULL;
	struct I32Node* curr_node = square->possibilities;
	while(curr_node) {
		if(curr_node->value == poss) {
			if(prev_node) {
				prev_node->next = curr_node->next;
			} else {
				square->possibilities = curr_node->next;
			}
			free(curr_node);
			curr_node = NULL;
		} else {
			prev_node = curr_node;
			curr_node = curr_node->next;			
		}
	}
	return;
}

struct I32Node*
newI32Node(int32_t value) {
	struct I32Node* new_node = wcalloc(1, sizeof(struct I32Node));
	new_node->next = NULL;
	new_node->value = value;
	return new_node;
}

void
updateSquarePossibilities(struct Board* board, size_t square_idx) {
	if(board->squares[square_idx].soln) return;
	bool possibilities[BOARD_LEN + 1];
	for(size_t i = 0; i < BOARD_LEN + 1; i++) {
		possibilities[i] = true;
	}
	size_t x = square_idx % BOARD_LEN;
	size_t y = square_idx / BOARD_LEN;
	for(size_t i = 0; i < BOARD_LEN; i++) {
		if(board->squares[i + (y * BOARD_LEN)].soln > 0) {
			possibilities[board->squares[i + (y * BOARD_LEN)].soln] = false;
		}
	}
	for(size_t i = 0; i < BOARD_LEN; i++) {
		if(board->squares[(i * BOARD_LEN) + x].soln) {
			possibilities[board->squares[(i * BOARD_LEN) + x].soln] = false;
		}
	}
	size_t locale;
	for(size_t i = 0; i < BOARD_LEN; i++) {
		for(size_t j = 0; j < BOARD_LEN; j++) {
			if(board->squares_in_locale[i][j] == square_idx) {
				locale = i;
			}
		}
	}

	for(size_t i = 0; i < BOARD_LEN; i++) {
		size_t comp_square_idx = board->squares_in_locale[locale][i];
		if(comp_square_idx == square_idx) continue;
		if(board->squares[comp_square_idx].soln) {
			possibilities[board->squares[comp_square_idx].soln] = false;
		}
	}

	for(int32_t i = 1; i <= BOARD_LEN; i++) {
		if(possibilities[i]) {
			addPossToSquare(&(board->squares[square_idx]), i);
		} else {
			removePossFromSquare(&(board->squares[square_idx]), i);
		}
	}
	return;
}

void
validateAllPossibilities(struct Board* board) {
	for(size_t i = 0; i < BOARD_AREA; i++) {
		struct I32Node* curr_node = board->squares[i].possibilities;
		int32_t num_possibilities = 0;
		int32_t verified_value = 0;
		while(curr_node) {
			num_possibilities++;
			verified_value = curr_node->value;
			curr_node = curr_node->next;
		}
		if(num_possibilities == 1) {
			board->squares[i].soln = verified_value;
			removePossFromSquare(&(board->squares[i]), verified_value);
			board->squares[i].possibilities = NULL;
		}
	}
	return;
}

bool
checkForWin(struct Board* board) {
	bool solved = true;
	board->num_cycles++;
	for(size_t i = 0; i < BOARD_AREA; i++) {
		if(!board->squares[i].soln) {
			solved = false;
		}
	}
	return solved;
}

void
TEST_printAllPossibilities(struct Board* board) {
	for(size_t i = 0; i < BOARD_AREA; i++) {
		printf("Square: %zu, with possibilities: ", i);
		struct I32Node* curr_node = board->squares[i].possibilities;
		while(curr_node) {
			printf("%d ", curr_node->value);
			curr_node = curr_node->next;
		}
		printf("\n");
	}
}

void
printSolution(struct Board* board) {
	printf("COMPLETED BOARD - %zu CYCLES\n\n", board->num_cycles);
	for(size_t i = 0; i < BOARD_LEN; i++) {
		for(size_t j = 0; j < BOARD_LEN; j++) {
			printf("%d ", board->squares[(i * BOARD_LEN) + j].soln);
		}
		printf("\n");
	}
	printf("\n");
	return;
}
