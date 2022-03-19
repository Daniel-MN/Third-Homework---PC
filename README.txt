//Musuroi Daniel-Nicusor
//Grupa:313CB


			Tema 3

		The Emperor's New Sudoku

	Tema a fost realizata folosind doua fisiere .c (cifre.c si sudoku.c)
si fisierele .h cifre.h (creata de mine), bmp_header.h si jsmn.h. Am ales
folosirea jsmn.h deoarece mi-a fost sugerata pe ocw si este destul de simpla
si usor de inteles, avand doar cateva structuri si nu foarte multe functii, 
spre deosebire de altele care sunt destul de incarcate. De asemenea, am 
creat diferite structuri precum: Pixel, Bitmap (ce contine lungimea si 
latimea bitmapului, dar si o matrice de Pixeli), Bmp_image (ce contine un 
pointer catre un file_header, unul catre un info_header si altul catre un 
bitmap), Square (o structua ce contine doar o matrice de pixeli folosita in
general pentru a retine un patrat de pixeli din cele 81 -sudoku 9x9- sau 
pentru crearea unor patrate de comparatie).
	Fisierul cifre.c contine diferite functii folositoare pentru 
scrierea cifrelor in patratele imaginii de sudoku sau pentru recunoasterea 
acestora dintr-o imagine.
	Alocarile dinamice de memorie au fost facute folosind functia 
calloc, iar ca eroare in cazul in care nu s-a putut aloca corect memorie 
se va ajunge la exit code-ul 303.
	Alte tipuri de erori:
1. exit code 5 - un fisier nu a putut fi deschis
2. exit code 202 - nu s-a reusit parsarea jsonului

	Task 1: task123 task1 input_to_outstring write_file 
parse_read_json size_file read_bitmap read_infoheader read_fileheader
	Taskul 1 face parsarea fisierului json dat ca input in linia de 
comanda folosind functii si structuri din jsmn.h, iar apoi citeste din
acesta cele trei structuri File Header, Info Header, BitMap. Cu ajutorul
functiei input_to_outstring se obtine numele fisierului de output pentru
fiecare fisier json. Functia write_file scrie intr-un fisier binar o 
structura Bmp_image.

	Task 2: task123 task2 mirror mirror_column
	Taskul 2 oglindeste imaginea obtinuta la taskul 1, oglindind 
fiecare coloana (1x9), apoi o scrie in fisierul de output folosind 
write_file.

	Task 3: task123 task3 sudoku input_to_outstring_task3 check_all 
check_3x3 check_columns check_lines
	Taskul 3 verifica daca sudokul este completat corect, rezultand
un fisier de output json cu un mesaj adecvat. Functia sudoku intoarce 
o matrice cu elementele tablei de sudoku. Pentru asta sunt folosite functii
precum get_patratel ( :))) ), care obtine o structura de tip square din
bitmap sau get_int, care recunoaste cifra scrisa in patratel si o intoarce.

	Task 4: task4 show_sudoku read_from_bmp sudoku solve_sudoku 
diference_matrix <primele 10 functii din cifre.c> write_file
	Taskul 4 citeste din fisierul binar bmp primit ca input si obtine
o structura de tip Bmp_imagine pe care o prelucreaza. Functia show_sudoku, 
cu ajutorul functiei sudoku, obtine sub forma de matrice tabla de sudoku, 
avand 0 in spatiile goale si se face o copie a acestei table. Apoi,
folosind functia solve_sudoku(backtracking), se pun in loc de 0, cifrele 
corespunzatoare astfel incat tabla de sudoku sa fie completa (daca acest 
lucru se poate, dar la task 4 toate jocurile date se pot completa). Din 
matricea astfel obtinuta se va scadea matricea initiala (din tabla obtinuta
cu solve_sudoku se va scadea tabla initiala - i-am facut o copie). 
Rezultatul acestei scaderi va fi o matrice care va avea 0 pe spatiile 
completate si cifre pe spatiile completate cu solve sudoku. Astfel, 
folosind primele 10 functii din cifre.c putem scrie cifrele cu magenta 
in bitmap in locurile indicate de tabla rezultata in urma scaderii. 
Structura Bmp_image obtinuta prin modificarea structurii Bitmap este 
scrisa in fisierul de output folosind write_file.

	Bonus: bonus show_sudoku read_from_bmp sudoku solve_sudoku 
diference_matrix <primele 10 functii din cifre.c> write_file
	La bonus se face in mare acelasi lucru ca la task 4, singurele
diferente fiind ca aici testele sunt mai complexe si ca se poate ca o
tabla de sudoku sa nu poata fi rezolvata. Daca tabla primita la input este
rezolvabila atunci se face acelasi lucru ca la task 4, dar daca tabla nu 
poate fi rezolvata, atunci functia show_sudoku, folosind functia X din 
cifre.c, va pune un X rosu in fiecare patrat si va afisa imaginea asa.