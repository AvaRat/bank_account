/*
 * files.c
 *
 *  Created on: 4 sty 2018
 *      Author: marcel
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "templates.h"


/*!
* \fn int save_and_free(operacja *op_list, dane *lista_odb)
* \details główna funkcja zapisująca i zwalniająca pamięć
* \param[in,out] *op_list lista operacji
* \param[in,out] *lista_odb lista kontaktów
* \retval 0 przy pomyślnym zakończeniu
* \retval -1 gdy funkcja napotka jakiś błąd
*/
int save_and_free(operacja *op_list, dane *lista_odb)
{
	unsigned int n_cont = 0, n_op = 0;
	FILE *fp_k = fopen(KONTAKTY, "wb");	/*!< otwarcie strumienia pliku z bazą zapisanych kontaktów w trybie do NADPISU, czyli plik zostaje nadpisany bazą danych z pamięci podręcznej*/
	FILE *fp_op = fopen(HISTORIA, "wb");/*!< otwarcie strumienia pliku z bazą zapisanych operacji w trybie do NADPISU, czyli plik zostaje nadpisany bazą danych z pamięci podręcznej*/
	if(fp_k==NULL || fp_op==NULL)
	{
		printf("nie udalo sie otworzyc pliku!!\n");
		printf("otworz program ponownie\n");
	}
	dane *wsk_odb = lista_odb;
	operacja *wsk_op = op_list;
	while(wsk_odb != NULL)
	{
		wsk_odb = wsk_odb->next_odb;
		n_cont++;
	}
	CHECK(1, fwrite(&n_cont, sizeof(unsigned int), 1, fp_k))

	FUN_CHECK(save_contacts(fp_k, lista_odb))

	fclose(fp_k);


	while(wsk_op != NULL)
	{
		wsk_op = wsk_op->next_op;
		n_op++;
	}
	CHECK(1, fwrite(&n_op, sizeof(unsigned int), 1, fp_op))
	FUN_CHECK(save_history(fp_op, op_list))
	fclose(fp_op);
	free_all(lista_odb, op_list);
	return 0;
}
/*!
* \fn void free_all(dane *contacts, operacja *history)
* @brief funkcja do zwalniania pamięci z list operacji i kontaktów
* @param[in,out] *contacts lista kontaktów
* @param[in,out] *history lista operacji
*/
void free_all(dane *contacts, operacja *history)
{
	dane *wsk = contacts;
	while(wsk != NULL)
	{
		contacts = wsk;
		wsk = wsk->next_odb;
		free(contacts);
	}

	operacja *wsk_op = history;
	while(wsk_op != NULL)
	{
		history = wsk_op;
		wsk_op = wsk_op->next_op;
		free(history);
	}
}
/*!
* \fn int read_file(dane **cont_list, operacja **history)
* @details ogólna funkcja zawierająca funkcja do odczytywanie danych z pliku i tworzenia struktur na ich podstawie
* @param[in,out] **cont_list pusta lista kontaktów, której zostanie zaalokowana pamięć
* @param[in,out] **history pusta lista operacji, której zostanie zaalokowana pamięć
* @retval 0 gdy funkcja zakończy działanie sukcesem
* @retval -1 gdy funkcje wewnątrz napotkają jakiś błąd
* @retval -2 gdzy funkcja nie będzie mogła otworzyć pliku
*/
int read_file(dane **cont_list, operacja **history)
{
	FILE *fp_contacts = fopen(KONTAKTY, "rb");	/*!<plik otwierany jest w trybie do odczytu binarnego (nie można go zmieniać)*/
	FILE *fp_history = fopen(HISTORIA, "rb");	/*!<plik otwierany jest w trybie do odczytu binarnego (nie można go zmieniać)*/
	if(fp_contacts==NULL || fp_history==NULL)
	{
		printf("Nie mozna otworzyc pliku \n");
		return -2;
	}
	FUN_CHECK(read_contacts(fp_contacts, cont_list))
	FUN_CHECK(read_history(fp_history, history))
	fclose(fp_contacts);
	fclose(fp_history);
	return 0;
}
/*!
* \fn int read_contacts(FILE *fp_k, dane **cont_list)
* @brief funkcja czytająca kontakty z pliku i alokująca na nie pamięć
* @param[in] *fp_k wskaźnik na strumień do pliku z zapisanymi kontaktami
* @param[in,out] **cont_list lista kontaktów
* @retval 0 kiedy funkcja zakończy działanie z sukcesem
* @retval -1 kiedy przy odczycie coś się nie będzie zgadzało
*/
int read_contacts(FILE *fp_k, dane **cont_list)
{
	unsigned int n_cont = 0;	/*!< liczba kontaktow*/
	unsigned int n = 0;			/*!<uniwersalna zmienna*/
	unsigned int n_read = 0;	/*!< liczba odczytanych kontaktów*/
	fread(&n_cont, sizeof(unsigned int), 1, fp_k);	/*!<odczytanie ile jest kontaktów w bazie danych*/
	dane **wsk_odb = cont_list;

	while(n_read < n_cont)
		{
			*wsk_odb = (dane *) malloc(sizeof(dane));
			ALOK_CHECK(wsk_odb)
			CHECK(1, fread(&n, sizeof(unsigned int), 1, fp_k))
			CHECK(n, fread((*wsk_odb)->name, sizeof(char), n, fp_k))

			CHECK(1, fread(&n, sizeof(unsigned int), 1, fp_k))
			CHECK(n, fread((*wsk_odb)->surname, sizeof(char), n, fp_k))

			CHECK(1, fread(&n, sizeof(unsigned int), 1, fp_k))
			CHECK(n, fread((*wsk_odb)->adress.street, sizeof(char), n, fp_k))

			CHECK(1, fread(&((*wsk_odb)->adress.nr), sizeof(unsigned int), 1, fp_k))

			CHECK(1, fread(&n, sizeof(unsigned int), 1, fp_k))
			CHECK(n, fread((*wsk_odb)->adress.city, sizeof(char), n, fp_k))

			wsk_odb = &((*wsk_odb)->next_odb);
			n_read++;	/*!<liczba odczytanych kontaktow zwieksza sie*/
		}
	printf("**contacts uploaded**\n");
	return 0;
}
/*!
* \fn int save_contacts(FILE *fp_k, dane *wsk_odb)
* @brief funkcja zapisująca kontakty z listy do pliku w formacie binarnym
* @param[in,out] *fp_k strumień pliku z kontaktami
* @param[in] *wsk_odb lista kontaktów
* @retval 0 kiedy funkcja zakończy działanie z sukcesem
* @retval -1 kiedy przy zapisie coś się nie będzie zgadzało
*/
int save_contacts(FILE *fp_k, dane *wsk_odb)
{
	unsigned int n = 0;
	while(wsk_odb != NULL)
		{
			n = strlen(wsk_odb->name);
			CHECK(1, fwrite(&n, sizeof(unsigned int), 1, fp_k))
			CHECK(n, fwrite(wsk_odb->name, sizeof(char), strlen(wsk_odb->name), fp_k))

			n = strlen(wsk_odb->surname);
			CHECK(1, fwrite(&n, sizeof(unsigned int), 1, fp_k))
			CHECK(n, fwrite(wsk_odb->surname, sizeof(char), strlen(wsk_odb->surname), fp_k))

			n = strlen(wsk_odb->adress.street);
			CHECK(1, fwrite(&n, sizeof(unsigned int), 1, fp_k))
			CHECK(n, fwrite(wsk_odb->adress.street, sizeof(char), strlen(wsk_odb->adress.street), fp_k))

			CHECK(1, fwrite(&(wsk_odb->adress.nr), sizeof(unsigned int), 1, fp_k))

			n = strlen(wsk_odb->adress.city);
			CHECK(1, fwrite(&n, sizeof(unsigned int), 1, fp_k))
			CHECK(n, fwrite(wsk_odb->adress.city, sizeof(char), strlen(wsk_odb->adress.city), fp_k))

			wsk_odb = wsk_odb->next_odb;
		}
		printf("**contacts saved**\n");
		return 0;
}
/*!
* \fn int save_history(FILE *fp_op, operacja *wsk_op)
* @brief funkcja zapisująca historię operacji z listy do pliku w formacie binarnym
* @param[in,out] *fp_op strumień pliku z historią operacji
* @param[in] *wsk_odb lista operacji
* @retval 0 kiedy funkcja zakończy działanie z sukcesem
* @retval -1 kiedy przy zapisie coś się nie będzie zgadzało
*/
int save_history(FILE *fp_op, operacja *wsk_op)
{
	unsigned int n = 0;

	while(wsk_op != NULL)
	{
		n = strlen(wsk_op->time);
		CHECK(1, fwrite(&n, sizeof(unsigned int), 1, fp_op));
		CHECK(n, fwrite(wsk_op->time, sizeof(char), n, fp_op));

		n = strlen(wsk_op->tytul);
		CHECK(1, fwrite(&n, sizeof(unsigned int), 1, fp_op));
		CHECK(n, fwrite(wsk_op->tytul, sizeof(char), n, fp_op));

		n = strlen(wsk_op->odbiorca.surname);
		CHECK(1, fwrite(&n, sizeof(unsigned int), 1, fp_op));
		CHECK(n, fwrite(wsk_op->odbiorca.surname, sizeof(char), n, fp_op));

		CHECK(1, fwrite(&(wsk_op->value), sizeof(float), 1, fp_op));

		CHECK(1, fwrite(&(wsk_op->saldo), sizeof(float), 1, fp_op));

		wsk_op = wsk_op->next_op;
	}

	printf("**History saved**\n");
	return 0;
}
/*!
* \fn int read_history(FILE *fp_op, operacja **history)
* @brief funkcja wczytująca historię i alokująca pamięć na listę operacji
* @param[in] *fp_op strumień pliku z historią operacji
* @param[in] **history historia operacji
* @retval 0 kiedy funkcja zakończy działanie z sukcesem
* @retval -1 kiedy przy odczycie coś się nie będzie zgadzało
*/
int read_history(FILE *fp_op, operacja **history)
{
	unsigned int n = 0;			/*!< dlugosc słów */
	unsigned int n_op = 0;		/*!< liczba operacji do wczytania */
	unsigned int n_read = 0;	/*!< liczba odczytanych operacji */

	fread(&n_op, sizeof(unsigned int), 1, fp_op);	/*!< za[isanie na początku pliku ile jest operacji wykonanych do tej pory w bazie danych*/

	operacja **wsk_op = history;
	while(n_read < n_op)
	{
		*wsk_op = (operacja *) malloc(sizeof(operacja));
		ALOK_CHECK(wsk_op)
		CHECK(1, fread(&n, sizeof(unsigned int), 1, fp_op));
		CHECK(n, fread((*wsk_op)->time, sizeof(char), n, fp_op));

		CHECK(1, fread(&n, sizeof(unsigned int), 1, fp_op));
		CHECK(n, fread((*wsk_op)->tytul, sizeof(char), n, fp_op));

		CHECK(1, fread(&n, sizeof(unsigned int), 1, fp_op));
		CHECK(n, fread((*wsk_op)->odbiorca.surname, sizeof(char), n, fp_op));

		CHECK(1, fread(&((*wsk_op)->value), sizeof(float), 1, fp_op));

		CHECK(1, fread(&((*wsk_op)->saldo), sizeof(float), 1, fp_op));

		wsk_op = &((*wsk_op)->next_op);
		n_read++;	
	}

	printf("**history uploaded**\n");
	return 0;
}

