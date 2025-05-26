#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Film Film;
typedef struct Nod Nod;

struct Film {
	int idFilm;
	char* titlu;
	float durata; // în minute
};

struct Nod {
	Film info;
	Nod* stanga;
	Nod* dreapta;
};

Film initializareFilm(int id, const char* titlu, float durata) {
	Film f;

	f.idFilm = id;
	f.titlu = (char*)malloc(sizeof(char) * (strlen(titlu) + 1));
	strcpy(f.titlu, titlu);
	f.durata = durata;

	return f;
}

void afisareFilm(Film f) {
	printf("ID: %d || Titlu: %s || Durata: %.2f minute\n", f.idFilm, f.titlu, f.durata);
}

Nod* inserareFilmInArbore(Nod* radacina, Film f) {
	if (radacina) {
		if (radacina->info.idFilm > f.idFilm) {
			radacina->stanga = inserareFilmInArbore(radacina->stanga, f);
		}
		else {
			radacina->dreapta = inserareFilmInArbore(radacina->dreapta, f);
		}
	}
	else {
		radacina = (Nod*)malloc(sizeof(Nod));
		radacina->info = f;
		radacina->stanga = NULL;
		radacina->dreapta = NULL;
	}

	return radacina;
}

void afisareInOrdine(Nod* radacina) {
	if (radacina) {
		afisareInOrdine(radacina->stanga);
		afisareFilm(radacina->info);
		afisareInOrdine(radacina->dreapta);
	}
}

void afisarePreOrdine(Nod* radacina) {
	if (radacina) {
		afisareFilm(radacina->info);
		afisarePreOrdine(radacina->stanga);
		afisarePreOrdine(radacina->dreapta);
	}
}

Film cautareFilmDupaId(Nod* radacina, int id) {
	if (radacina) {
		if (radacina->info.idFilm > id)
			return cautareFilmDupaId(radacina->stanga, id);
		else if (radacina->info.idFilm < id)
			return cautareFilmDupaId(radacina->dreapta, id);
		else
			return initializareFilm(radacina->info.idFilm, radacina->info.titlu, radacina->info.durata);
	}
	else {
		return initializareFilm(0, "N/A", 0);
	}
}

float calculDurataTotala(Nod* radacina) {
	if (radacina) {
		return calculDurataTotala(radacina->stanga) +
			radacina->info.durata +
			calculDurataTotala(radacina->dreapta);
	}
	else {
		return 0;
	}
}

int calculInaltime(Nod* radacina) {
	if (radacina) {
		int inaltimeS = calculInaltime(radacina->stanga);
		int inaltimeD = calculInaltime(radacina->dreapta);
		return 1 + (inaltimeS > inaltimeD ? inaltimeS : inaltimeD);
	}
	else {
		return 0;
	}
}

void main() {
	Nod* radacina = NULL;

	radacina = inserareFilmInArbore(radacina, initializareFilm(10, "Interstellar", 169));
	radacina = inserareFilmInArbore(radacina, initializareFilm(5, "Inception", 148));
	radacina = inserareFilmInArbore(radacina, initializareFilm(15, "Dune", 155));
	radacina = inserareFilmInArbore(radacina, initializareFilm(3, "The Prestige", 130));
	radacina = inserareFilmInArbore(radacina, initializareFilm(20, "Tenet", 150));

	printf("Afisare in ordine:\n");
	afisareInOrdine(radacina);

	printf("\nFilm cautat:\n");
	Film f = cautareFilmDupaId(radacina, 5);
	afisareFilm(f);

	float durataTotala = calculDurataTotala(radacina);
	printf("\nDurata totala a filmelor: %.2f minute\n", durataTotala);

	int inaltime = calculInaltime(radacina);
	printf("Inaltime arbore: %d\n", inaltime);

	return;
}