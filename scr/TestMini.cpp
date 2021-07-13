#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_GRAF 101
#define INFINITY 10000

double graf[MAX_GRAF][MAX_GRAF];   //tablica kwadratowa na macierz sasiedstwa
double k_graf[MAX_GRAF][MAX_GRAF]; //kopia tablicy
int tmp = 0;                       //Liczba zmienna
int* t = &tmp;                     //Wskaznik na zmienna tmp


void finder_path(double graf[][MAX_GRAF], int a, int b, int* w_top, double k_graf[][MAX_GRAF]);

void read_parametrs(FILE* p, int* f_top, int* f_path, double graf[][MAX_GRAF]);

void read_paths(FILE* in, double graf[][MAX_GRAF], int* w_r_p, double k_graf[][MAX_GRAF], double va[MAX_GRAF]);


int main(int argc, char* argv[]) {
	int top = 0;                   //Liczba wierzcholkow
	int path = 0;                  //Liczba sciezek
	int read_path = 0;             //Liczba wczytanych sciezek (do sprawdzania poprawnosci)
	int* w_top = &top;             //Wskaznik na wierzcholki
	int* w_path = &path;           //Wskaznik na sciezki
	int* w_read_path = &read_path; //Wskaznik liczbe wczytanych sciezek
	double v;                      //Wygenerowana wartosc losowa
	double value_t[MAX_GRAF];      //Tablica wszystkich losowych wartosci wierzcholkow
	srand(time(NULL));

	FILE* in = argc > 1 ? fopen(argv[1], "r") : fopen("graf5x5.txt", "r");
	if (in == NULL) {
		printf("Nie udalo sie otworzyc pliku");
		exit(1);
	}

	//wczytanie wstepnych parametrow
	read_parametrs(in, w_top, w_path, graf);
	
	//zerowanie tablicy i generowanie wag dla wierzcholkow
	for (int a = 0; a < top; a++) {
		v = rand() % 1001;
		v = v / 100;
		value_t[a+1] = v;
		for (int b = 0; b < top; b++)
			graf[a][b] = 0;
	}
	//wczytanie wszystkich sciezek
	read_paths(in, graf, w_read_path, k_graf, value_t);
	
	//wypisanie wczytanych informacji
	printf("INFORMACJE O GRAFIE Z PLIKU: \n");
	for (int a = 0; a <= top; a++) {
		printf("%5d.", a);
	}
	printf("\n");
	for (int a = 1; a <= top; a++) {

		printf("%3d.|", a);
		for (int b = 1; b <= top; b++)
			printf(" %5.2f", graf[a][b]);
		printf("|\n");
	}
	printf("\nLiczba wszystkich wczytanych wierzcholkow -> %d\n", *w_top);
	printf("Liczba zadeklarowanych sciezek            -> %d\n", *w_path);
	printf("Liczba wczytanych sciezek                 -> %d\n", *w_read_path);

	int start = argc>2 ? atoi(argv[2]) : 3;   //poczatek szukanie
	int finish = argc>3 ? atoi(argv[3]) : 23;   //koniec szukania
	if (start == NULL || finish == NULL) {
		printf("Prosze wprowadzic poprawne parametry!\n");
		printf("Argument uruchomieniowe: <start> <koniec>\n");
		exit(121);
	}
	printf("\nStart: V%d \nKoniec: V%d \n", start, finish);

	//szukanie sciezki
	finder_path(graf, start, finish, w_top, k_graf);
	return 0;
}
void read_parametrs(FILE* p, int* f_top, int* f_path, double graf[][MAX_GRAF]) {
	*t = fgetc(p);
	if ((*t) != 'W')
		exit(-1);
	*t = fgetc(p);
	if ((*t) != '=')
		exit(-2);

	if (fscanf(p, "%i", f_top) != 1) {
		printf("Zly format liczby wierzcholkow\n");
		exit(-1);
	}

	*t = fgetc(p);
	if ((*t) != ';')
		exit(-3);
	*t = fgetc(p);
	if ((*t) != 'E')
		exit(-4);
	*t = fgetc(p);
	if ((*t) != '=')
		exit(-5);

	if (fscanf(p, "%i", f_path) != 1) {
		printf("Zly format liczby sciezek\n");
		exit(-6);
	}
}

