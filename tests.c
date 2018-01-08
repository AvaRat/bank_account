/*
 * tests.c
 *
 *  Created on: 4 sty 2018
 *      Author: marcel
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "templates.h"

/*!
* \fn void usun_pierwszy(dane **kontakty)
* \brief funkcja testuj¹ca usówanie jedynego kontaktu w liœcie
* \details funkcja nie wczytuje bazy danych z pliku, lecz alokuje jeden kontakt a nastêpnie sprawdza, czy wszystko dobrze dzia³a
* \param[in] **kontakty lista kontaktów
*/
void usun_pierwszy(dane **kontakty)
{
	*kontakty = malloc(sizeof(dane));
	strcpy((*kontakty)->name, "Jan");
	strcpy((*kontakty)->surname, "Kowalski");
	(*kontakty)->next_odb = NULL;
	printf("Test usuniecia jedynego kontaktu z listy jednokierunkowej\npo kazdej operacji nacisnij enter aby kontynuowac\n\n");
	printf("zaalokowano jeden kontakt\n");
	getchar();
	printf("wyswietlenie listy\n");
	print_list_odb(kontakty, SMALL);
	getchar();
	printf("kontakt zostaje usuniety\n\n");
	delete_contact(kontakty, *kontakty);
	getchar();
	printf("wyswietlenie listy\n");
	print_list_odb(kontakty, SMALL);
	getchar();
}

/*!
* \fn void from_scratch(dane **kontakty, operacja **history)
* \brief test, czy wszystko dobrze dziala gdy otwieramy program z pust¹ baz¹ danych (pustym plikiem)
* \param[in] **kontakty lista kontaktów
* \param[in] **history lista operacji
*/
void from_scratch(dane **kontakty, operacja **history)
{
	printf("TEST aplikacji, gdy nie ma jeszcze zadnej bazy danych ani zadnego kontaktu\nNacisnij enter po kazdej operacji aby kontynuowac\n\n");
	printf("\t\tLista odbiorcow\n");
	print_list_odb(kontakty, BIG);
	if(getchar()=='\n')
	{
		printf("\t\tHistoria operacji\n");
		print_history(*history);
	}
	if(getchar() == '\n')
	{
		printf("\t\tRobienie przelewu\n");
		nowy_przelew(kontakty, history);
	}
	if(getchar() == '\n')
	{
		printf("\t\tedycja kontaktu\n");
		edit_contacts(kontakty);
	}
	printf("Koniec testu\n");
}
/*!
* \details funkcja sprawdza, czy mo¿emy dodaæ wilkie iloœci danych (10000 kontaków i 10000 operacji do³adowania) i w jakim czasie jesteœmy w stanie to zrobiæ. Ponad to
*	testuje ona czy du¿a iloœæ operacji nie opóŸni odczytywania stanu konta. (¿eby odczytaæ stan konta trzeba "dolecieæ" do koñca listy operacji)
* \param[in] **kontakty lista kontaktów
* \param[in] **history lista operacji
*/
void add_a_lot(dane **kontakty, operacja **history)
{
	unsigned int n;
	printf("po kazdej operacji nacisnij enter\n\n");
	printf("Tworzenie 10000 kontaktow\nwcisnij enter\n\n");
	getchar();
	for(n=0;  n<10000;  n++)
	{
		dodaj_odbiorce(kontakty, NEW, TEST);
	}
	print_list_odb(kontakty, SMALL);
	printf("\ntworzenie 10000 operacji\n");
	printf("**press enter**\n\n");
	getchar();
	for(n=0;  n<10000;  n++)
	{
	doladuj_konto(history, TEST);
	}
	printf("**press enter**\n");
	getchar();
	print_history(*history);
	printf("**press enter**\n");
	getchar();
	wysw_stan(*history);
	printf("nacisnij enter aby zakonczyc\n");
	getchar();
}


