#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define LINE 20
#define MAX_LINE 1024           //obavezno magic numbers
#define EXIT_SUCCESS 0
#define FILE_ERROR_OPEN -1      //definirati greske i njihove returnove
#define ALLOC -2
#define SCANF_ERROR -3

/*  cheatsheet, 
 kako definirati strukture, pokazivace*/ 

//struct _Student;                      //forrward decl         
//typedef struct _Student* Position;    //pokazivač na studenta

typedef struct _Student {

	char name[LINE];        
	char lastName[LINE];    
	int bodovi;             

	//Position next;                   //kasnije ce trebat
    
} Stud;

//obavezno deklarirati funkciju prije maina, tek kasnije definirati
int procitajBrojRedaka();

int main() {
	
	int i = 0;                  //obavezno početne vrijednosti dodjeliti
	int brojRedaka = 0;
	int maksBodovi = 0;

	FILE* fp = NULL;            //pokazivač na datoteku, filePointer
	Stud* Student = NULL;       //pokazivac na dinamicki niz

	brojRedaka = procitajBrojRedaka();
	
	//provjera ima li greske
	if (brojRedaka < 0) {
		return FILE_ERROR_OPEN;
	}

    //ako ima studenata u datot
	if (brojRedaka > 0)
	{
		//uvijek moramo ponovo otvarat
		fp = fopen("popis.txt", "r");       //r je za read
		if (!fp) {
			printf("Greska pri otvaranju datoteke u mainu \n");
			return FILE_ERROR_OPEN;
		}

		//dinamicka alokacija
		Student = (Stud*)malloc(brojRedaka * sizeof(Stud)); 
		if (Student == NULL) {              //provjera malloca
			printf("Greska s mallocom\n");
			fclose(fp);
			return ALLOC; 
		}

		//ucitavanje podataka, trazenje maksimuma
		for (i = 0; i < brojRedaka; i++) {
			
            /* ako je fscanf razlicito od 3 (tribaju nam samo 3 podatka), 
               znaci da nesto nije dobro  */
            //& treba samo za brojeve, nizovi ime i prezime ne trebaju
			if (fscanf(fp, " %s %s %d ", Student[i].name, Student[i].lastName, &Student[i].bodovi) != 3) {
				free(Student);          //prekidamo prog
				fclose(fp);
				return SCANF_ERROR;
			}

			//trazimo najveci broj bodova
			if (Student[i].bodovi > maksBodovi) {
				maksBodovi = Student[i].bodovi;
			}
		}

		
		fclose(fp);             /* zatvarati obavezno nakon  
                                   otvaranja, da ne zaboravimo */


		//formula za relativne bodove, ispis na ekran
		
		for (i = 0; i < brojRedaka; i++) {
			double relativniBodovi = 0.0;
			
			if (maksBodovi > 0) {
				//moram staviti double ispred bodova da ne zaokruži na nulu
				relativniBodovi = ((double)Student[i].bodovi / maksBodovi) * 100;
			}

			
			printf("Student: %s %s\n", Student[i].name, Student[i].lastName);
			printf("Apsolutni bodovi: %d\n", Student[i].bodovi);
			printf("Relativni bodovi: %.2lf%%\n", relativniBodovi);
			printf("\n");
		}

		// OBAVEZNO oslobadjanje memorije na kraju
		free(Student);
	}

	return EXIT_SUCCESS;
}


 // broji retke u datoteci preko feof i fgets, kako smo na satu radili

int procitajBrojRedaka()
{
	int brojac = 0;                 
	char buffer[MAX_LINE] = {0};    // medjuspremnik, privremeno ucitavamo
	FILE* fp = NULL;                // lokalni pokazivac na datot

	fp = fopen("popis.txt", "r");
	if (!fp) {                      // ako ne postoji
		printf("Greska pri otvaranju u funkciji!\n");
		return FILE_ERROR_OPEN;
	}

	/* petlja koja se izvsava dok ne stigne do kraja datoteke
        fgets zahvati cijelu liniju, ako postoji
        brojac se inkrementira */

	while (!feof(fp)) {
		if (fgets(buffer, 50, fp) != NULL) {
			brojac++;
		}
	}

	//opet obavezno zatvaranje
	fclose(fp);

	return brojac;
}
