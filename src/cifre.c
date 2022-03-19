#include "bmp_header.h"
#include "cifre.h"

//functiile urmatoare scriu in square-ul 
//patrat cifra data de numele functiei sau
//litera X:
void X(Pixel **map, Pixel roz, int start_x, int start_y){
    int i, j;
    Pixel alb = {.R = 255, .G = 255, .B = 255};

    for (i = 0; i < 7; i++){
        for (j = 0; j < 7; j++){
            map[start_y + i][start_x + j] = alb;
        }
    }

    for (i = 1; i < 6; i++){
        map[start_y + i][start_x + i] = roz;
        map[start_y + i][start_x + 6 - i] = roz;
    }
}

void unu(Pixel **map, Pixel roz, int start_x, int start_y){
    int i;
    for (i = 1; i < 6; i++){
        map[start_y + i][start_x + 5] = roz;
    }
}

void doi(Pixel **map, Pixel roz, int start_x, int start_y){
    int i;
    for (i = 1; i < 6; i++){
        map[start_y + 1][start_x + i] = roz;
        map[start_y + 3][start_x + i] = roz;
        map[start_y + 5][start_x + i] = roz;
    }
    map[start_y + 2][start_x + 5] = roz;
    map[start_y + 4][start_x + 1] = roz;
}

void trei(Pixel **map, Pixel roz, int start_x, int start_y){
    int i;
    for (i = 1; i < 6; i++){
        map[start_y + 1][start_x + i] = roz;
        map[start_y + 3][start_x + i] = roz;
        map[start_y + 5][start_x + i] = roz;
    }
    map[start_y + 2][start_x + 5] = roz;
    map[start_y + 4][start_x + 5] = roz;
}

void patru(Pixel **map, Pixel roz, int start_x, int start_y){
    int i;
    for (i = 1; i < 6; i++){
        map[start_y + i][start_x + 5] = roz;
        map[start_y + 3][start_x + i] = roz;
    }
    map[start_y + 1][start_x + 1] = roz;
    map[start_y + 2][start_x + 1] = roz;
}

void cinci(Pixel **map, Pixel roz, int start_x, int start_y){
    int i;
    for (i = 1; i < 6; i++){
        map[start_y + 1][start_x + i] = roz;
        map[start_y + 3][start_x + i] = roz;
        map[start_y + 5][start_x + i] = roz;
    }
    map[start_y + 2][start_x + 1] = roz;
    map[start_y + 4][start_x + 5] = roz;
}

void sase(Pixel **map, Pixel roz, int start_x, int start_y){
    int i;
    for (i = 1; i < 6; i++){
        map[start_y + 1][start_x + i] = roz;
        map[start_y + 3][start_x + i] = roz;
        map[start_y + 5][start_x + i] = roz;
    }
    map[start_y + 2][start_x + 1] = roz;
    map[start_y + 4][start_x + 5] = roz;
    map[start_y + 4][start_x + 1] = roz;
}

void sapte(Pixel **map, Pixel roz, int start_x, int start_y){
    int i;
    for (i = 1; i < 6; i++){
        map[start_y + i][start_x + 5] = roz;
        map[start_y + 1][start_x + i] = roz;
    }
}

void opt(Pixel **map, Pixel roz, int start_x, int start_y){
    sase(map, roz, start_x, start_y);
    map[start_y + 2][start_x + 5] = roz;
}

void noua(Pixel **map, Pixel roz, int start_x, int start_y){
    cinci(map, roz, start_x, start_y);
    map[start_y + 2][start_x + 5] = roz;
}

