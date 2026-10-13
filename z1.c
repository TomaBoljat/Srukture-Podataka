#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_BODOVA 50

typedef struct Student
{
    char ime[50];
    char prezime[50];
    int bodovi;
} stud;

int main()
{
    int brojac = 0;
    double relativni_bodovi = 0;
    FILE *fp;
    
    fp = fopen("popis.txt", "r");
    if (!fp)
    {
        printf("Greska pri otvaranju datoteke.\n");
        return -1;
    }

    // 1. Sigurnije brojanje studenata preko povratne vrijednosti fscanf
    char tempIme[50], tempPrez[50];
    int tempBodovi;
    while (fscanf(fp, "%s %s %d", tempIme, tempPrez, &tempBodovi) == 3)
    {
        brojac++;
    }

    if (brojac == 0)
    {
        printf("Datoteka je prazna ili neispravno formatirana\n");
        fclose(fp);
        return 0;
    }

    rewind(fp); // Vraćamo pokazivač na početak datoteke

    // 2. Alokacija s provjerom
    stud *studenti = (stud *)malloc(brojac * sizeof(stud));
    if (studenti == NULL)
    {
        printf("Greska pri alokaciji memorije.\n");
        fclose(fp);
        return -1;
    }

    // 3. Učitavanje u alocirani niz
    for (int i = 0; i < brojac; i++)
    {
        fscanf(fp, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);
    }

    // 4. Ispis
    for (int i = 0; i < brojac; i++)
    {
        relativni_bodovi = (double)studenti[i].bodovi / MAX_BODOVA * 100;
        printf("%s %s %d (apsolutni bodovi) %.2f%% (relativni bodovi)\n", 
               studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, relativni_bodovi);
    }

    free(studenti);
    fclose(fp);
    return 0;
}