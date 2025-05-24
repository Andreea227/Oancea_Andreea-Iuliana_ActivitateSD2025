#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>

struct carte {
	int id;
	char* titlu;
	int nr_pagini;
	float pret;
};

typedef struct carte carte;

carte citesteCarte(FILE* f) {
	carte c;
	char buffer[100];
	fscanf(f, "%d", &c.id);
	fscanf(f, "%s", buffer);
	c.titlu = (char*)malloc(strlen(buffer) + 1);
	strcpy(c.titlu, buffer);
	fscanf(f, "%d", &c.nr_pagini);
	fscanf(f, "%f", &c.pret);
	return c;
}

void afisareCarte(carte c) {
	printf("%d. %s - %d pagini - %.2f lei\n", c.id, c.titlu, c.nr_pagini, c.pret);
}

struct NOD {
	carte info;
	struct NOD* st;
	struct NOD* dr;
};

typedef struct NOD NOD;

NOD* initNod(carte c, NOD* st, NOD* dr) {
	NOD* nou = (NOD*)malloc(sizeof(NOD));
	nou->info = c;
	nou->st = st;
	nou->dr = dr;
	return nou;
}

NOD* inserare(NOD* rad, carte c) {
	if (rad) {
		if (rad->info.id > c.id) {
			rad->st = inserare(rad->st, c);
		}
		else {
			rad->dr = inserare(rad->dr, c);
		}
		return rad;
	}
	else {
		return initNod(c, NULL, NULL);
	}
}

void afisareArbore(NOD* rad) {
	if (rad) {
		afisareArbore(rad->st);
		afisareCarte(rad->info);
		afisareArbore(rad->dr);
	}
}

carte cautareCarte(NOD* rad, int id) {
	if (rad) {
		if (rad->info.id == id) {
			return rad->info;
		}
		else if (rad->info.id < id) {
			return cautareCarte(rad->dr, id);
		}
		else {
			return cautareCarte(rad->st, id);
		}
	}
	else {
		carte c;
		c.id = -1;
		c.titlu = NULL;
		c.nr_pagini = 0;
		c.pret = 0;
		return c;
	}
}

int inaltimeArbore(NOD* rad) {
	if (rad) {
		int hSt = inaltimeArbore(rad->st);
		int hDr = inaltimeArbore(rad->dr);
		return 1 + (hSt > hDr ? hSt : hDr);
	}
	else {
		return 0;
	}
}

void afisareDePeNivel(NOD* rad, int nivelCautat, int nivelCurent) {
	if (rad) {
		if (nivelCautat == nivelCurent) {
			afisareCarte(rad->info);
		}
		else {
			afisareDePeNivel(rad->st, nivelCautat, nivelCurent + 1);
			afisareDePeNivel(rad->dr, nivelCautat, nivelCurent + 1);
		}
	}
}

void main() {
	NOD* rad = NULL;
	FILE* f = fopen("carti.txt", "r");
	int nrCarti = 0;
	fscanf(f, "%d", &nrCarti);
	for (int i = 0; i < nrCarti; i++) {
		rad = inserare(rad, citesteCarte(f));
	}
	fclose(f);

	afisareArbore(rad);
	printf("\nCarte cautata:\n");
	afisareCarte(cautareCarte(rad, 3));
	printf("\nInaltime arbore: %d\n", inaltimeArbore(rad));
	printf("\nCarti de pe nivelul 2:\n");
	afisareDePeNivel(rad, 2, 1);
}