void read_paths(FILE* in, double graf[][MAX_GRAF], int* w_r_p, double k_graf[][MAX_GRAF], double va[MAX_GRAF]) {
	int t1,      //zmienna dla numeru sciezki
		t2,      //zmienna dla numeru poczatkowego wektora
		t3;      //zmienna dla numeru koncowego wektora
	double waga; //zmienna dla numeru wagi 

	*t = fgetc(in);
	if ((*t) != '\n')
		exit(-7);

	while ((*t) == '\n') {

		*t = fgetc(in);
		if ((*t) != 'e')
			exit(-8);
		if (fscanf(in, "%i", &t1) != 1) {
			printf("Zly format \n");
		}
		*t = fgetc(in);
		if ((*t) != '=')
			exit(-9);
		*t = fgetc(in);
		if ((*t) != '(')
			exit(-10);
		*t = fgetc(in);
		if ((*t) != 'V')
			exit(-11);
		if (fscanf(in, "%i", &t2) != 1) {
			printf("Zly format \n");
		}
		*t = fgetc(in);
		if ((*t) != ',')
			exit(-12);
		*t = fgetc(in);
		if ((*t) != 'V')
			exit(-13);
		if (fscanf(in, "%i", &t3) != 1) {
			printf("Zly format \n");
		}
		*t = fgetc(in);
		if ((*t) != ')')
			exit(-15);
		graf[t2][t3] = ((va[t2]+va[t3])/2);
		k_graf[t2][t3] = ((va[t2] + va[t3]) / 2);
		(*w_r_p)++;
		*t = fgetc(in);
		if ((*t) != '\n')
			break;

	}
}

void finder_path(double graf[][MAX_GRAF], int a, int b, int* w_top, double k_graf[][MAX_GRAF]) {
	int liczba_wierzcholkow = *w_top;
	int start = a;
	int koniec = b;
	int sprawdzone_wierzcholki[MAX_GRAF];
	int poprzednik[MAX_GRAF];
	double koszt[MAX_GRAF];
	int liczba_rozwiazan = 0;

	for (int i = 1; i <= liczba_wierzcholkow; i++) {
		if (graf[i][koniec] != 0)
			liczba_rozwiazan++;
		sprawdzone_wierzcholki[i] = 0;
		koszt[i] = INFINITY;
	}

	int sprawdzenie = 0; //Wartosc 0 jesli niewszystkie wierzcholki sa sprawdzone, 1 jesli wszystkie sprawdzone
	int obiekt = a;
	int x = 1;
	int iftrue = 0;
	int kopiowanie = 0;
	sprawdzenie = start;
	koszt[start] = 0;
	poprzednik[start] = -1;
	while (sprawdzenie != 1) {
		x = 0;
		while (graf[obiekt][x] == 0 && x <= liczba_wierzcholkow) {
			if (x == liczba_wierzcholkow) {
				sprawdzone_wierzcholki[obiekt] = 1;
				obiekt = poprzednik[obiekt];
				iftrue = 1;
				break;
			}
			x++;
		}
		if (iftrue != 1) {
			if (koszt[x] > (koszt[obiekt] + graf[obiekt][x])) {
				if (koszt[x] != INFINITY)
					kopiowanie = 1;
				koszt[x] = koszt[obiekt] + graf[obiekt][x];
				graf[obiekt][x] = 0;
				poprzednik[x] = obiekt;
				obiekt = x;
			}
			if (koszt[x] < (koszt[obiekt] + graf[obiekt][x]) && koszt[x] != INFINITY) {
				graf[obiekt][x] = 0;
			}
			if (kopiowanie == 1) {
				for (int a = 1; a <= liczba_wierzcholkow; a++) {
					graf[obiekt][a] = k_graf[obiekt][a];
					kopiowanie = 0;
				}
			}

		}
		iftrue = 0;

		sprawdzenie = 1;
		for (int i = 1; i <= *w_top; i++) {
			if (sprawdzone_wierzcholki[i] == 0) {
				sprawdzenie = 0;
				break;
			}
		}
		if (obiekt<-1) {
			printf("Zle dane!\n");
			exit(-1001);
		}
	}
	//Wypisywanie najkrotszej sciezki
	printf("Koszt dojscia do konca: %lf \n", koszt[koniec]);
	printf("To jest najkrotsza droga do wierzcholka: \n");
	while (koniec != start) {
		printf("%d-->", koniec);
		koniec = poprzednik[koniec];
	}
	printf("%d", start);

}