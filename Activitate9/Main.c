#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pizzerie Pizzerie;
typedef struct Nod Nod;

struct Pizzerie {
	char* nume;
	int nrComenziZilnic;
	float pretMediuPizza;
};

struct Nod {
	struct Pizzerie pizzerie;
	struct Nod* next;
};

struct Pizzerie initializarePizzerie(const char* nume, int nrComenziZilnic, float pretMediuPizza) {
	struct Pizzerie p;
	p.nume = malloc(strlen(nume) + 1);
	strcpy(p.nume, nume);
	p.nrComenziZilnic = nrComenziZilnic;
	p.pretMediuPizza = pretMediuPizza;
	return p;
}

void afisarePizzerie(struct Pizzerie p) {
	printf("Pizzerie: %s | Comenzi/zi: %d | Pret mediu: %.2f\n", p.nume, p.nrComenziZilnic, p.pretMediuPizza);
}

void afisareLista(Nod* cap) {
	while (cap != NULL) {
		afisarePizzerie(cap->pizzerie);
		cap = cap->next;
	}
}

void pushBack(Nod** head, struct Pizzerie p) {
	Nod* nou = malloc(sizeof(Nod));
	nou->pizzerie = p;
	nou->next = NULL;
	if (*head == NULL) {
		*head = nou;
	}
	else {
		Nod* temp = *head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

Nod* pushFront(Nod* head, struct Pizzerie p) {
	Nod* nou = malloc(sizeof(Nod));
	nou->pizzerie = p;
	nou->next = head;
	return nou;
}

int main() {
	struct Pizzerie p1 = initializarePizzerie("Pizza Napoli", 120, 25.5f);
	struct Pizzerie p2 = initializarePizzerie("Pizza Roma", 95, 23.0f);

	Nod* lista = NULL;
	lista = pushFront(lista, p1);
	lista = pushFront(lista, p2);
	pushBack(&lista, initializarePizzerie("Pizza Milano", 80, 27.3f));

	afisareLista(lista);

	// Eliberare memorie
	while (lista != NULL) {
		free(lista->pizzerie.nume);
		Nod* tmp = lista;
		lista = lista->next;
		free(tmp);
	}

	return 0;
}