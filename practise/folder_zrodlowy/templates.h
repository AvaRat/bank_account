/*
 * main.h
 *
 *  Created on: 4 sty 2018
 *      Author: marcel
 */

#ifndef TEMPLATES_H_
#define TEMPLATES_H_

#define N 30
#define S 40	//maksymalna dlugosc nazwy

#define OPCJE "\n'q' - zamknij program\t'd' - doladuj konto\t'w' - wyswietl stan\t'h' - pokaz historie operacji\n\t\t\t'p' - przelew\t'n' - nowy kontakt\t'k' - kontakty\t'e' - edycja kontaktow\n"
#define ALOK_CHECK(wsk) {if(wsk == NULL){printf("Blad alokacji pamieci w lini: %d\n", __LINE__); exit(EXIT_FAILURE);}}


typedef struct ad{
	char street[S];
	unsigned int nr;
	char city[N];
} adres;

typedef struct da{
	char name[N];
	char surname[S];
	adres adress;
	struct da *next_odb;
}dane;

typedef struct op{
	char time[S];
	char tytul[S];
	dane odbiorca;
	float value;
	float saldo;
	struct op *next_op;
}operacja;

typedef enum  {NEW, EXISTING}cont;
typedef enum  {SMALL, BIG}what_list;

void doladuj_konto(operacja **);
void wysw_stan(operacja *);
void nowy_przelew(dane **, operacja **);
void dodaj_odbiorce(dane **, cont);
dane **find_cont(dane **);
void print_list_odb(dane **, unsigned int);
void delete_contact(dane ** , dane *);
void print_history(operacja *);
void edit_contacts(dane **);
void get_data(char *);

#endif /* TEMPLATES_H_ */
