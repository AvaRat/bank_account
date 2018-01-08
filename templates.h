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
#define ALOK_CHECK(wsk) {if(wsk == NULL){printf("Blad alokacji pamieci w lini: %d\n", __LINE__); return -1;}}/*!<sprawdza, czy pamiêæ zosta³a poprawnie zaalokowana*/
#define FUN_CHECK(fun) {if(fun != 0) return -1;}
#define FUN_M_CHECK(fun) {if(fun != 0) {printf("cos poszlo nie tak\nsprobuj jeszcze raz\n"); break;}}/*!<makro które przerywa pêtle, gdy funkcja nie zwróci '0'*/

/*!
* @brief struktura pomocnicza do przechowywania adresów
*/
typedef struct ad{
	char street[S];	/*!< ulica*/
	unsigned int nr;	/*!<numer domu*/
	char city[S];	/*!<miasto*/
} adres;

/*!
* @brief struktura bêd¹ca podstaw¹ listy kontaktów
*/
typedef struct da{
	char name[N];	/*!< imiê*/
	char surname[S];	/*!< nazwisko*/
	adres adress;	/*!< struktura przechowuj¹ca adres*/
	struct da *next_odb;	/*!< wskaŸnik do nastêpnego kontaktu*/
}dane;

/*!
* @brief g³ówna struktura programu, gdzie przechowywane s¹ wszystkie operacjie i stan konta
*/
typedef struct op{
	char time[S];	/*!< czas wykonania operacji*/
	char tytul[S];	/*!< tytu³ operacji*/
	dane odbiorca;	/*!<dane odbiorcy*/
	float value;	/*!< kwota operacji*/
	float saldo;	/*!<saldo po operacji*/
	struct op *next_op;	/*!< wskaŸnik do nastêpnej operacji*/
}operacja;

/*!
* \enum	what_cont
* \brief wyliczenie dla funkcji dodaj_odbiorce()
*/
typedef enum	{NEW, EXISTING}what_cont;
/*\enum 
* \brief wyliczenie dla funkcji print_list_odb()
*/
typedef enum	{SMALL, BIG}what_list;
/*\enum
* \brief wylicznenie mówi¹ce, czy funkcja dzia³a w ramach testu, czy w rzeczywistej aplikacji
*/
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
