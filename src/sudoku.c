#include "bmp_header.h"
#include "cifre.h"
#include "jsmn.h"


//in cazul in care memoria nu se aloca corect se returneaza codul 303
int size_file(FILE *file){
    int SIZE;
    fseek(file, 0, SEEK_END);

    SIZE = ftell(file);

    fseek(file, 0, SEEK_SET);

    return SIZE;
}

//verifica daca un token este cheia dorita sau nu
//returnneaza 1 daca da si 0 daca nu
int eqtoken_key(jsmntok_t *tok, char *key, char *json){

    if (tok->type != JSMN_STRING){
        return 0;
    }

    if ((int)strlen(key) != tok->end - tok->start){
        return 0;
    }

    if (strncmp(json + tok->start, key, tok->end - tok->start) != 0){
        return 0;
    }

    return 1;
}

//transforma un token intr-un int
int tok_to_int(jsmntok_t *tok, char *json){
    char *buffer;
    buffer = (char *)calloc(Max_len_str_int, sizeof(char));
    if (buffer == NULL){
        exit(303);
    }

    strncpy(buffer, json + tok->start, tok->end - tok->start);

    int n = atoi(buffer);
    free(buffer);

    return n;
}

//transforma un token untr-un string
char *tok_to_string(jsmntok_t *tok, char *json){

    char *result;
    result = (char *)calloc(tok->end - tok->start + 1, sizeof(char));
    if (result == NULL){
        exit(303);
    }

    strncpy(result, json + tok->start, tok->end - tok->start);

    return result;
}

//citeste structura bitmap creata de mine
Bitmap *read_bitmap(char *json, jsmntok_t *t, int r){
    Bitmap *bitmap;

    bitmap = (Bitmap *)calloc(1, sizeof(Bitmap));
    if(bitmap == NULL){
        exit(303);
    }

    char *buffer;
    buffer = (char *)calloc(Max_len_str_int, sizeof(char));
    if (buffer == NULL){
        exit(303);
    }

    //am luat dimensiunile bitmap inainte pentru a aloca dinamic matricea
    //de pixeli map
    if (eqtoken_key(&t[r-2], "height", json)){
        bitmap->height = tok_to_int(&t[r-1], json);
    } else {
        printf("Format incorect json: height\n");
    }

    if (eqtoken_key(&t[r-10], "width", json)){
        bitmap->width = tok_to_int(&t[r-9], json);
    }

    bitmap->map = (Pixel **)calloc(bitmap->height, sizeof(Pixel *));
    if (bitmap->map == NULL){
        exit(303);
    }

    int i, j;
    for (i = 0; i < bitmap->height; i++){
        bitmap->map[i] = (Pixel *)calloc(bitmap->width, sizeof(Pixel));
        if (bitmap->map[i] == NULL){
            exit(303);
        }
    }

    //k este indicele tokenului la care ma aflu
    //primul token ce reprezinta o culoare de bit este tokenul de pe 
    //pozitia 3
    int k = 3;
    for (i = 0; i < bitmap->height; i++){
        for (j = 0; j < bitmap->width; j++){
            bitmap->map[i][j].B = tok_to_int(&t[k], json);
            bitmap->map[i][j].G = tok_to_int(&t[k + 1], json);
            bitmap->map[i][j].R = tok_to_int(&t[k + 2], json);
            k += 3;
        }
    }

    free(buffer);

    return bitmap;
}

