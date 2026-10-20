#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Person* Position;

typedef struct _Person {
    char ime[50];
    char prezime[50];
    int godina;

    Position next;
}Person;

Position createPerson(const char* ime, const char* prezime, int godina) {
    Position newP = (Position)malloc(sizeof(Person));
    if (!newP) return NULL;
    strcpy(newP->ime, ime);
    strcpy(newP->prezime, prezime);
    newP->godina = godina;
    newP->next = NULL;
    return newP;
}

int insertAfter(Position p, Position q) {
    
    q->next = p->next;
    p->next = q;
    return 0;
}

int insertBegin(Position head, const char* ime, const char* prezime, int godina) {
    Position newP = createPerson(ime, prezime, godina);
    if (!newP) return -1;
    return insertAfter(head, newP);
}

int insertEnd(Position head, const char* ime, const char* prezime, int godina) {
    if (!head) return -1;
    Position p = head;
    while (p->next != NULL) p = p->next;
    Position newP = createPerson(ime, prezime, godina);
    if (!newP) return -1;
    return insertAfter(p, newP);
}

void printList(Position head) {
    if (!head || !head->next) {
        printf("Lista prazna.\n");
        return;
    }
    Position q = head->next;
    while (q) {
        printf("%s %s %d\n", q->ime, q->prezime, q->godina);
        q = q->next;
    }
}

Position findBySurname(Position head, const char* prezime) {
    if (!head) return NULL;
    Position q = head->next;
    while (q) {
        if (strcmp(q->prezime, prezime) == 0) return q;
        q = q->next;
    }
    return NULL;
}

int deleteBySurname(Position p, const char* prezime) {
    if (!p) return -1;
    Position prev = p;
    Position curr = p->next;
    while (curr && strcmp(curr->prezime, prezime) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (!curr) {
        printf("Osoba nije pronadena.\n");
        return 0;
    }


    prev->next = curr->next;
    free(curr);
    return 1;
}

void freeList(Position head) {
    if (!head) return;
    Position curr = head->next;
    while (curr) {
        Position temp = curr;
        curr = curr->next;
        free(temp);
    }
    head->next = NULL;
}

int addPerson(Position head) {
    char ime[50], prezime[50];
    int god;
    char choice;
    printf("Unesite ime prezime godina (npr. Ana Horvat 1990):\n");
    if (scanf("%s %s %d", ime, prezime, &god) != 3) {
        printf("Neispravan unos.\n");
        return -1;
    }
    printf("Unesite 'p' za unos na pocetak ili 'k' za kraj liste: ");
    scanf(" %c", &choice);
    if (choice == 'p' || choice == 'P')
        return insertBegin(head, ime, prezime, god);
    else
        return insertEnd(head, ime, prezime, god);
}

int main(void) {
    Person Head = { .ime = {0}, .prezime = {0}, .godina = 0, .next = NULL };
    Position head = &Head;

    int n=0;
    printf("Koliko osoba zelite unijeti? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Neispravan unos.\n");
        return -1;
    }

    for (int i = 0; i < n; i++) {
        if (addPerson(head) != 0) {
            printf("Greska pri dodavanju osobe.\n");
        }
    }

    printf("\nTrenutna lista:\n");
    printList(head);

    char target[50];
    printf("\nUnesi prezime za trazenje: ");
    scanf("%s", target);
    Position found = findBySurname(head, target);
    if (found) printf("Pronaden: %s %s %d\n", found->ime, found->prezime, found->godina);
    else printf("Osoba s prezimenom %s nije pronadena.\n", target);

    printf("\nUnesi prezime osobe za brisanje: ");
    scanf("%s", target);
    int deleted = deleteBySurname(head, target);
    if (deleted == 1) { 
        printf("Osoba izbrisana.\n");
    }
    else if (deleted == 0) { 
        printf("Osoba nije pronadena.\n"); 
    }
    else
        printf("Greska pri brisanju.\n");

    printf("\nLista nakon brisanja:\n");
    printList(head);

    freeList(head);
    return 0;
}