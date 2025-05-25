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




Alt model……


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina masina;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* first;
	Nod* last;
	int nrNoduri;
};
typedef struct ListaDubla ListaDubla;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasiniDeLaInceput(ListaDubla lista) {
	Nod* p = lista.first;
	while (p) {
		afisareMasina(p->masina);
		p = p->next;
	}
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = NULL;
	nou->prev = lista->last;
	if (lista->last != NULL) {
		lista->last->next = nou;
	}
	else {
		lista->first = nou;
	}
	lista->last = nou;
	lista->nrNoduri++;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	lista.nrNoduri = 0;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.model != NULL && m.numeSofer != NULL)
			adaugaMasinaInLista(&lista, m);
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(ListaDubla* lista) {
	Nod* p = lista->first;
	while (p) {
		Nod* aux = p;
		p = p->next;
		free(aux->masina.model);
		free(aux->masina.numeSofer);
		free(aux);
	}
	lista->first = NULL;
	lista->last = NULL;
	lista->nrNoduri = 0;
}

float calculeazaPretMediu(ListaDubla lista) {
	if (lista.nrNoduri > 0) {
		float suma = 0;
		Nod* p = lista.first;
		while (p) {
			suma += p->masina.pret;
			p = p->next;
		}
		return suma / lista.nrNoduri;
	}
	return 0;
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	if (lista.first) {
		Nod* max = lista.first;
		Nod* p = lista.first->next;
		while (p) {
			if (p->masina.pret > max->masina.pret) {
				max = p;
			}
			p = p->next;
		}
		char* nume = (char*)malloc(strlen(max->masina.numeSofer) + 1);
		strcpy_s(nume, strlen(max->masina.numeSofer) + 1, max->masina.numeSofer);
		return nume;
	}
	return NULL;
}

ListaDubla filtreazaMasiniCuMaiMultDe3Usi(ListaDubla lista) {
	ListaDubla filtrata;
	filtrata.first = NULL;
	filtrata.last = NULL;
	filtrata.nrNoduri = 0;

	Nod* p = lista.first;
	while (p) {
		if (p->masina.nrUsi > 3) {
			Masina mNoua;
			mNoua.id = p->masina.id;
			mNoua.nrUsi = p->masina.nrUsi;
			mNoua.pret = p->masina.pret;

			mNoua.model = (char*)malloc(strlen(p->masina.model) + 1);
			strcpy_s(mNoua.model, strlen(p->masina.model) + 1, p->masina.model);

			mNoua.numeSofer = (char*)malloc(strlen(p->masina.numeSofer) + 1);
			strcpy_s(mNoua.numeSofer, strlen(p->masina.numeSofer) + 1, p->masina.numeSofer);

			mNoua.serie = p->masina.serie;

			adaugaMasinaInLista(&filtrata, mNoua);
		}
		p = p->next;
	}

	return filtrata;
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");

	printf("=== Lista initiala ===\n");
	afisareListaMasiniDeLaInceput(lista);

	printf("\n=== Masini cu mai mult de 3 usi ===\n");
	ListaDubla masiniCuUsi = filtreazaMasiniCuMaiMultDe3Usi(lista);
	afisareListaMasiniDeLaInceput(masiniCuUsi);

	printf("\nPretul mediu al masinilor este: %.2f\n", calculeazaPretMediu(lista));

	char* numeSofer = getNumeSoferMasinaScumpa(lista);
	printf("Soferul cu cea mai scumpa masina este: %s\n", numeSofer);
	free(numeSofer);

	dezalocareLDMasini(&masiniCuUsi);
	dezalocareLDMasini(&lista);
	return 0;
}