//citeste din fisierul json file_header
bmp_fileheader *read_fileheader(
    char *json, 
    jsmntok_t *t,
    Bitmap *bitmap){

    bmp_fileheader *header;
    header = (bmp_fileheader *)calloc(1, sizeof(bmp_fileheader));
    if (header == NULL){
        exit(303);
    }

    int index_fileheader = bitmap->height * bitmap->width * 3 + 3;

    int i;
    char *buffer;
    if (eqtoken_key(&t[index_fileheader], "file_header", json)){
        index_fileheader += 2;
        for (i = 0; i < Fileheader_size_tokens; i++){
            if (eqtoken_key(&t[index_fileheader + i], "signature", json)){
                i++;
                buffer = tok_to_string(&t[index_fileheader + i], json);
                header->fileMarker1 = buffer[0];
                header->fileMarker2 = buffer[1];
            }

            if (eqtoken_key(&t[index_fileheader + i], "file_size", json)){
                i++;
                header->bfSize = tok_to_int(&t[index_fileheader + i], 
                json);
            }

            if (eqtoken_key(&t[index_fileheader + i], "reserved", json)){
                unsigned short *p;
                int n;
                i++;
                n = tok_to_int(&t[index_fileheader + i], json);
                p = (unsigned short *)&n;
                header->unused1 = *p;
                header->unused2 = *(p + 1);
            }

            if (eqtoken_key(&t[index_fileheader + i], "offset", json)){
                i++;
                header->imageDataOffset = tok_to_int(
                    &t[index_fileheader + 1], json);
            }
        }
    }

    free(buffer);

    return header;
}

//citeste din fisierul json info_header
bmp_infoheader *read_infoheader(
    char *json,
    jsmntok_t *t,
    Bitmap *bitmap){

    bmp_infoheader *header;
    header = (bmp_infoheader *)calloc(1, sizeof(bmp_infoheader));
    if (header == NULL){
        exit(303);
    }
        
    int index = 3 + bitmap->width * bitmap->height * 3 + 10;


    int i;
    if (eqtoken_key(&t[index], "info_header", json)){
        index += 2;
        for (i = 0; i < Infoheader_size_tokens; i++){
            if (eqtoken_key(&t[index + i], "size", json)){
                i++;
                header->biSize = tok_to_int(&t[index + i], json);
            }

            if (eqtoken_key(&t[index + i], "width", json)){
                i++;
                header->width = tok_to_int(&t[index + i], json);
            }

            if (eqtoken_key(&t[index + i], "height", json)){
                i++;
                header->height = tok_to_int(&t[index + i], json);
            }

            if (eqtoken_key(&t[index + i], "planes", json)){
                i++;
                header->planes = tok_to_int(&t[index + i], json);
            }

            if (eqtoken_key(&t[index + i], "bit_count", json)){
                i++;
                header->bitPix = tok_to_int(&t[index + i], json);
            }

            if (eqtoken_key(&t[index + i], "compression", json)){
                i++;
                header->biCompression = tok_to_int(&t[index + i], json);
            }

            if (eqtoken_key(&t[index + i], "image_size", json)){
                i++;
                header->biSizeImage = tok_to_int(&t[index + i], json);
            }

            if (eqtoken_key(&t[index + i], "width", json)){
                i++;
                header->width = tok_to_int(&t[index + i], json);
            }

            if (eqtoken_key(&t[index + i], "x_pixels_per_meter", json)){
                i++;
                header->biXPelsPerMeter = tok_to_int(&t[index + i], json);
            }

            if (eqtoken_key(&t[index + i], "y_pixels_per_meter", json)){
                i++;
                header->biYPelsPerMeter = tok_to_int(&t[index + i], json);
            }

            if (eqtoken_key(&t[index + i], "colors_used", json)){
                i++;
                header->biClrUsed = tok_to_int(&t[index + i], json);
            }

            if (eqtoken_key(&t[index + i], "colors_important", json)){
                i++;
                header->biClrImportant = tok_to_int(&t[index + i], json);
            }
        }
    }

    return header;
}

//parseaza json-ul si apoi cu ajutorul celorlalte functii
//returneaza o Bmp_image, structura creata de mine
Bmp_image *parse_read_json(char input[]){

    FILE *in = fopen(input, "rt");
    if (in == NULL){
        exit(5);
    }

    int SIZE = size_file(in);
    char *json;
    json = (char *)calloc(SIZE + 1, sizeof(char));
    if (json == NULL){
        exit(303);
    }

    fread(json, sizeof(char), SIZE, in);
    fclose(in);

    jsmn_parser p;
    //Nu vor avea cum sa fie mai multe tokenuri decat marimea fisierului
    jsmntok_t *t;
    t = (jsmntok_t *)calloc(SIZE, sizeof(jsmntok_t));
    if (t == NULL){
        exit(303);
    }

    jsmn_init(&p);

    int r;
    r = jsmn_parse(&p, json, strlen(json), t, SIZE);

    if (r < 0) {
        exit(202);
    }

    Bmp_image *image;
    image = (Bmp_image *)calloc(1, sizeof(Bmp_image));

    image->bitmap = read_bitmap(json, t, r);

    image->file_header = read_fileheader(json, t, image->bitmap);

    image->info_header = read_infoheader(json, t, image->bitmap);

    free(json);
    free(t);

    return image;
}

