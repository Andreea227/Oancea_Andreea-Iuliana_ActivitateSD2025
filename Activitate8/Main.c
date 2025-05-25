#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Librarie {
	char* nume;
	int nrTipuriCarti;
	float* preturi;
	int* nrExemplare;
};

struct Librarie creareLibrarie(char* nume, int nrTipuriCarti, float* preturi, int* nrExemplare) {
	struct Librarie librarie;

	librarie.nume = calloc(strlen(nume) + 1, sizeof(char));
	if (librarie.nume != NULL) {
		strcpy(librarie.nume, nume);
	}
	librarie.nrTipuriCarti = nrTipuriCarti;
	librarie.preturi = calloc(nrTipuriCarti, sizeof(float));
	librarie.nrExemplare = calloc(nrTipuriCarti, sizeof(int));

	for (int i = 0; i < nrTipuriCarti; i++) {
		librarie.preturi[i] = preturi[i];
		librarie.nrExemplare[i] = nrExemplare[i];
	}

	return librarie;
}

void afisareLibrarie(struct Librarie l) {
	printf("\nNumele librariei: %s", l.nume);
	printf("\nNumar tipuri carti: %d", l.nrTipuriCarti);
	printf("\nPreturi si stocuri:\n");

	for (int i = 0; i < l.nrTipuriCarti; i++) {
		printf("Carte %d - %.2f lei, %d exemplare\n", i + 1, l.preturi[i], l.nrExemplare[i]);
	}
}

void editareNume(struct Librarie* librarie, char* numeNou) {
	free(librarie->nume);
	librarie->nume = calloc(strlen(numeNou) + 1, sizeof(char));
	if (librarie->nume != NULL) {
		strcpy(librarie->nume, numeNou);
	}
}

void main() {
	float preturi[] = { 45.5, 29.9 };
	int exemplare[] = { 10, 20 };

	struct Librarie l = creareLibrarie("Carturesti", 2, preturi, exemplare);
	afisareLibrarie(l);

	editareNume(&l, "Humanitas");
	afisareLibrarie(l);

	// Eliberare memorie
	free(l.nume);
	free(l.preturi);
	free(l.nrExemplare);
}