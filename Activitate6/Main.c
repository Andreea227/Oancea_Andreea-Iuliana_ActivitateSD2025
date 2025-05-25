#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraCarte {
	int id;
	int nrPagini;
	float pret;
	char* titlu;
	char* autor;
	unsigned char categorie;
};
typedef struct StructuraCarte Carte;

struct Nod {
	Carte info;
	struct Nod* next;
};
typedef struct Nod Nod;

Carte citireCarteDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Carte c;
	aux = strtok(buffer, sep);
	c.id = atoi(aux);
	c.nrPagini = atoi(strtok(NULL, sep));
	c.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	c.titlu = malloc(strlen(aux) + 1);
	strcpy(c.titlu, aux);

	aux = strtok(NULL, sep);
	c.autor = malloc(strlen(aux) + 1);
	strcpy(c.autor, aux);

	c.categorie = *strtok(NULL, sep);
	return c;
}

void afisareCarte(Carte c) {
	printf("ID: %d\n", c.id);
	printf("Pagini: %d\n", c.nrPagini);
	printf("Pret: %.2f\n", c.pret);
	printf("Titlu: %s\n", c.titlu);
	printf("Autor: %s\n", c.autor);
	printf("Categorie: %c\n\n", c.categorie);
}

void afisareListaCarti(Nod* cap) {
	while (cap != NULL) {
		afisareCarte(cap->info);
		cap = cap->next;
	}
}

void adaugaCarteInLista(Nod** cap, Carte carteNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = carteNoua;
	nou->next = NULL;
	if (*cap) {
		Nod* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

void adaugaLaInceputInLista(Nod** cap, Carte carteNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = carteNoua;
	nou->next = *cap;
	*cap = nou;
}

Nod* citireListaCartiDinFisier(const char* numeFisier) {
	Nod* cap = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			adaugaCarteInLista(&cap, citireCarteDinFisier(f));
		}
		fclose(f);
	}
	return cap;
}

void dezalocareListaCarti(Nod** cap) {
	while (*cap) {
		Nod* p = *cap;
		*cap = p->next;
		free(p->info.titlu);
		free(p->info.autor);
		free(p);
	}
}

float calculeazaPretMediu(Nod* cap) {
	float suma = 0;
	int contor = 0;
	while (cap) {
		suma += cap->info.pret;
		contor++;
		cap = cap->next;
	}
	return (contor > 0) ? suma / contor : 0;
}

float calculeazaPretCartiAutor(Nod* cap, const char* autorCautat) {
	float suma = 0;
	while (cap) {
		if (strcmp(cap->info.autor, autorCautat) == 0) {
			suma += cap->info.pret;
		}
		cap = cap->next;
	}
	return suma;
}

void stergeCartiDupaCategorie(Nod** cap, char categorieCautata) {
	while (*cap && (*cap)->info.categorie == categorieCautata) {
		Nod* aux = *cap;
		*cap = aux->next;
		free(aux->info.titlu);
		free(aux->info.autor);
		free(aux);
	}

	if (*cap) {
		Nod* p = *cap;
		while (p && p->next) {
			if (p->next->info.categorie == categorieCautata) {
				Nod* aux = p->next;
				p->next = aux->next;
				free(aux->info.titlu);
				free(aux->info.autor);
				free(aux);
			}
			else {
				p = p->next;
			}
		}
	}
}

int main() {
	Nod* cap = citireListaCartiDinFisier("carti.txt");
	afisareListaCarti(cap);

	printf("Pretul mediu: %.2f\n", calculeazaPretMediu(cap));
	printf("Pret total carti autor 'Popescu': %.2f\n", calculeazaPretCartiAutor(cap, "Popescu"));

	printf("\nStergere carti categorie A:\n");
	stergeCartiDupaCategorie(&cap, 'A');
	afisareListaCarti(cap);

	printf("\nStergere carti categorie F:\n");
	stergeCartiDupaCategorie(&cap, 'F');
	afisareListaCarti(cap);

	dezalocareListaCarti(&cap);
	return 0;
}