//in functie de numele fisierului json, returneaza un
//string cu numele fisierului de output
char *input_to_outstring(char input[], int task){
    char *output;
    output = (char *)calloc(Len_str_bmpname, sizeof(char));
    if (output == NULL){
        exit(303);
    }

    output[0] = input[strlen(input) - 7];
    output[1] = input[strlen(input) - 6];
    output[2] = '\0';

    int n;
    n = atoi(output);

    if (n < 10){
        sprintf(output, "output_task%d_board0%d.bmp", task, n);
    } else {
        sprintf(output, "output_task%d_board%d.bmp", task, n);
    }

    return output;
}


//scrie in fisierul binar de output
void write_file(Bmp_image *image, FILE *out){
    fwrite(image->file_header, sizeof(bmp_fileheader), 1, out);

    fwrite(image->info_header, sizeof(bmp_infoheader), 1, out);


    int nr_padding;
    nr_padding = 4 - (image->bitmap->width * 3) % 4;
    if (nr_padding == 4){
        nr_padding = 0;
    }

    int i, j;
    for (i = image->bitmap->height - 1; i > -1; i--){
        for (j = 0; j < image->bitmap->width; j++){
            fwrite(&image->bitmap->map[i][j].B, sizeof(unsigned char), 1, out);
            fwrite(&image->bitmap->map[i][j].G, sizeof(unsigned char), 1, out);
            fwrite(&image->bitmap->map[i][j].R, sizeof(unsigned char), 1, out);
        }

        for (j = 0; j < nr_padding; j++){
            unsigned char n = 0;
            fwrite(&n, sizeof(unsigned char), 1, out);
        }
    }
}

//creaza o imagine bmp, avand ca input doar numele 
//fisierului json din care citeste datele
void task1(char input[], Bmp_image *image){

    char *output;
    output = input_to_outstring(input, 1);
    FILE *out = fopen(output, "wb");
    if (out == NULL){
        exit(5);
    }

    free(output);

    write_file(image, out);

    fclose(out);
}

//oglindeste o coloana(9 casute x 1 casuta)
void mirror_column(Bitmap *bitmap, int start, int nr_pixels){
    Pixel p;

    int i, j;
    for (i = 0; i < bitmap->height; i++){
        for (j = 0; j < nr_pixels / 2; j++){
            p = bitmap->map[i][start + j];
            bitmap->map[i][start + j] = bitmap->
            map[i][start + nr_pixels - j - 1];
            bitmap->map[i][start + nr_pixels - j - 1] = p;
        }
    }
}

//oglindeste imaginea
void mirror(Bmp_image *image){
    int nr_pixels = ((image->bitmap->width - 4) / 3 - 2) / 3;
    int start = 1;
    
    int i;
    for (i = 0; i < 9; i++){
        mirror_column(image->bitmap, start, nr_pixels);
        start += nr_pixels;
        start++;
    }
}

void task2(char input[], Bmp_image *image){
    mirror(image);

    char *output;
    output = input_to_outstring(input, 2);
    FILE *out = fopen(output, "wb");
    if (out == NULL){
        exit(5);
    }

    free(output);

    write_file(image, out);

    fclose(out);
}

//in functie de numele fisierului de input
//returneaza un string cu numele fisierului json de output
char *input_to_outstring_task3(char input[]){
    char *output;
    output = (char *)calloc(Len_str_jsonname, sizeof(char));
    if (output == NULL){
        exit(303);
    }

    output[0] = input[strlen(input) - 7];
    output[1] = input[strlen(input) - 6];
    output[2] = '\0';

    int n;
    n = atoi(output);

    if (n < 10){
        sprintf(output, "output_task3_board0%d.json", n);
    } else {
        sprintf(output, "output_task3_board%d.json", n);
    }

    return output;
}

