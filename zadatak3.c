#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 30

typedef struct _Person {
	char name[MAX];
	char lastName[MAX];
	int yearOfBirth;
	struct _Person* next;
} Person;

int insertBegin(char*, char*, int, Person*);
int printList(Person*);
int insertEnd(char*, char*, int, Person*);
Person* findByLastName(char*, Person*);
int deletePerson(char*, Person*);
int deleteAll(Person*);
int insertAfter(char*, char*, char*, int, Person*);
int insertBefore(char*, char*, char*, int, Person*);
int writeListToFile(char*, Person*);
int readListFromFile(char*, Person*);
int sortList(Person*);

int main(void) {

	Person head = { 0 };

	int choice = -1;

	while (choice) {
		printf("\nIZBORNIK\n");
		printf("1. Dodaj na pocetak\n");
		printf("2. Dodaj na kraj\n");
		printf("3. Ispisi listu\n");
		printf("4. Pronadji po prezimenu\n");
		printf("5. Obrisi po prezimenu\n");
		printf("6. Dodaj nakon osobe\n");
		printf("7. Dodaj ispred osobe\n");
		printf("8. Upis liste u datoteku\n");
		printf("9. Citanje liste iz datoteke\n");
		printf("10. Sortiraj listu\n");
		printf("0. Izlaz\n");
		printf("Odaberite opciju: ");
		scanf("%d", &choice);

		switch (choice)
		{
			case 1: {
				Person newP;
				printf("Unesite ime, prezime i godinu rodjenja: ");
				scanf("%s %s %d", newP.name, newP.lastName, &newP.yearOfBirth);
				insertBegin(newP.name, newP.lastName, newP.yearOfBirth, &head);
				break;
			}

			case 2: {
				Person newP;
				printf("Unesite ime, prezime i godinu rodjenja: ");
				scanf("%s %s %d", newP.name, newP.lastName, &newP.yearOfBirth);
				insertEnd(newP.name, newP.lastName, newP.yearOfBirth, &head);
				break;
			}

			case 3: {
				printList(&head);
				break;
			}

			case 4: {
				char _lastName[MAX];
				printf("Unesite prezime: ");
				scanf("%s", _lastName);

				Person* newP = findByLastName(_lastName, &head);

				if (newP) {
					printf("Pronadjena osoba %s %s %d!\n", newP->name, newP->lastName, newP->yearOfBirth);
				}
				else {
					printf("Osoba s prezimenom %s nije pronadjena!\n", _lastName);
				}

				break;
			}

			case 5: {
				char _lastName[MAX];
				printf("Unesite prezime: ");
				scanf("%s", _lastName);

				if (deletePerson(_lastName, &head) == 0) {
					puts("Osoba je uspjesno obrisana!");
				}

				break;
			}

			case 6: {
				Person newP;
				char _targetLastName[MAX];

				printf("Unesite prezime osobe nakon koje zelite unijeti novu osobu: ");
				scanf("%s", _targetLastName);

				printf("Unesite ime, prezime i godinu rodjenja nove osobe: ");
				scanf("%s %s %d", newP.name, newP.lastName, &newP.yearOfBirth);

				insertAfter(_targetLastName, newP.name, newP.lastName, newP.yearOfBirth, &head);

				break;
			}

			case 7: {
				Person newP;
				char _targetLastName[MAX];

				printf("Unesite prezime osobe ispred koje zelite unijeti novu osobu: ");
				scanf("%s", _targetLastName);

				printf("Unesite ime, prezime i godinu rodjenja nove osobe: ");
				scanf("%s %s %d", newP.name, newP.lastName, &newP.yearOfBirth);

				insertBefore(_targetLastName, newP.name, newP.lastName, newP.yearOfBirth, &head);

				break;
			}

			case 8: {
				char fileName[MAX];

				printf("Unesite ime datoteke: ");
				scanf("%s", fileName);

				if (writeListToFile(fileName, &head) == 0) puts("Upis liste u datoteku uspjesno je izvrsen!");
				break;
			}

			case 9: {
				char fileName[MAX];

				printf("Unesite ime datoteke: ");
				scanf("%s", fileName);

				if (readListFromFile(fileName, &head) == 0) puts("Citanje liste iz datoteke uspjesno je izvrseno!");
				break;
			}
			
			case 10: {
				if (sortList(&head) == 0) puts("Lista je uspjesno sortirana");
				break;
			}

			case 0: {
				puts("Izlaz!");
				deleteAll(&head);
				break;
			}

			default: {
				puts("Krivi izbor, pokusajte ponovno!");
				break;
			}
		}
	}

	deleteAll(&head);

	return 0;
}

int insertBegin(char* _name, char* _lastName, int _yearOfBirth, Person* head) {
	Person* newP = (Person*)malloc(sizeof(Person));

	if (!newP) {
		puts("Greska!");
		return -1;
	}

	strcpy(newP->name, _name);
	strcpy(newP->lastName, _lastName);
	newP->yearOfBirth = _yearOfBirth;

	newP->next = head->next;
	head->next = newP;

	return 0;
}

int printList(Person* head) {
	Person* curr = head->next;

	if (!curr) {
		puts("Lista je prazna!");
		return 0;
	}

	while (curr) {
		printf("\n%s %s %d\n", curr->name, curr->lastName, curr->yearOfBirth);
		curr = curr->next;
	}

	return 0;
}

