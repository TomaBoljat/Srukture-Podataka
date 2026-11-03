#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct _Poly* Pozicija;

typedef struct _Poly {
	int koeficijent;
	int eksponent;
	Pozicija next;
} Poly;

int insertPoly(Pozicija glava, int koeficijent, int eksponent) {
	Pozicija prethodni = glava;
	Pozicija trenutni = glava->next;

	while (trenutni && trenutni->eksponent > eksponent) {
		prethodni = trenutni;
		trenutni = trenutni->next;
	}

	if (trenutni && trenutni->eksponent == eksponent) {
		trenutni->koeficijent += koeficijent;
		if (trenutni->koeficijent == 0) {
			prethodni->next = trenutni->next;
			free(trenutni);
		}
		return 0;
	}

	if (koeficijent == 0) return 0;

	Pozicija noviPolinom = (Pozicija)malloc(sizeof(Poly));
	if (!noviPolinom) return -1;

	noviPolinom->koeficijent = koeficijent;
	noviPolinom->eksponent = eksponent;
	noviPolinom->next = trenutni;
	prethodni->next = noviPolinom;
	return 0;
}

void freePoly(Pozicija glava) {
	Pozicija trenutni = glava->next;
	while (trenutni) {
		Pozicija next = trenutni->next;
		free(trenutni);
		trenutni = next;
	}
	glava->next = NULL;
}

int readPoly(FILE* datoteka, Pozicija glava) {
	int brojClanaka;
	int status = fscanf(datoteka, "%d", &brojClanaka);
	if (status != 1 || brojClanaka < 0) return -1;

	for (int indeks = 0; indeks < brojClanaka; indeks++) {
		int koeficijent;
		int eksponent;
		if (fscanf(datoteka, "%d %d", &koeficijent, &eksponent) != 2 || eksponent < 0)
			return -1;
		if (insertPoly(glava, koeficijent, eksponent) != 0) return -1;
	}
	return 1;
}

int addPolys(Pozicija rezultat, Pozicija p, Pozicija q) {
	for (Pozicija clan = p->next; clan; clan = clan->next)
		if (insertPoly(rezultat, clan->koeficijent, clan->eksponent) != 0) return -1;
	for (Pozicija clan = q->next; clan; clan = clan->next)
		if (insertPoly(rezultat, clan->koeficijent, clan->eksponent) != 0) return -1;
	return 0;
}

int multiplyPolys(Pozicija rezultat, Pozicija p, Pozicija q) {
	for (Pozicija lijevi = p->next; lijevi; lijevi = lijevi->next) {
		for (Pozicija desni = q->next; desni; desni = desni->next) {
			int koeficijent = lijevi->koeficijent * desni->koeficijent;
			if (insertPoly(rezultat, koeficijent, lijevi->eksponent + desni->eksponent) != 0)
				return -1;
		}
	}
	return 0;
}

void printPoly(Pozicija glava) {
	Pozicija clan = glava->next;
	if (!clan) {
		printf("0");
		return;
	}

	while (clan) {
		if (clan != glava->next && clan->koeficijent >= 0) printf(" + ");
		else if (clan != glava->next) printf(" ");
		printf("%d*x^%d", clan->koeficijent, clan->eksponent);
		clan = clan->next;
	}
}

int main(void) {
	int odabir;
	printf("Odaberite operaciju (1 - zbrajanje, 2 - mnozenje): ");
	scanf("%d", &odabir);
	if (odabir != 1 && odabir != 2) {
		printf("Neispravan odabir operacije.\n");
		return 1;
	}

	FILE* datoteka = fopen("polinomi.txt", "r");
	if (!datoteka) {
		printf("Greska pri otvaranju datoteke.\n");
		return 1;
	}

	Poly rezultat = { 0, 0, NULL };
	int brojPolinoma = 0;
	int statusCitanja;
	while (1) {
		Poly trenutni = { 0, 0, NULL };
		statusCitanja = readPoly(datoteka, &trenutni);
		if (statusCitanja == 0) {
			freePoly(&trenutni);
			break;
		}
		if (statusCitanja < 0) {
			printf("Neispravan format datoteke.\n");
			freePoly(&trenutni);
			freePoly(&rezultat);
			fclose(datoteka);
			return 1;
		}

		if (brojPolinoma == 0) {
			rezultat.next = trenutni.next;
			trenutni.next = NULL;
		}
		else {
			Poly kombinirani = { 0, 0, NULL };
			int statusKombinacije = odabir == 1 ? addPolys(&kombinirani, &rezultat, &trenutni) : multiplyPolys(&kombinirani, &rezultat, &trenutni);
			if (statusKombinacije != 0) {
				printf("Greska pri alokaciji memorije ili prekoracenje raspona brojeva.\n");
				freePoly(&kombinirani);
				freePoly(&trenutni);
				freePoly(&rezultat);
				fclose(datoteka);
				return 1;
			}
			freePoly(&rezultat);
			rezultat.next = kombinirani.next;
			kombinirani.next = NULL;
		}

		freePoly(&trenutni);
		brojPolinoma++;
	}
	fclose(datoteka);

	if (brojPolinoma == 0) {
		printf("Datoteka ne sadrzi nijedan polinom.\n");
		freePoly(&rezultat);
		return 1;
	}

	printf("%s %d polinoma: ", odabir == 1 ? "Zbroj" : "Umnozak", brojPolinoma);
	printPoly(&rezultat);
	printf("\n");

	freePoly(&rezultat);
	return 0;
}