int check_lines(int **tabla){
    int i, j;
    //suma pe fiecare linie (fiecare coloana sau fiecare 
    //patrat 3x3)trebuie sa fie 45, altfel pe linia
    //(coloana sau patratelul 3x3) ceva nu este in regula
    int sum;
    for (i = 0; i < 9; i++){
        sum = 0;
        for (j = 0; j < 9; j++){
            sum += tabla[i][j];
        }
        if (sum != Suma_cifrelor){
            return 0;
        }
    }

    return 1;
}

int check_columns(int **tabla){
    int i, j;
    //suma pe fiecare linie (fiecare coloana sau fiecare 
    //patrat 3x3)trebuie sa fie 45, altfel pe linia
    //(coloana sau patratelul 3x3) ceva nu este in regula
    int sum;
    for (i = 0; i < 9; i++){
        sum = 0;
        for (j = 0; j < 9; j++){
            sum += tabla[j][i];
        }
        if (sum != Suma_cifrelor){
            return 0;
        }
    }

    return 1;
}

int check_3x3(int **tabla){
    int i, j;
    //suma pe fiecare linie (fiecare coloana sau fiecare 
    //patrat 3x3)trebuie sa fie 45, altfel pe linia
    //(coloana sau patratelul 3x3) ceva nu este in regula
    int sum;
    for (i = 0; i < 9; i = i + 3){
        for (j = 0; j < 9; j = j + 3){
            sum = tabla[i][j] + tabla[i][j + 1] + tabla[i][j + 2] + 
            tabla[i + 1][j] + tabla[i + 1][j + 1] + tabla[i + 1][j + 2] + 
            tabla[i + 2][j] + tabla[i + 2][j + 1] + tabla[i + 2][j + 2];

            if (sum != Suma_cifrelor){
                return 0;
            }
        }
    }

    return 1;
}

int check_all(int **tabla){
    if (check_lines(tabla) && check_columns(tabla) && check_3x3(tabla)){
        return 1;
    }
    return 0;
}

void task3(char input[], Bmp_image *image){
    int i;
    int **tabla;
    tabla = sudoku(image->bitmap);

    char *output;
    output = input_to_outstring_task3(input);
    FILE *out = fopen(output, "wt");
    if (out == NULL){
        exit(5);
    }

    free(output);

    fprintf(out, "{\n\t\"input_file\":");
    fprintf(out, "\t\"%s\",\n", input + 14);
    fprintf(out, "\t\"game_state\":");

    if (check_all(tabla)){
        fprintf(out, "\t\"Win!\"\n}\n");
    } else {
        fprintf(out, "\t\"Loss :(\"\n}\n");
    }

    for (i = 0; i < 9; i++){
        free(tabla[i]);
    }
    free(tabla);

    fclose(out);
}

void free_image(Bmp_image *image){
    int i;
    for (i = 0; i < 73; i++){
        free(image->bitmap->map[i]);
    }
    free(image->bitmap->map);
    free(image->bitmap);
    free(image->info_header);
    free(image->file_header);
    free(image);
}

void task123(char input[]){
    Bmp_image *image;
    image = parse_read_json(input);

    task1(input, image);
    task2(input, image);
    task3(input, image);
    
    free_image(image);
}

Bmp_image *alloc_image(){
    Bmp_image *image;
    image = (Bmp_image *)calloc(1, sizeof(Bmp_image));
    image->file_header = (bmp_fileheader *)calloc(1, sizeof(bmp_fileheader));
    image->info_header = (bmp_infoheader *)calloc(1, sizeof(bmp_infoheader));
    image->bitmap = (Bitmap *)calloc(1, sizeof(Bitmap));
    image->bitmap->width = 73;
    image->bitmap->height = 73;

    image->bitmap->map = (Pixel **)calloc(73, sizeof(Pixel *));
    
    if (image == NULL || image->file_header == NULL ||
     image->info_header == NULL || image->bitmap == NULL ||
     image->bitmap->map == NULL){
        exit(303);
    }
    
    int i;
    for (i = 0; i < 73; i++){
        image->bitmap->map[i] = (Pixel *)calloc(73, sizeof(Pixel));
        if (image->bitmap->map[i] == NULL){
            exit(303);
        }
    }

    return image;
}

