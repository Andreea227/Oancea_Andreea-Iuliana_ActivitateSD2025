#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cinema Cinema;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Cinema {
	char* nume;
	int nrSali;
};

Cinema creareCinema(const char* nume, int nrSali) {
	Cinema c;
	c.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(c.nume, nume);
	c.nrSali = nrSali;

	return c;
}

struct Nod {
	Cinema info;
	Nod* next;
};

struct HashTable {
	Nod** vector;
	int dim;
};

int functieHash(int dim, const char* nume) {
	return strlen(nume) % dim;
}

HashTable creareTabela(int dim) {
	HashTable ht;
	ht.dim = dim;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dim);
	for (int i = 0; i < dim; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

void inserareInLista(Nod* cap, Cinema c) {
	Nod* temp = (Nod*)malloc(sizeof(Nod));
	temp->info = c;
	temp->next = NULL;
	while (cap->next) {
		cap = cap->next;
	}
	cap->next = temp;
}

void inserareCinemaInTabela(Cinema c, HashTable h) {
	int cod = functieHash(h.dim, c.nume);
	if (h.vector[cod] == NULL) {
		h.vector[cod] = (Nod*)malloc(sizeof(Nod));
		h.vector[cod]->info = c;
		h.vector[cod]->next = NULL;
	}
	else {
		inserareInLista(h.vector[cod], c);
	}
}

void afisareCinema(Cinema c) {
	printf("Cinema-ul %s are %d sali\n", c.nume, c.nrSali);
}

void afisareLista(Nod* cap) {
	while (cap) {
		afisareCinema(cap->info);
		cap = cap->next;
	}
}

void afisareTabela(HashTable h) {
	for (int i = 0; i < h.dim; i++) {
		printf("\nPozitia %d:\n", i);
		afisareLista(h.vector[i]);
	}
}

Cinema cautaCinemaDupaNume(HashTable h, const char* nume) {
	int poz = functieHash(h.dim, nume);
	Nod* aux = h.vector[poz];
	while (aux && strcmp(aux->info.nume, nume) != 0) {
		aux = aux->next;
	}
	if (aux) {
		return creareCinema(aux->info.nume, aux->info.nrSali);
	}
	else {
		return creareCinema("N/A", 0);
	}
}

void main() {
	HashTable ht = creareTabela(5);
	inserareCinemaInTabela(creareCinema("Cinema City", 8), ht);
	inserareCinemaInTabela(creareCinema("Movieplex", 6), ht);
	inserareCinemaInTabela(creareCinema("Gloria", 3), ht);
	inserareCinemaInTabela(creareCinema("Scala", 4), ht);
	inserareCinemaInTabela(creareCinema("Union", 5), ht);
	inserareCinemaInTabela(creareCinema("Hollywood", 7), ht);

	afisareTabela(ht);

	Cinema c = cautaCinemaDupaNume(ht, "Union");
	printf("\nCinema-ul cautat:\n");
	afisareCinema(c);

	return;
}