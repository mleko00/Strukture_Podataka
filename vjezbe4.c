#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

struct Poli;
typedef struct Poli* Position;
typedef struct Poli {
    int coef;
    int pow;
    Position Next;
} polinom;


int read_from_file(Position head1, Position head2, char* fileName);
int read_line(Position head, char* string, int n);
int insert_sorted(Position head, int k, int e);
Position create_element(int k, int e);
int print_polynom(Position first);
int sum_polynom(Position first1, Position first2, Position headSum);
int clear_zeros(Position head);
int multiply_polynom(Position first1, Position first2, Position headProdukt);
int delete_after(Position previous);
int delete_all(Position head);

int main(int argc, char** argv) {
    char fileName[MAX_LINE] = { 0 };
    
    
    polinom head1 = { .coef = 0, .pow = 0, .Next = NULL };
    polinom head2 = { .coef = 0, .pow = 0, .Next = NULL };
    polinom headSum = { .coef = 0, .pow = 0, .Next = NULL };
    polinom headProdukt = { .coef = 0, .pow = 0, .Next = NULL };

    Position p1 = &head1;
    Position p2 = &head2;
    Position pS = &headSum;
    Position pP = &headProdukt;

    printf("Unesite ime datoteke: \n");
    scanf(" %s", fileName);

    
    if (read_from_file(p1, p2, fileName) == 0) {
        printf("\nPrvi polinom:\t");
        print_polynom(p1->Next);

        printf("\nDrugi polinom:\t");
        print_polynom(p2->Next);

        
        sum_polynom(p1->Next, p2->Next, pS);
        printf("\nZbroj polinoma:\t");
        print_polynom(pS->Next);

        
        multiply_polynom(p1->Next, p2->Next, pP);
        printf("\nProdukt polinoma: ");
        print_polynom(pP->Next);
    }

    
    delete_all(p1);
    delete_all(p2);
    delete_all(pS);
    delete_all(pP);

    printf("\n");

    return 0;
}


int read_from_file(Position head1, Position head2, char* fileName) {
    FILE* fp = NULL;
    char string[MAX_LINE] = { 0 };

    fp = fopen(fileName, "r");
    if (!fp) {
        printf("Neuspjesno otvaranje datoteke!\n");
        return -1;
    }

    if (fgets(string, MAX_LINE, fp) != NULL) {
        read_line(head1, string, (int)strlen(string));
    }

    if (fgets(string, MAX_LINE, fp) != NULL) {
        read_line(head2, string, (int)strlen(string));
    }

    fclose(fp);

    return 0;
}


int read_line(Position head, char* string, int n) {
    string[n] = '\0';
    int k = 0, e = 0;
    int t = 0;
    char* sstring = string;
    int check = 0;

    while (strlen(sstring) > 0) {
        t = 0;
        check = sscanf(sstring, " %d %d %n", &k, &e, &t);
        if (check != 2)
            break;
        sstring += t;

        insert_sorted(head, k, e);
    }

    return 0;
}


int insert_sorted(Position head, int k, int e) {
    Position temp = head;
    Position newEl = NULL;

    
    while (temp->Next != NULL && temp->Next->pow > e) {
        temp = temp->Next;
    }

    
    if (temp->Next != NULL && temp->Next->pow == e) {
        temp->Next->coef += k;
    } else {
        newEl = create_element(k, e);
        if (!newEl) {
            return -2;
        }
        newEl->Next = temp->Next;
        temp->Next = newEl;
    }

    return 0;
}


Position create_element(int k, int e) {
    Position newEl = NULL;
    newEl = (Position)malloc(sizeof(polinom));
    if (!newEl) {
        perror("Neuspjesno alociranje memorije!\n");
        return NULL;
    }

    newEl->pow = e;
    newEl->coef = k;
    newEl->Next = NULL;

    return newEl;
}


int print_polynom(Position first) {
    Position temp = first;

    if (temp == NULL) {
        printf(" 0 ");
        return 0;
    }

    while (temp != NULL) {
        if (temp->coef != 0) {
            if (temp != first && temp->coef > 0) {
                printf(" + ");
            } else if (temp->coef < 0) {
                printf(" - ");
            }

            int absCoef = (temp->coef < 0) ? -temp->coef : temp->coef;

            if (temp->pow == 0) {
                printf("%d", absCoef);
            } else if (temp->pow == 1) {
                if (absCoef == 1) printf("X");
                else printf("%dX", absCoef);
            } else {
                if (absCoef == 1) printf("X^%d", temp->pow);
                else printf("%dX^%d", absCoef, temp->pow);
            }
        }
        temp = temp->Next;
    }

    return 0;
}


int sum_polynom(Position first1, Position first2, Position headSum) {
    Position temp1 = first1;
    Position temp2 = first2;

    while (temp1 != NULL) {
        insert_sorted(headSum, temp1->coef, temp1->pow);
        temp1 = temp1->Next;
    }

    while (temp2 != NULL) {
        insert_sorted(headSum, temp2->coef, temp2->pow);
        temp2 = temp2->Next;
    }

    clear_zeros(headSum);

    return 0;
}


int multiply_polynom(Position first1, Position first2, Position headProdukt) {
    Position temp1 = first1;
    Position temp2 = first2;

    while (temp1 != NULL) {
        temp2 = first2;
        while (temp2 != NULL) {
            insert_sorted(headProdukt, temp1->coef * temp2->coef, temp1->pow + temp2->pow);
            temp2 = temp2->Next;
        }
        temp1 = temp1->Next;
    }

    clear_zeros(headProdukt);

    return 0;
}


int clear_zeros(Position head) {
    Position p = head;
    Position temp = NULL;

    while (p->Next != NULL) {
        if (p->Next->coef == 0) {
            temp = p->Next;
            p->Next = temp->Next;
            free(temp);
        } else {
            p = p->Next;
        }
    }

    return 0;
}


int delete_after(Position previous) {
    Position temp = previous->Next;
    previous->Next = temp->Next;
    free(temp);

    return 0;
}


int delete_all(Position head) {
    while (head->Next != NULL) {
        delete_after(head);
    }

    return 0;
}