CC = gcc
CFLAGS = -Wall -Wextra
SRC = src
EXE = sudoku
OBJ = sudoku.o cifre.o
	
all: $(EXE)

build: $(EXE)

sudoku: $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(CFLAGS)

sudoku.o: $(SRC)/sudoku.c
	$(CC) -c $(SRC)/sudoku.c $(CFLAGS)

cifre.o: $(SRC)/cifre.c
	$(CC) -c $(SRC)/cifre.c $(CFLAGS)

run: $(EXE)
	./$(EXE)

.PHONY : clean
clean :
	rm -rf $(EXE) *~