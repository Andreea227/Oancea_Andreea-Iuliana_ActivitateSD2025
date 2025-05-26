#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Car {
    char* model;
    int topSpeed;
};

struct Heap {
    int dimensiune;
    int numarElemente;
    struct Car* vector;
};

struct Car initializareCar(const char* modelC, int topSpeedC) {
    struct Car c;
    c.model = malloc(sizeof(char) * (strlen(modelC) + 1));
    strcpy(c.model, modelC);
    c.topSpeed = topSpeedC;

    return c;
}

struct Heap initializareHeap(int dimensiune) {
    struct Heap h;
    h.dimensiune = dimensiune;
    h.numarElemente = 0;
    h.vector = malloc(sizeof(struct Car) * dimensiune);

    return h;
}

void afisareCar(struct Car c) {
    printf("Model: %s | Viteza maxima: %d km/h\n", c.model, c.topSpeed);
}

void afisareHeap(struct Heap h) {
    for (int i = 0; i < h.numarElemente; i++) {
        afisareCar(h.vector[i]);
    }
}

void filtrare(struct Heap h, int poz) {
    int pozS = 2 * poz + 1;
    int pozD = 2 * poz + 2;
    int pozMax = poz;

    if (pozS < h.numarElemente && h.vector[pozMax].topSpeed < h.vector[pozS].topSpeed) {
        pozMax = pozS;
    }

    if (pozD < h.numarElemente && h.vector[pozMax].topSpeed < h.vector[pozD].topSpeed) {
        pozMax = pozD;
    }


    if (pozMax != poz) {
        struct Car aux = h.vector[pozMax];
        h.vector[pozMax] = h.vector[poz];
        h.vector[poz] = aux;

        if (2 * pozMax + 1 < h.numarElemente) {
            filtrare(h, pozMax);
        }
    }
}


struct Car extrageCarMaxSpeed(struct Heap* heap) {
    struct Car aux = heap->vector[0];
    heap->vector[0] = heap->vector[heap->numarElemente - 1];
    heap->vector[heap->numarElemente - 1] = aux;
    heap->numarElemente--;

    for (int i = (heap->numarElemente - 1) / 2; i >= 0; i--) {
        filtrare(*heap, i);
    }

    return aux;
}

void main() {
    struct Heap h = initializareHeap(6);
    h.numarElemente = 6;
    h.vector[0] = initializareCar("Audi", 240);
    h.vector[1] = initializareCar("BMW", 260);
    h.vector[2] = initializareCar("Ford", 220);
    h.vector[3] = initializareCar("Toyota", 210);
    h.vector[4] = initializareCar("Honda", 230);
    h.vector[5] = initializareCar("Mazda", 200);
    
    for (int i = (h.numarElemente - 2) / 2; i >= 0; i--) {
        filtrare(h, i);
    }

    afisareHeap(h);
    printf("\n");

    struct Car c = extrageCarMaxSpeed(&h);

    afisareHeap(h);

    extrageCarMaxSpeed(&h);
    extrageCarMaxSpeed(&h);
    extrageCarMaxSpeed(&h);
    extrageCarMaxSpeed(&h);
    extrageCarMaxSpeed(&h);

    printf("\nNr elemente: %d\n", h.numarElemente);

    afisareHeap(h);

    printf("\nVector sortat:\n");

    for (int i = 0; i < h.dimensiune; i++) {
        afisareCar(h.vector[i]);
        free(h.vector[i].model);
    }

    free(h.vector);

    return;
}