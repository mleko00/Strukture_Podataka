#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SUCCESS (0)
#define ERROR_GENERAL (-1)
#define ERROR_MALLOC (-2)
#define ERROR_FILE (-3)
#define MAX_STRING (64)
#define MAX_LINE (1024)

struct _Element;
typedef struct _Element* Position;
typedef struct _Element {
	double number;
	Position next;
} Element;

int readFromFile(Position head);
int calculateOperation(char operator, Position head);
Position createElement(double number);
int deleteFirst(Position head);
int deleteAll(Position head);
int push(Position head, double tempNum);
double pop(Position head);
int printPostfixResult(Position head);

int main() {
	Element head = { 0, NULL };
	int status = 0;

	status = readFromFile(&head);

	if (status == SUCCESS) {
		deleteAll(&head);
	}

	return EXIT_SUCCESS;
}

int readFromFile(Position head) {
	FILE* fp = NULL;
	char buffer[MAX_LINE] = { 0 };
	char fileName[MAX_STRING] = { 0 };
	char operator = '0';
	int b = 0;
	double tempNumber = 0;
	int check = 0;

	printf("Unesite ime datoteke (s nastavkom):\n");
	if (scanf(" %63s", fileName) != 1) {
		return ERROR_GENERAL;
	}

	fp = fopen(fileName, "r");
	if (!fp) {
		printf("Datoteka se ne moze otvoriti");
		return ERROR_FILE;
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		char* pbuffer = buffer;
		while (*pbuffer != '\0' && *pbuffer != '\n' && *pbuffer != '\r') {
			b = 0;
			if (sscanf(pbuffer, " %lf %n", &tempNumber, &b) == 1) {
				push(head, tempNumber);
				pbuffer += b;
			}
			else if (sscanf(pbuffer, " %c %n", &operator, &b) == 1) {
				calculateOperation(operator, head);
				pbuffer += b;
			}
			else {
				pbuffer++;
			}
		}
	}

	fclose(fp);

	check = printPostfixResult(head);

	if (check != SUCCESS) {
		return ERROR_GENERAL;
	}

	return SUCCESS;
}

int calculateOperation(char operator, Position head) {
	double num1 = 0;
	double num2 = 0;
	double result = 0;

	if (!head->next || !head->next->next) {
		printf("Postfix izraz netocno napisan\n");
		return ERROR_GENERAL;
	}

	num1 = pop(head);
	num2 = pop(head);

	switch (operator) {
	case '+':
		result = num2 + num1;
		break;
	case '-':
		result = num2 - num1;
		break;
	case '*':
		result = num2 * num1;
		break;
	case '/':
		if (num1 == 0) {
			printf("Nije moguce dijeljenje s nulom\n");
			return ERROR_GENERAL;
		}
		result = num2 / num1;
		break;
	default:
		printf("Nepoznat operator: %c\n", operator);
		return ERROR_GENERAL;
	}

	push(head, result);

	return SUCCESS;
}

Position createElement(double number) {
	Position newElement = NULL;

	newElement = (Position)malloc(sizeof(Element));
	if (!newElement) {
		printf("Alokacija memorije neuspjesna\n");
		return NULL;
	}

	newElement->number = number;
	newElement->next = NULL;

	return newElement;
}

int push(Position head, double tempNum) {
	Position newElement = NULL;

	newElement = createElement(tempNum);
	if (!newElement) {
		return ERROR_MALLOC;
	}

	newElement->next = head->next;
	head->next = newElement;

	return SUCCESS;
}

int deleteFirst(Position head) {
	Position temp = NULL;

	temp = head->next;
	if (!temp) {
		return SUCCESS;
	}

	head->next = temp->next;
	free(temp);

	return SUCCESS;
}

int deleteAll(Position head) {
	int count = 0;

	while (head->next != NULL) {
		deleteFirst(head);
		count++;
	}

	return count;
}

double pop(Position head) {
	double toPop = 0;

	if (!head->next) {
		printf("Postfix izraz netocno napisan\n");
		return 0;
	}
	toPop = head->next->number;
	deleteFirst(head);

	return toPop;
}

int printPostfixResult(Position head) {
	if (!head->next || head->next->next) {
		printf("Postfix izraz netocno napisan\n");
		return ERROR_GENERAL;
	}

	printf("Rezultat postfixa je: %.2lf\n", head->next->number);

	return SUCCESS;
}
