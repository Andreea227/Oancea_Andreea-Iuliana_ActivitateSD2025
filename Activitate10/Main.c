#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Biblioteca {
	int cod;
	char* nume;
	unsigned int nrCarti;
	float* preturi;
	float suprafata;
};

struct Biblioteca initializare(int cod, const char* nume, unsigned int nrCarti, const float* preturi, float suprafata) {
	struct Biblioteca b;
	b.cod = cod;
	b.nrCarti = nrCarti;
	b.suprafata = suprafata;
	b.nume = malloc((strlen(nume) + 1) * sizeof(char));
	strcpy(b.nume, nume);
	b.preturi = malloc(sizeof(float) * nrCarti);
	for (int i = 0; i < nrCarti; i++) {
		b.preturi[i] = preturi[i];
	}
	return b;
}

void afisare(struct Biblioteca b) {
	printf("Cod: %d. Suprafata: %.2f mp. Nume: %s\n", b.cod, b.suprafata, b.nume);
	printf("Preturi carti: ");
	for (int i = 0; i < b.nrCarti; i++) {
		printf("%.2f; ", b.preturi[i]);
	}
	printf("\n");
}

struct Biblioteca copiereBiblioteca(struct Biblioteca b) {
	struct Biblioteca copie;
	copie.cod = b.cod;
	copie.nrCarti = b.nrCarti;
	copie.suprafata = b.suprafata;
	copie.nume = malloc((strlen(b.nume) + 1) * sizeof(char));
	strcpy(copie.nume, b.nume);
	copie.preturi = malloc(sizeof(float) * b.nrCarti);
	for (int i = 0; i < b.nrCarti; i++) {
		copie.preturi[i] = b.preturi[i];
	}
	return copie;
}

void push_biblioteca(struct Biblioteca b, struct Biblioteca** biblioteci, int* nrBiblioteci) {
	(*nrBiblioteci)++;
	struct Biblioteca* nou = malloc(sizeof(struct Biblioteca) * (*nrBiblioteci));

	for (int i = 0; i < (*nrBiblioteci) - 1; i++) {
		nou[i] = (*biblioteci)[i]; // shallow copy
	}
	nou[(*nrBiblioteci) - 1] = copiereBiblioteca(b);

	free(*biblioteci);
	*biblioteci = nou;
}

int totalCarti(struct Biblioteca* biblioteci, int nrBiblioteci) {
	int total = 0;
	for (int i = 0; i < nrBiblioteci; i++) {
		total += biblioteci[i].nrCarti;
	}
	return total;
}

int main() {
	float preturi1[] = { 50.5f, 35.0f, 60.3f };
	struct Biblioteca b1 = initializare(1, "Biblioteca Nationala", 3, preturi1, 800.0f);
	afisare(b1);

	struct Biblioteca* biblioteci = malloc(sizeof(struct Biblioteca));
	int nrBiblioteci = 1;
	biblioteci[0] = copiereBiblioteca(b1);

	float preturi2[] = { 40.0f, 42.5f };
	struct Biblioteca b2 = initializare(2, "Biblioteca Judeteana", 2, preturi2, 550.0f);
	struct Biblioteca b3 = initializare(3, "Biblioteca Oraseneasca", 2, preturi2, 600.0f);

	push_biblioteca(b2, &biblioteci, &nrBiblioteci);
	push_biblioteca(b3, &biblioteci, &nrBiblioteci);

	for (int i = 0; i < nrBiblioteci; i++) {
		afisare(biblioteci[i]);
	}

	printf("Numarul total de carti din toate bibliotecile: %d\n", totalCarti(biblioteci, nrBiblioteci));

	for (int i = 0; i < nrBiblioteci; i++) {
		free(biblioteci[i].nume);
		free(biblioteci[i].preturi);
	}
	free(biblioteci);
	free(b1.nume);
	free(b1.preturi);

	return 0;
}