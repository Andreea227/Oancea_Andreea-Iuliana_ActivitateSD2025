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
	struct Pizzerie pizzerie;
	pizzerie.nume = malloc(strlen(nume) + 1);

	if (pizzerie.nume != NULL) {
		strcpy(pizzerie.nume, nume);
	}

	pizzerie.nrComenziZilnic = nrComenziZilnic;
	pizzerie.pretMediuPizza = pretMediuPizza;

	return pizzerie;
}

void afisarePizzerie(struct Pizzerie pizzerie) {
	printf("Pizzerie: %s | Comenzi/zi: %d | Pret mediu: %.2f\n", 
		pizzerie.nume, pizzerie.nrComenziZilnic, pizzerie.pretMediuPizza);
}

void afisareLista(Nod* cap) {
	while (cap != NULL) {
		afisarePizzerie(cap->pizzerie);
		cap = cap->next;
	}
}

void pushBack(Nod** head, struct Pizzerie pizzerie) {
	Nod* nou = malloc(sizeof(Nod));

	nou->pizzerie = pizzerie;
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

Nod* pushFront(Nod* head, struct Pizzerie pizzerie) {
	Nod* nou = malloc(sizeof(Nod));

	nou->pizzerie = pizzerie;
	nou->next = head;

	return nou;
}

int main() {
	struct Pizzerie pizzerie1 = initializarePizzerie("Pizza Napoli", 120, 25.5f);
	struct Pizzerie pizzerie2 = initializarePizzerie("Pizza Roma", 95, 23.0f);

	Nod* lista = NULL;

	lista = pushFront(lista, pizzerie1);
	lista = pushFront(lista, pizzerie2);
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