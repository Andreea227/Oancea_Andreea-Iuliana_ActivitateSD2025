#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int id;
	char* denumire;
	int nrBlocuri;
	float zonaMetropolitana;
} Oras;

Oras createOras(int id, const char* denumire, int nrBlocuri, float zonaMetropolitana) {
	Oras o;
	o.id = id;
	o.denumire = (char*)malloc(strlen(denumire) + 1);
	strcpy(o.denumire, denumire);
	o.nrBlocuri = nrBlocuri;
	o.zonaMetropolitana = zonaMetropolitana;
	return o;
}

typedef struct NOD {
	Oras info;
	struct NOD* st;
	struct NOD* dr;
} NOD;

int inaltimeArbore(NOD* rad) {
	if (rad) {
		int hS = inaltimeArbore(rad->st);
		int hD = inaltimeArbore(rad->dr);

		return 1 + (hS > hD ? hS : hD);
	}
	else return 0;
}

int gradEchilibru(NOD* rad) {
	if (rad) {
		return inaltimeArbore(rad->st) - inaltimeArbore(rad->dr);
	}
	else return 0;
}

NOD* rotireLaStanga(NOD* rad) {
	if (rad) {
		NOD* aux = rad->dr;
		rad->dr = aux->st;
		aux->st = rad;
		return aux;
	}
	return rad;
}

NOD* rotireLaDreapta(NOD* rad) {
	if (rad) {
		NOD* aux = rad->st;
		rad->st = aux->dr;
		aux->dr = rad;
		return aux;
	}
	return rad;
}

NOD* inserareAVL(NOD* rad, Oras o) {
	if (rad) {
		if (o.id < rad->info.id)
			rad->st = inserareAVL(rad->st, o);
		else
			rad->dr = inserareAVL(rad->dr, o);

		int ge = gradEchilibru(rad);

		if (ge == -2) {
			if (gradEchilibru(rad->dr) != -1)
				rad->dr = rotireLaDreapta(rad->dr);
			rad = rotireLaStanga(rad);
		}
		if (ge == 2) {
			if (gradEchilibru(rad->st) != 1)
				rad->st = rotireLaStanga(rad->st);
			rad = rotireLaDreapta(rad);
		}

		return rad;
	}
	else {
		NOD* nou = (NOD*)malloc(sizeof(NOD));
		nou->info = o;
		nou->st = nou->dr = NULL;
		return nou;
	}
}

void afisareOras(Oras o) {
	printf("ID: %d | Denumire: %s | Blocuri: %d | Zona metropolitana: %.2f km^2\n",
		o.id, o.denumire, o.nrBlocuri, o.zonaMetropolitana);
}

void afisareInordine(NOD* rad) {
	if (rad) {
		afisareInordine(rad->st);
		afisareOras(rad->info);
		afisareInordine(rad->dr);
	}
}

void eliberareArbore(NOD* rad) {
	if (rad) {
		eliberareArbore(rad->st);
		eliberareArbore(rad->dr);
		free(rad->info.denumire);
		free(rad);
	}
}

void main() {
	NOD* rad = NULL;

	FILE* f = fopen("orase.txt", "r");
	if (f) {
		int n;
		fscanf(f, "%d", &n);

		for (int i = 0; i < n; i++) {
			int id, nrBlocuri;
			float zona;
			char nume[100];
			fscanf(f, "%d %s %d %f", &id, nume, &nrBlocuri, &zona);

			Oras o = createOras(id, nume, nrBlocuri, zona);
			rad = inserareAVL(rad, o);
		}
		fclose(f);
	}

	printf("\nOrase in ordine (AVL):\n");
	afisareInordine(rad);

	printf("\nInaltime arbore AVL: %d\n", inaltimeArbore(rad));

	eliberareArbore(rad);
}