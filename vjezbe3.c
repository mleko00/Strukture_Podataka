#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 64
#define EXIT_SUCCESS 0
#define ALLOC_ERROR -1
#define NOT_FOUND -2
#define GENERAL_ERROR -3
#define FILE_ERROR -4


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
int deleteList(Position head); 


int insertAfterPerson(Position head, char* targetSurname, char* name, char* surname, int birthYear); 
int insertBeforePerson(Position head, char* targetSurname, char* name, char* surname, int birthYear); 
int sortList(Position head); 
int writeToFile(Position first, char* filename); 
int readFromFile(Position head, char* filename); 

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
            printf("Izlaz iz programa\n");
            deleteList(&head); 
            return EXIT_SUCCESS;

        case 1: 
            printf("Unesite ime: ");
            scanf(" %s", name);
            printf("Unesite prezime: ");
            scanf(" %s", surname);
            printf("Unesite godinu rođenja: ");
            scanf("%d", &birthYear);
            insertBeginning(&head, name, surname, birthYear);
            break;

        case 2: 
            printf("\n--- POPIS OSOBA ---\n");
            if (printList(head.next) < 0) {
                printf("Lista je prazna\n");
            }
            break;

        case 3: 
            printf("Unesite ime: ");
            scanf(" %s", name);
            printf("Unesite prezime: ");
            scanf(" %s", surname);
            printf("Unesite godinu rođenja: ");
            scanf("%d", &birthYear);
            insertEnd(&head, name, surname, birthYear);
            break;

        case 4: 
            printf("Unesite prezime koje tražite: ");
            scanf(" %s", searchSurname);
            Position found = findPerson(head.next, searchSurname);
            if (found != NULL) {
                printf("Pronađena osoba: %s %s, %d.\n", found->name, found->surname, found->birthYear);
            } else {
                printf("Osoba nije pronađena\n");
            }
            break;

        case 5: 
            printf("Unesite prezime osobe za brisanje: ");
            scanf(" %s", searchSurname);
            if (deletePerson(&head, searchSurname) == EXIT_SUCCESS) {
                printf("Osoba izbrisana\n");
            } else {
                printf("Osoba nije pronađena\n");
            }
            break;

        case 6: 
            printf("Unesite prezime osobe NAKON koje dodajemo: ");
            scanf(" %s", searchSurname);
            printf("Ime nove osobe: ");
            scanf(" %s", name);
            printf("Prezime nove osobe: ");
            scanf(" %s", surname);
            printf("Godina rođenja: ");
            scanf("%d", &birthYear);
            insertAfterPerson(&head, searchSurname, name, surname, birthYear);
            break;

        case 7: 
            printf("Unesite prezime osobe ISPRED koje dodajemo: ");
            scanf(" %s", searchSurname);
            printf("Ime nove osobe: ");
            scanf(" %s", name);
            printf("Prezime nove osobe: ");
            scanf(" %s", surname);
            printf("Godina rođenja: ");
            scanf("%d", &birthYear);
            insertBeforePerson(&head, searchSurname, name, surname, birthYear);
            break;

        case 8: 
            sortList(&head);
            printf("Lista je sortirana po prezimenima\n");
            break;

        case 9: 
            writeToFile(head.next, "popis.txt");
            printf("spremljeno u 'popis.txt'\n");
            break;

        case 10: // E. Čitanje iz datoteke
            readFromFile(&head, "popis.txt");
            printf(" učitano iz 'popis.txt'\n");
            break;

        default:
            printf("Krivi izbor\n");
        }
    }
    return EXIT_SUCCESS;
}


int insertAfter(Position p, Position newPerson) {
    if (p == NULL || newPerson == NULL) return GENERAL_ERROR;
    newPerson->next = p->next;
    p->next = newPerson;
    return EXIT_SUCCESS;
}

