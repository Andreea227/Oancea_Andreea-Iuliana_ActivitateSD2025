#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* nume;
	int nrSali;
	float incasariLunare;
} Cinema;

Cinema citireCinemaDinFisier(FILE* f) {
	char buffer[100], * token;
	char sep[2] = ",\n";
	Cinema c;

	if (fgets(buffer, 100, f)) {
		token = strtok(buffer, sep);
		c.nume = (char*)malloc(strlen(token) + 1);
		strcpy(c.nume, token);

		c.nrSali = atoi(strtok(NULL, sep));
		c.incasariLunare = atof(strtok(NULL, sep));
	}
	else {
		c.nume = NULL;
		c.nrSali = 0;
		c.incasariLunare = 0;
	}

	return c;
}

void afisareCinema(Cinema c) {
	printf("Cinema: %s | Sali: %d | Incasari: %.2f\n", c.nume, c.nrSali, c.incasariLunare);
}

void dezalocareCinema(Cinema c) {
	free(c.nume);
}


typedef struct NodStiva {
	Cinema info;
	struct NodStiva* next;
} NodStiva;

void pushStack(NodStiva** varf, Cinema c) {
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->info = c;
	nou->next = *varf;
	*varf = nou;
}

Cinema popStack(NodStiva** varf) {
	Cinema c;
	if (*varf) {
		NodStiva* temp = *varf;
		c = temp->info;
		*varf = temp->next;
		free(temp);
	}
	else {
		c.nume = NULL;
		c.nrSali = 0;
		c.incasariLunare = 0;
	}
	return c;
}

int emptyStack(NodStiva* varf) {
	return varf == NULL;
}

NodStiva* citireStackDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) return NULL;

	NodStiva* varf = NULL;
	while (!feof(f)) {
		Cinema c = citireCinemaDinFisier(f);
		if (c.nume) pushStack(&varf, c);
	}

	fclose(f);
	return varf;
}

void dezalocareStiva(NodStiva** varf) {
	while (!emptyStack(*varf)) {
		Cinema c = popStack(varf);
		dezalocareCinema(c);
	}
}

typedef struct NodCoada {
	Cinema info;
	struct NodCoada* next;
	struct NodCoada* prev;
} NodCoada;

typedef struct {
	NodCoada* prim;
	NodCoada* ultim;
} Coada;

void enqueue(Coada* c, Cinema cinema) {
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->info = cinema;
	nou->next = NULL;
	nou->prev = c->ultim;
	if (c->ultim)
		c->ultim->next = nou;
	else
		c->prim = nou;
	c->ultim = nou;
}

Cinema dequeue(Coada* c) {
	Cinema cinema;
	if (c->prim) {
		NodCoada* temp = c->prim;
		cinema = temp->info;
		c->prim = temp->next;
		if (c->prim)
			c->prim->prev = NULL;
		else
			c->ultim = NULL;
		free(temp);
	}
	else {
		cinema.nume = NULL;
		cinema.nrSali = 0;
		cinema.incasariLunare = 0;
	}
	return cinema;
}

int emptyQueue(Coada* c) {
	return c->prim == NULL;
}

Coada citireCoadaDinFisier(const char* numeFisier) {
	Coada coada = { NULL, NULL };
	FILE* f = fopen(numeFisier, "r");
	if (!f) return coada;

	while (!feof(f)) {
		Cinema c = citireCinemaDinFisier(f);
		if (c.nume) enqueue(&coada, c);
	}

	fclose(f);
	return coada;
}

void dezalocareCoada(Coada* c) {
	while (!emptyQueue(c)) {
		Cinema cinema = dequeue(c);
		dezalocareCinema(cinema);
	}
}

// -------------------- FILTRARE CINEMATOGRAFE MARI ---------------------

Coada filtrareCinematografeMari(Coada original) {
	Coada rezultat = { NULL, NULL };
	Coada temp = { NULL, NULL };

	while (!emptyQueue(&original)) {
		Cinema c = dequeue(&original);
		if (c.incasariLunare > 20000) {
			enqueue(&rezultat, c);
		}
		else {
			enqueue(&temp, c);
		}
	}

	// refacem coada original?
	while (!emptyQueue(&temp)) {
		enqueue(&original, dequeue(&temp));
	}

	return rezultat;
}

// -------------------- MAIN ---------------------

int main() {
	printf("Citire din fisier in STACK:\n");
	NodStiva* stiva = citireStackDinFisier("Cinema.txt");
	while (!emptyStack(stiva)) {
		Cinema c = popStack(&stiva);
		afisareCinema(c);
		dezalocareCinema(c);
	}

	printf("\nCitire din fisier in QUEUE:\n");
	Coada coada = citireCoadaDinFisier("Cinema.txt");

	// Filtrare
	Coada mari = filtrareCinematografeMari(coada);

	printf("\nCinematografe cu incasari peste 20000:\n");
	while (!emptyQueue(&mari)) {
		Cinema c = dequeue(&mari);
		afisareCinema(c);
		dezalocareCinema(c);
	}

	// Dezalocare coada original?
	dezalocareCoada(&coada);

	return 0;
}