CC = gcc
CFLAGS = -Wall -Wpedantic -Werror #--std=c89
#LDFLAGS = -lpanel -lncurses

## sudo apt install libncurses5-dev  and/or libc6-dev for stdio.h

OS := $(shell uname)

OBJS =	./obj/memory.o 		\
		./obj/sudoku_solver.o \
		./obj/main.o 	\

autorun1 := $(shell mkdir -p obj)
autorun2 := $(shell mkdir -p bin)
autorun4 := $(shell mkdir -p saves)

main-run: main-build
ifeq ($(OS),Darwin)
	./bin/sudoku_solver.run
else
	 ./bin/sudoku_solver.run
endif

main-build: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o ./bin/sudoku_solver.run $(LDFLAGS)

./obj/%.o: ./src/%.c ./src/%.h
	$(CC) $(CFLAGS) -c $< -o $@
