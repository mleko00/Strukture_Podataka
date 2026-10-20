#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 64
#define EXIT_SUCCESS 0
#define ALLOC_ERROR -1
#define NOT_FOUND -2
#define GENERAL_ERROR -3


struct _Person;
typedef struct _Person* Position;
typedef struct _Person {
    char name[MAX_LENGTH];
    char surname[MAX_LENGTH];
    int birthYear;
    Position next;
} Person;


int insertBeginning(Position head, char* name, char* surname, int birthYear);
int insertAfter(Position p, Position newPerson);
int printList(Position first);
int insertEnd(Position head, char* name, char* surname, int birthYear);
Position findPerson(Position first, char* surname);
Position findBefore(Position first, char* surname);
int deletePerson(Position head, char* surname);
int menu();

int main() {
    Person head = { .name = {0}, .surname = {0}, .birthYear = 0, .next = NULL };
    int choice = 0;
    char searchSurname[MAX_LENGTH] = { 0 };
    char name[MAX_LENGTH] = { 0 };
    char surname[MAX_LENGTH] = { 0 };
    int birthYear = 0;
    int status = 0;

    while (1) {
        choice = menu();

        switch (choice) {
        case 0:
            printf("Izlaz iz programa.\n");
            return EXIT_SUCCESS;

        case 1:
            printf("Unesite ime nove osobe: ");
            scanf(" %s", name);
            printf("Unesite prezime nove osobe: ");
            scanf(" %s", surname);
            printf("Unesite godinu rođenja nove osobe: ");
            scanf("%d", &birthYear);

            status = insertBeginning(&head, name, surname, birthYear);
            if (status != EXIT_SUCCESS) {
                printf("Greška pri unosu na početak liste!\n");
            }
            break;

        case 2:
            printf("\n--- POPIS OSOBA ---\n");
            status = printList(head.next);
            if (status < 0) {
                printf("Lista je prazna!\n");
            }
            break;

        case 3:
            printf("Unesite ime nove osobe: ");
            scanf(" %s", name);
            printf("Unesite prezime nove osobe: ");
            scanf(" %s", surname);
            printf("Unesite godinu rođenja nove osobe: ");
            scanf("%d", &birthYear);

            status = insertEnd(&head, name, surname, birthYear);
            if (status != EXIT_SUCCESS) {
                printf("Greška pri unosu na kraj liste!\n");
            }
            break;

        case 4:
            printf("Unesite prezime tražene osobe: ");
            scanf(" %s", searchSurname);
            Position found = findPerson(head.next, searchSurname);
            if (found != NULL) {
                printf("Pronađena osoba: %s %s, %d. godina\n", found->name, found->surname, found->birthYear);
            } else {
                printf("Osoba s tim prezimenom nije pronađena.\n");
            }
            break;

        case 5:
            printf("Unesite prezime osobe koju želite izbrisati: ");
            scanf(" %s", searchSurname);
            status = deletePerson(&head, searchSurname);
            if (status == EXIT_SUCCESS) {
                printf("Osoba je uspješno izbrisana.\n");
            } else {
                printf("Brisanje nije uspjelo (osoba nije pronađena).\n");
            }
            break;

        default:
            printf("Pogrešan unos, pokušajte ponovno.\n");
        }
    }

    return EXIT_SUCCESS;
}


int insertAfter(Position p, Position newPerson) {
    if (p == NULL || newPerson == NULL) {
        return GENERAL_ERROR;
    }
    newPerson->next = p->next;
    p->next = newPerson;
    return EXIT_SUCCESS;
}


int insertBeginning(Position head, char* name, char* surname, int birthYear) {
    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Pogreška pri alokaciji memorije!\n");
        return ALLOC_ERROR;
    }

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;

    insertAfter(head, newPerson);

    return EXIT_SUCCESS;
}


int printList(Position first) {
    if (first == NULL) {
        return NOT_FOUND;
    }

    Position temp = first;
    while (temp != NULL) {
        printf("Ime: %s, Prezime: %s, Godina rođenja: %d\n", temp->name, temp->surname, temp->birthYear);
        temp = temp->next;
    }
    return EXIT_SUCCESS;
}


int insertEnd(Position head, char* name, char* surname, int birthYear) {
    Position temp = head;

    while (temp->next != NULL) {
        temp = temp->next;
    }

    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Pogreška pri alokaciji memorije!\n");
        return ALLOC_ERROR;
    }

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;

    insertAfter(temp, newPerson);

    return EXIT_SUCCESS;
}


Position findPerson(Position first, char* surname) {
    Position temp = first;
    while (temp != NULL) {
        if (strcmp(temp->surname, surname) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

Position findBefore(Position first, char* surname) {
    Position temp = first;
    while (temp->next != NULL) {
        if (strcmp(temp->next->surname, surname) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}


int deletePerson(Position head, char* surname) {
    Position prev = findBefore(head, surname);
    if (prev == NULL) {
        return NOT_FOUND;
    }

    Position toDelete = prev->next;
    prev->next = toDelete->next;
    free(toDelete);

    return EXIT_SUCCESS;
}

int menu() {
    int choice = 0;
    printf("\n--- IZBORNIK ---\n"
        "0 - Izlaz\n"
        "1 - Unos novog elementa na početak liste\n"
        "2 - Ispis liste\n"
        "3 - Unos novog elementa na kraj liste\n"
        "4 - Pronalazak elementa u listi (po prezimenu)\n"
        "5 - Brisanje elementa iz liste\n"
        "Unesite vaš izbor: ");
    scanf("%d", &choice);
    return choice;
}