Square *alloc_cifre(){
    Square *cifre;
    int i, j, k;
    cifre = (Square *)calloc(10, sizeof(Square));
    for (k = 0; k < 10; k++){
        cifre[k].map = (Pixel **)calloc(7, sizeof(Pixel *));
        if (cifre[k].map == NULL){
            exit(303);
        }
        for (i = 0; i < 7; i++){
            cifre[k].map[i] = (Pixel *)calloc(7, sizeof(Pixel));
            if (cifre[k].map[i] == NULL){
                exit(303);
            }
        }
    }

    Pixel roz = {.R = 255, .G = 175, .B = 175};
    Pixel alb = {.R = 255, .G = 255, .B = 255};

    //fac toti pixelii albi, urmand ca apoi sa colorez cu roz
    //fiecare cifra
    for (k = 0; k < 10; k++){
        for (i = 0; i < 7; i++){
            for (j = 0; j < 7; j++){
                cifre[k].map[i][j] = alb;
            }
        }
    }

    unu(cifre[1].map, roz, 0, 0);
    doi(cifre[2].map, roz, 0, 0);
    trei(cifre[3].map, roz, 0, 0);
    patru(cifre[4].map, roz, 0, 0);
    cinci(cifre[5].map, roz, 0, 0);
    sase(cifre[6].map, roz, 0, 0);
    sapte(cifre[7].map, roz, 0, 0);
    opt(cifre[8].map, roz, 0, 0);
    noua(cifre[9].map, roz, 0, 0);
    

    return cifre;
}

void free_cifre(Square *cifre){
    int k, i;

    for (k = 0; k < 10; k++){
        for (i = 0; i < 7; i++){
            free(cifre[k].map[i]);
        }
        free(cifre[k].map);
    }
    free(cifre);
}

//returneaza 0 daca pixelii au culori diferite
//returneaza 1 daca pixelii au aceeasi culoare
int cmp_pixels(Pixel p1, Pixel p2){
    if (p1.R != p2.R){
        return 0;
    }

    if(p1.G != p2.G){
        return 0;
    }

    if(p1.B != p2.B){
        return 0;
    }

    return 1;
}

//compara un patratel din sudoku din cele 81 (9 x 9)
//cu o cifra
int cmp_squares(Square cifra, Square patratel){
    int i, j;
    for (i = 0; i < 7; i++){
        for (j = 0; j < 7; j++){
            if (cmp_pixels(cifra.map[i][j], patratel.map[i][j]) == 0){
                return 0;
            }
        }
    }

    return 1;
}

void get_patratel(Bitmap *bitmap, Square *patratel, int start_x, int start_y){
    int i, j;

    for (i = 0; i < 7; i++){
        for (j = 0; j < 7; j++){
            patratel->map[i][j] = bitmap->map[start_y + i][start_x + j];
        }
    }
}

int get_int(Square patratel){
    Square *cifre;
    cifre = alloc_cifre();

    int i;
    for (i = 0; i < 10; i++){
        if (cmp_squares(cifre[i], patratel)){
            free_cifre(cifre);
            return i;
        }
    }

    free_cifre(cifre);
    return Suma_cifrelor;
}

int **sudoku(Bitmap *bitmap){
    int i, j;
    int **tabla;
    tabla = (int **)calloc(9, sizeof(int *));
    if (tabla == NULL){
        exit(303);
    }
    for (i = 0; i < 9; i++){
        tabla[i] = (int *)calloc(9, sizeof(int));
        if (tabla[i] == NULL){
            exit(303);
        }
    }
    
    int start_x = 1;
    int start_y = 1;

    Square patratel;
    patratel.map = (Pixel **)calloc(7, sizeof(Pixel *));
    if (patratel.map == NULL){
        exit(303);
    }

    for (i = 0; i < 7; i++){
        patratel.map[i] = (Pixel *)calloc(7, sizeof(Pixel));
        if (patratel.map[i] == NULL){
            exit(303);
        }
    }

    for (i = 0; i < 9; i++){
        for (j = 0; j < 9; j++){
            get_patratel(bitmap, &patratel, start_x, start_y);
            tabla[i][j] = get_int(patratel);
            start_x += 8;
        }
        start_y += 8;
        start_x = 1;
    }

    for (i = 0; i < 7; i++){
        free(patratel.map[i]);
    }
    free(patratel.map);
    return tabla;
}