Bmp_image *read_from_bmp(char input[]){
    Bmp_image *image;
    image = alloc_image();

    FILE *in = fopen(input, "rb");
    if (in == NULL){
        exit(5);
    }

    fread(image->file_header, sizeof(bmp_fileheader), 1, in);
    fread(image->info_header, sizeof(bmp_infoheader), 1, in);

    int i, j;
    for (i = 72; i > -1; i--){
        for (j = 0; j < 73; j++){
            fread(&image->bitmap->map[i][j].B, sizeof(unsigned char), 1, in);
            fread(&image->bitmap->map[i][j].G, sizeof(unsigned char), 1, in);
            fread(&image->bitmap->map[i][j].R, sizeof(unsigned char), 1, in);
        }
        fseek(in, 1, SEEK_CUR);
    }

    fclose(in);

    return image;
}

//returneaza numarul care ar trebui scris in casuta
//goala daca aceasta este singura de pe linie
//returneaza 0 daca nu este singura goala
int nr_line(int **tabla, int line){
    int i, nr = 0, sum = 0;
    for (i = 0; i < 9; i++){
        if (tabla[line][i] == 0){
            nr++;
        } else {
            sum += tabla[line][i];
        }
    }
    if (nr == 1){
        return 45 - sum;
    } else {
        return 0;
    }
}

//returneaza numarul care ar trebui scris in casuta
//goala daca aceasta este singura de pe coloana
//returneaza 0 daca nu este singura goala
int nr_column(int **tabla, int column){
    int i, nr = 0, sum = 0;
    for (i = 0; i < 9; i++){
        if (tabla[i][column] == 0){
            nr++;
        } else {
            sum += tabla[i][column];
        }
    }
    if (nr == 1){
        return 45 - sum;
    } else {
        return 0;
    }
}

int check_valid(int **tabla, int n, int line, int col){
    int i, j;
    for (i = 0; i < 9; i++){
        if (tabla[line][i] == n){
            return 0;
        }
    }

    for (i = 0; i < 9; i++){
        if (tabla[i][col] == n){
            return 0;
        }
    }

    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            if (tabla[line - line % 3 + i][col - col % 3 + j] == n){
                return 0;
            }
        }
    }

    return 1;
}

int solve_sudoku(int **tabla, int line, int col){
    int n;
    col++;
    if (col == 9){
        line++;
        col = 0;
    }

    if (line == 9){
        return 1;
    }


    if (tabla[line][col] != 0){
        return solve_sudoku(tabla, line, col);
    }

    //cazul cand tabla[line, col] = 0:
    for (n = 1; n < 10; n++){
        if (check_valid(tabla, n, line, col)){
            tabla[line][col] = n;
            if (solve_sudoku(tabla, line, col)){
                return 1;
            }

        }
        //in cazul in care nu se poate rezolva cu cifra n la
        //pozitia line, col va reveni la aceasta pozitie si
        //o va face din nou 0, urmand sa incerce din nou cu 
        //urmatoarea cifra in cazul in care aceasta exista 
        //(n < 9) sau sa ajunga la concluzia ca nu se poate
        //si sa revina la pasul anterior
        tabla[line][col] = 0;
    }

    //in cazul in care se iese din for inseamna ca nu s-a gasit
    //o cifra valida si se va intoarce la pasul anterior
    return 0;
}

//calculeaza diferenta dintre doua matrice, rezultatul fiind
//prima matrice data ca parametru
void diference_matrix(int **matrix1, int **matrix2){
    int i, j;
    for (i = 0; i < 9; i++){
        for (j = 0; j < 9; j++){
            matrix1[i][j] = matrix1[i][j] - matrix2[i][j];
        }
    }
}

