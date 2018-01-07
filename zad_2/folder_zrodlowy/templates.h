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
#define ALOK_CHECK(wsk) {if(wsk == NULL){printf("Blad alokacji pamieci w lini: %d\n", __LINE__); return -1;}}
#define FUN_CHECK(fun) {if(fun != 0) return -1;}
#define FUN_M_CHECK(fun) {if(fun != 0) {printf("cos poszlo nie tak\nsprobuj jeszcze raz\n"); break;}}

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

typedef enum	{NEW, EXISTING}what_cont;
typedef enum	{SMALL, BIG}what_list;
typedef enum	{REAL, TEST}test;

int doladuj_konto(operacja **, test);
void wysw_stan(operacja *);
int nowy_przelew(dane **, operacja **);
int dodaj_odbiorce(dane **, what_cont, test);
dane **find_cont(dane **);
void print_list_odb(dane **, unsigned int);
void delete_contact(dane ** , dane *);
void print_history(operacja *);
int edit_contacts(dane **);
void get_data(char *);

#endif /* TEMPLATES_H_ */