int insertEnd(char* _name, char* _lastName, int _yearOfBirth, Person* head) {
	Person* newP = (Person*)malloc(sizeof(Person));

	if (!newP) {
		puts("Greska!");
		return -1;
	}

	strcpy(newP->name, _name);
	strcpy(newP->lastName, _lastName);
	newP->yearOfBirth = _yearOfBirth;

	Person* curr = head;

	while (curr->next) {
		curr = curr->next;
	}

	newP->next = curr->next;
	curr->next = newP;

	return 0;
}

Person* findByLastName(char* _lastName, Person* head) {
	Person* curr = head->next;

	while (curr) {
		if (!strcmp(curr->lastName, _lastName)) {
			return curr;
		}

		curr = curr->next;
	}

	return NULL;
}

int deletePerson(char* _lastName, Person* head) {
	Person* prev = head;

	if (!prev->next) {
		puts("Lista je prazna!");
		return 1;
	}

	Person* curr = findByLastName(_lastName, prev);
	if (!curr) {
		puts("Osoba nije pronadjena!");
		return 1;
	}

	while (prev->next != curr) {
		prev = prev->next;
	}

	prev->next = curr->next;

	free(curr);

	return 0;
}

int deleteAll(Person* head) {

	if (!head->next) {
		puts("Lista je prazna!");
		return 0;
	}

	while (head->next) {
		Person* temp = head->next->next;

		free(head->next);

		head->next = temp;
	}

	return 0;
}

int insertAfter(char* _targetLastName, char* _name, char* _lastName, int _yearOfBirth, Person* head) {

	Person* target = findByLastName(_targetLastName, head);

	if (!target) {
		printf("Osoba s prezimenom %s nije pronadjena!\n", _targetLastName);
		return -1;
	}

	Person* newP = (Person*)malloc(sizeof(Person));

	if (!newP) {
		puts("Greska!");
		return -1;
	}

	strcpy(newP->name, _name);
	strcpy(newP->lastName, _lastName);
	newP->yearOfBirth = _yearOfBirth;

	newP->next = target->next;
	target->next = newP;

	return 0;
}

int insertBefore(char* _targetLastName, char* _name, char* _lastName, int _yearOfBirth, Person* head) {

	Person* target = findByLastName(_targetLastName, head);

	if (!target) {
		printf("Osoba s prezimenom %s nije pronadjena!\n", _targetLastName);
		return -1;
	}

	Person* prev = head;
	while (prev->next != target) prev = prev->next;

	Person* newP = (Person*)malloc(sizeof(Person));

	if (!newP) {
		puts("Greska!");
		return -1;
	}

	strcpy(newP->name, _name);
	strcpy(newP->lastName, _lastName);
	newP->yearOfBirth = _yearOfBirth;

	newP->next = target;
	prev->next = newP;

	return 0;
}

int writeListToFile(char* fileName, Person* head) {
	if (!head->next) {
		puts("Nije moguce upisati listu u datoteku jer je lista prazna!");
		return -1;
	}

	FILE* fp = fopen(fileName, "w");
	if (!fp) {
		puts("Greska pri otvaranju datoteke!");
		return -1;
	}

	Person* curr = head->next;
	
	while (curr) {
		if (fprintf(fp, "%s %s %d\n", curr->name, curr->lastName, curr->yearOfBirth) <= 0) {
			puts("Doslo je do greske pri pisanju u datoteku!");
			fclose(fp);
			return -1;
		}
		curr = curr->next;
	}

	fclose(fp);

	return 0;
}

int readListFromFile(char* fileName, Person* head) {
	FILE* fp = fopen(fileName, "r");
	if (!fp) {
		puts("Greska pri otvaranju datoteke!");
		return -1;
	}

	deleteAll(head);

	Person temp;
	int counter = 0;
	
	while (fscanf(fp, "%s %s %d", temp.name, temp.lastName, &temp.yearOfBirth) == 3) {
		insertEnd(temp.name, temp.lastName, temp.yearOfBirth, head);
		counter++;
	}

	if (counter == 0) {

		if (feof(fp)) puts("Nije moguce citati iz datoteke jer je datoteka prazna!");
		else puts("Doslo je do greske pri citanju iz datoteke!");

		fclose(fp);
		return -1;
	}

	fclose(fp);

	return 0;
}

int sortList(Person* head) {
	if (!head->next) {
		puts("Nije moguce sortirati praznu listu!");
		return -1;
	}

	int N = 0;
	Person* prev = head;

	while (prev->next) {
		N++;
		prev = prev->next;
	}

	for (int i = 0; i < N - 1; i++) {

		prev = head;
		Person* curr = head->next;

		for (int j = 0; j < N - 1 - i; j++) {

			int resultLastName = strcmp(curr->lastName, curr->next->lastName);
			int resultName = strcmp(curr->name, curr->next->name);

			if ((resultLastName > 0) ||
				(resultLastName == 0 && resultName > 0) ||
				(resultLastName == 0 && resultName == 0 && curr->yearOfBirth > curr->next->yearOfBirth)) {

				Person* temp = curr->next;

				prev->next = temp;
				curr->next = temp->next;
				temp->next = curr;

				prev = temp;
			}
			else {
				prev = curr;
				curr = curr->next;
			}
		}
	}

	return 0;
}