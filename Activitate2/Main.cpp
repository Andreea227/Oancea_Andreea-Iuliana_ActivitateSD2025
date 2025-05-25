#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraTelefon {
	int id;
	int memorieGB;
	float pret;
	char* model;
	char* numeClient;
	unsigned char codGarantie;
};
typedef struct StructuraTelefon Telefon;

Telefon citireTelefonDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Telefon t1;
	aux = strtok(buffer, sep);
	t1.id = atoi(aux);
	t1.memorieGB = atoi(strtok(NULL, sep));
	t1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	t1.model = malloc(strlen(aux) + 1);
	strcpy(t1.model, aux);
	aux = strtok(NULL, sep);
	t1.numeClient = malloc(strlen(aux) + 1);
	strcpy(t1.numeClient, aux);
	t1.codGarantie = *strtok(NULL, sep);

	return t1;
}

struct Heap {
	int lungime;
	Telefon* vector;
	int nrTelefoane;
};
typedef struct Heap Heap;

void afisareTelefon(Telefon tel) {
	printf("ID: %d\n", tel.id);
	printf("Memorie: %d GB\n", tel.memorieGB);
	printf("Pret: %.2f lei\n", tel.pret);
	printf("Model: %s\n", tel.model);
	printf("Client: %s\n", tel.numeClient);
	printf("Garantie: %c\n\n", tel.codGarantie);
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrTelefoane = 0;
	heap.vector = (Telefon*)malloc(sizeof(Telefon) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;

	if (pozFiuSt < heap.nrTelefoane && heap.vector[pozMax].pret < heap.vector[pozFiuSt].pret) {
		pozMax = pozFiuSt;
	}
	if (pozFiuDr < heap.nrTelefoane && heap.vector[pozMax].pret < heap.vector[pozFiuDr].pret) {
		pozMax = pozFiuDr;
	}
	if (pozMax != pozitieNod) {
		Telefon aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		if (pozMax <= (heap.nrTelefoane - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDeTelefoaneDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);

	while (!feof(file)) {
		heap.vector[heap.nrTelefoane++] = citireTelefonDinFisier(file);
	}
	fclose(file);

	for (int i = (heap.nrTelefoane - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrTelefoane; i++) {
		afisareTelefon(heap.vector[i]);
	}
}

void afisareHeapAscuns(Heap heap) {
	for (int i = heap.nrTelefoane; i < heap.lungime; i++) {
		afisareTelefon(heap.vector[i]);
	}
}

Telefon extrageTelefon(Heap* heap) {
	if (heap->nrTelefoane > 0) {
		Telefon aux = heap->vector[heap->nrTelefoane - 1];
		heap->nrTelefoane--;

		for (int i = (heap->nrTelefoane - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);
		}
		return aux;
	}
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].model);
		free(heap->vector[i].numeClient);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrTelefoane = 0;
}

int main() {
	Heap heap = citireHeapDeTelefoaneDinFisier("telefoane.txt");
	afisareHeap(heap);

	printf("Telefoane extrase:\n\n");
	for (int i = 0; i < 10; i++) {
		afisareTelefon(extrageTelefon(&heap));
	}

	printf("Heap-ul ascuns:\n\n");
	afisareHeapAscuns(heap);

	dezalocareHeap(&heap);
	return 0;
}