//citeste imaginea data ca input si completeaza spatiile libere
Bmp_image *show_sudoku(char input[]){

    void (*write_digit[10])(Pixel **, Pixel, int, int) = {X, unu, doi, 
    trei, patru, cinci, sase, sapte, opt, noua};

    Pixel magenta = {.R = 255, .G = 0, .B = 255};
    Pixel rosu = {.R = 255, .G = 0, .B = 0};

    int i, j;
    Bmp_image *image;
    image = read_from_bmp(input);

    int **tabla;
    tabla = sudoku(image->bitmap);

    int **tabla_copy;
    tabla_copy = (int **)calloc(9, sizeof(int *));
    if (tabla_copy == NULL){
        exit(303);
    }
    for (i = 0; i < 9; i++){
        tabla_copy[i] = (int *)calloc(9, sizeof(int));
        if (tabla_copy[i] == NULL){
            exit(303);
        }
    }


    for (i = 0; i < 9; i++){
        for (j = 0; j < 9; j++){
            tabla_copy[i][j] = tabla[i][j];
        }
    }

    //daca sudoku a fost rezolvat, atunci se calculeaza diferenta
    //dintre matricea finala dupa adaugarea cifrelor si 
    //matricea initiala, in matricea rezultat avand elementele >0
    //elementele ce trebuie completate pe imagine
    if (solve_sudoku(tabla, 0, -1)){

        diference_matrix(tabla, tabla_copy);

        for (i = 0; i < 9; i++){
            for (j = 0; j < 9; j++){
                if (tabla[i][j] != 0){
                    write_digit[tabla[i][j]](image->bitmap->map, magenta, 
                    8 * j + 1, 8 * i + 1);
                }
            }
        }

    } else {
        for (i = 0; i < 9; i++){
            for (j = 0; j < 9; j++){
                write_digit[0](image->bitmap->map, rosu, 
                 8 * j + 1, 8 * i + 1);
            }
        }
    }

    for (i = 0; i < 9; i++){
        free(tabla_copy[i]);
    }
    free(tabla_copy);

    for (i = 0; i < 9; i++){
        free(tabla[i]);
    }
    free(tabla);

    return image;
}

void task4(char input[]){
    Bmp_image *image;
    image = show_sudoku(input);

    char *output;
    output = (char *)calloc(Len_str_bmpname, sizeof(char));
    if (output == NULL){
        exit(303);
    }

    output[0] = input[strlen(input) - 6];
    output[1] = input[strlen(input) - 5];
    output[2] = '\0';

    int n;
    n = atoi(output);

    if (n < 10){
        sprintf(output, "output_task4_board0%d.bmp", n);
    } else {
        sprintf(output, "output_task4_board%d.bmp", n);
    }

    FILE *out = fopen(output, "wb");
    if (out == NULL){
        exit(5);
    }

    write_file(image, out);

    fclose(out);

    free(output);

    free_image(image);
}

void bonus(char input[]){
    Bmp_image *image;
    image = show_sudoku(input);

    char *output;
    output = (char *)calloc(Len_str_bmpname, sizeof(char));
    if (output == NULL){
        exit(303);
    }

    output[0] = input[strlen(input) - 6];
    output[1] = input[strlen(input) - 5];
    output[2] = '\0';

    int n;
    n = atoi(output);

    if (n < 10){
        sprintf(output, "output_bonus_board0%d.bmp", n);
    } else {
        sprintf(output, "output_bonus_board%d.bmp", n);
    }

    FILE *out = fopen(output, "wb");
    if (out == NULL){
        exit(5);
    }

    write_file(image, out);

    fclose(out);

    free(output);

    free_image(image);
}

int main(int argc, char *argv[]){
    int number;

    if (argc != 3){
        exit(30);
    }

    number = atoi(argv[2]);
    switch(number){
        case 123:{
            task123(argv[1]);
            break;
        }
        case 4:{
            task4(argv[1]);
            break;
        }
        default:{
            bonus(argv[1]);
            break;
        }
    }
    return 0;
}