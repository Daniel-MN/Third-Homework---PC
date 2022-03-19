#ifndef CIFRE
#define CIFRE

void X(Pixel **map, Pixel roz, int start_x, int start_y);

void unu(Pixel **map, Pixel roz, int start_x, int start_y);

void doi(Pixel **map, Pixel roz, int start_x, int start_y);

void trei(Pixel **map, Pixel roz, int start_x, int start_y);

void patru(Pixel **map, Pixel roz, int start_x, int start_y);

void cinci(Pixel **map, Pixel roz, int start_x, int start_y);

void sase(Pixel **map, Pixel roz, int start_x, int start_y);

void sapte(Pixel **map, Pixel roz, int start_x, int start_y);

void opt(Pixel **map, Pixel roz, int start_x, int start_y);

void noua(Pixel **map, Pixel roz, int start_x, int start_y);

Square *alloc_cifre();

void free_cifre(Square *cifre);

int cmp_pixels(Pixel p1, Pixel p2);

int cmp_squares(Square cifra, Square patratel);

void get_patratel(Bitmap *bitmap, Square *patratel, 
int start_x, int start_y);

int get_int(Square patratel);

int **sudoku(Bitmap *bitmap);

#endif