int insertBeginning(Position head, char* name, char* surname, int birthYear) {
    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) return ALLOC_ERROR;

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;

    insertAfter(head, newPerson);
    return EXIT_SUCCESS;
}

int printList(Position first) {
    if (first == NULL) return NOT_FOUND;
    Position temp = first;
    while (temp != NULL) {
        printf("Ime: %s, Prezime: %s, Godina: %d\n", temp->name, temp->surname, temp->birthYear);
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
    if (newPerson == NULL) return ALLOC_ERROR;

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;

    insertAfter(temp, newPerson);
    return EXIT_SUCCESS;
}

Position findPerson(Position first, char* surname) {
    Position temp = first;
    while (temp != NULL) {
        if (strcmp(temp->surname, surname) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

Position findBefore(Position first, char* surname) {
    Position temp = first;
    while (temp->next != NULL) {
        if (strcmp(temp->next->surname, surname) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}


int deletePerson(Position head, char* surname) {
    Position prev = findBefore(head, surname);
    if (prev == NULL) return NOT_FOUND;

    Position toDelete = prev->next;
    prev->next = toDelete->next;
    free(toDelete); 

    return EXIT_SUCCESS;
}


int deleteList(Position head) {
    Position temp = NULL;
    while (head->next != NULL) {
        temp = head->next;
        head->next = temp->next;
        free(temp); 
    }
    return EXIT_SUCCESS;
}


int insertAfterPerson(Position head, char* targetSurname, char* name, char* surname, int birthYear) {
    Position target = findPerson(head->next, targetSurname);
    if (target == NULL) return NOT_FOUND;

    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) return ALLOC_ERROR;

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;

    insertAfter(target, newPerson);
    return EXIT_SUCCESS;
}


int insertBeforePerson(Position head, char* targetSurname, char* name, char* surname, int birthYear) {
    Position prev = findBefore(head, targetSurname);
    if (prev == NULL) return NOT_FOUND; 

    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) return ALLOC_ERROR;

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;

    insertAfter(prev, newPerson);
    return EXIT_SUCCESS;
}


int sortList(Position head) {
    if (head->next == NULL || head->next->next == NULL) return NOT_FOUND;

    int swapped;
    Position p;
    Position l = NULL;

    do {
        swapped = 0;
        p = head;
        while (p->next->next != l) {
            
            if (strcmp(p->next->surname, p->next->next->surname) > 0) {
                Position temp = p->next->next;
                p->next->next = temp->next;
                temp->next = p->next;
                p->next = temp;
                swapped = 1;
            }
            p = p->next;
        }
        l = p->next;
    } while (swapped);

    return EXIT_SUCCESS;
}


int writeToFile(Position first, char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) return FILE_ERROR;

    Position temp = first;
    while (temp != NULL) {
        fprintf(fp, "%s %s %d\n", temp->name, temp->surname, temp->birthYear);
        temp = temp->next;
    }

    fclose(fp);
    return EXIT_SUCCESS;
}


int readFromFile(Position head, char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) return FILE_ERROR;

    
    deleteList(head);

    char name[MAX_LENGTH];
    char surname[MAX_LENGTH];
    int birthYear;

    while (fscanf(fp, "%s %s %d", name, surname, &birthYear) == 3) {
        insertEnd(head, name, surname, birthYear);
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

int menu() {
    int choice = 0;
    printf("\n IZBOR \n"
        "0  Izlaz \n"
        "1  Unos na početak\n"
        "2  Ispis liste\n"
        "3  Unos na kraj\n"
        "4  Traženje po prezimenu\n"
        "5  Brisanje osobe \n"
        "6 - Unos NAKON osobe \n"
        "7 - Unos ISPRED osobe \n"
        "8 - Sortiraj po prezimenima \n"
        "9 - Spremi u datoteku \n"
        "10 - Učitaj iz datoteke \n"
        "Izaberi opciju: ");
    scanf("%d", &choice);
    return choice;
}