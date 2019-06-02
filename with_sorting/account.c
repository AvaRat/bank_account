

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "tests.h"
#include "files.h"
#include "templates.h"
/*!
*	\author Marcel Kaliński
*	\bug maksymalna długość imion (ze spacjami)wynosi 30 a innych nazw 40
*	\warning przekroczenie niektórych z tych wartości spowoduje wyłożenie się programu
*	\warning pliki z bazą danych (kontakty i historia operacji) muszą znajdować się w tym samym folderze co plik wykonywalny
*/

/*!
* \fn int main(int arg_c, char *arg_v[])
*\brief funkcja główna
*@param[in] arg_c liczba argumentów wywołania programu
*@param[in] *arg_v[] tablica wskaźników do argumentów wywołania programu
*@retval 0 jeśli funkcja zakończyła się powodzeniem
*/
int main(int arg_c, char *arg_v[])
{
	operacja *history = NULL;	/*!< pusty wskaźnik do pszyszłej listy operacji*/
	dane *kontakty = NULL;	/*!< pusty wskaźnik do pszyszłej listy kontaktów*/


	char choice;	//!< pobierana od użytkownika litera decydująca o konkretnym działaniu

	if(arg_c == 1)
	{
		printf("main\n");
		if(read_file(&kontakty, &history) != 0)
		{
			printf("Blad przy wczytywaniu bazy danych!!\n");
			return -1;
		}
		printf("witaj w koncie bankowym!\n");
		printf("jaką operacje chcesz wykonac?\n\t\t\t %s\n", OPCJE);

		while((choice = getchar()) != 'q')
		{
			switch(choice)
			{
			case 'd': FUN_M_CHECK(doladuj_konto(&history, REAL))
				break;
			case 'w': wysw_stan(history);
				break;
			case 'h':  print_history(history);
				break;
			case 'p':  FUN_M_CHECK(nowy_przelew(&kontakty, &history))
				break;
			case 'n':  FUN_M_CHECK(dodaj_odbiorce(&kontakty, NEW, REAL))
				break;
			case 'k': print_sorted(kontakty);
			// print_list_odb(&kontakty, BIG);
				break;
			case 'e':  FUN_M_CHECK(edit_contacts(&kontakty))
				break;
			default: printf("%s\n", OPCJE);
				break;
			}
		}
		save_and_free(history, kontakty);
	}else if(arg_c == 2)
	{
		if(strcmp(arg_v[1], "test1") == 0)
		{
			printf("test1\n");
			usun_pierwszy(&kontakty);
		}else if(strcmp(arg_v[1], "test2") == 0)
		{
			printf("test2\n");
			from_scratch(&kontakty, &history);
		}else if(strcmp(arg_v[1], "test3") == 0)
		{
			printf("test3\n");
			add_a_lot(&kontakty, &history);
		}else
			printf("use:\n<program> - normalne otwarcie programu\n"
							"<program> test1 - test usuniecia pierwszego kontaktu z listy\n"
							"<program> test2 - test sytuacji, gdy zaczynamy program z pustym plikiem\n");

		printf("zwalnianie pamieci\n");
		getchar();
		free_all(kontakty, history);
	}else if(arg_c > 2)
	{
		printf("use:\n<program> - normalne otwarcie programu\n\\"
				"<program> test1 - test usuniecia pierwszego kontaktu z listy\n\\"
				"<program> test2 - test sytuacji, gdy zaczynamy program z pustym plikiem\n");
	}

	return 0;

}

/*!
* \fn int doladuj_konto(operacja **lista_op, test use)
*	\brief doładowuje konto
*	\details alokuje nową operację doładowania o wartości wprowadzonej przez użytownika a jako nazwisko odbiorcy podaje nazwisko autora
*	\param[in] **lista_op	podwójny wskaźnik na listę operacji
*	\param[in] use	determinuje, czy funkcja jest użyta w ramach testu, czy we właściwym programie
*	\retval 0	kiedy zakończy działanie sukcesem
*	\retval -1	kiedy nastąpi jakiś błąd
*/
int doladuj_konto(operacja **lista_op, test use)
{
	float n = 0;
	float saldo = 0;
	if(use == REAL)
	{
		printf("Wpisz wartosc o ile chcesz doladowac swoje konto:\n");
		scanf("%f", &n);
	}else if(use == TEST)
		n = 5;

    time_t mytime;
    mytime = time(NULL);

	operacja **wsk_op = lista_op;
	while(*wsk_op != NULL)
	{
		saldo = (*wsk_op)->saldo;
		wsk_op = &((*wsk_op)->next_op);
	}
	*wsk_op =(operacja*) malloc(sizeof(operacja));
	ALOK_CHECK(*wsk_op)
	strcpy((*wsk_op)->tytul, "Przelew zewnetrzny przychodzacy");
	(*wsk_op)->value = n;
	(*wsk_op)->saldo = saldo + n;
	strncpy((*wsk_op)->odbiorca.surname, "Kalinski", 9);
	(*wsk_op)->next_op = NULL;
	strcpy((*wsk_op)->time, ctime(&mytime));
	printf("doladowano o %.2f zl\n", n);
	return 0;
}
/*!
* \fn void wysw_stan(operacja *lista_op)
*	\brief wyświetla stan konta w danym momencie
*	\param[in]	*lista_op	wskaźnik na listę operacji
*/
void wysw_stan(operacja *lista_op)
{
	if(lista_op == NULL)
		printf("Nie dokonales jeszcze zadnych operacji\n");
	else
	{
		operacja *wsk_op=lista_op;
		for(;  wsk_op->next_op!=NULL;  wsk_op=wsk_op->next_op) {}
		printf("Stan Twojego konta na dzien %s %s:\n\t\t%.2f zl\n",__DATE__, __TIME__, wsk_op->saldo);
	}
}
/*!
* \fn int int nowy_przelew(dane **lista_odb, operacja **lista_op)
*	\brief robi nowy przelew
*	\details	pobiera od użytkownika odpowiednie informacje po czym alokuje kolejną operacja na końcu listy
*	\param[in]	**lista_odb	podwójny wskaźnik na listę kontaktów
*	\param[in]	**lista_op	podwójny wskaźnik na listę operacji
*	\retval 0 dla zakończenia z sukcesem
*	\retval -1 dla zakończenia z błedem
*/
int nowy_przelew(dane **lista_odb, operacja **lista_op)
{
	float n;
	char tytul[S];

    time_t mytime;
    mytime = time(NULL);
	if((*lista_odb) == NULL)
	{
		printf("Musisz najpierw dodac osobe do kontaktow\n");
	}else
	{

		dane *wsk_odb;

		wsk_odb = *(find_cont(lista_odb));

		printf("Dane odbiorcy\nimie: %s\nnazwisko: %s\nadres: %s %d %s\n", wsk_odb->name, wsk_odb->surname, wsk_odb->adress.street, wsk_odb->adress.nr,  wsk_odb->adress.city);

		printf("podaj tytul przelewu: \n");
		get_data(tytul);

		printf("Podaj kwote:\n");
		scanf("%f", &n);
		operacja **wsk_op = lista_op;
		float saldo = 0;
		while(*wsk_op != NULL)
		{
			saldo = (*wsk_op)->saldo;
			wsk_op = &((*wsk_op)->next_op);
		}
		*wsk_op =(operacja *) malloc(sizeof(operacja));
		ALOK_CHECK(*wsk_op)
		(*wsk_op)->odbiorca = *wsk_odb;	//znaleziona po nazwisku osoba jest przypisywana do operacji
		strncpy((*wsk_op)->tytul, tytul, strlen(tytul)+1);
		(*wsk_op)->value = n;
		(*wsk_op)->saldo = saldo - n;
		(*wsk_op)->next_op = NULL;
		strcpy((*wsk_op)->time, ctime(&mytime));
		printf("Odjeto %.2f zl z Twojego konta\n", n);
	}
	return 0;
}
/*!
* \fn	int dodaj_odbiorce(dane **lista_odb, what_cont contact, test use)
* \details funkcja ma dwa główne tryby: 
*					1)dodawanie nowego odbiorcy
*					2)edycja już istniejącego kontaktu
*	przy dodawaniu nowego następuje alokacja kontaktu na końcu listy a w przypadku edycji tylko zmiana wartości i nazw odpowiednich zmiennych
*	funkcję można również wywołac w ramach testu i wtedy nie będzie ona pobierała od użytkownika żadnych wartości, tylko nadpisze je z góry.
*	\param[in] **lista_odb	podwójny wskaźnik na listę kontaktów
*	\param[in] contact	wartość decydująca, czy funkcja ma tworzyć nowy kontakt, czy edytować istniejący
*	\param[in] use	wartość decydująca, czy funkcja działa w ramach testu, czy nie
*	\retval 0 dla zakończenia z sukcesem
*	\retval -1 dla zakończenia z błedem
*/
int dodaj_odbiorce(dane **lista_odb, what_cont contact, test use)
{
	char name[N];
	char surname[S];
	char street[S];
	int nr;
	char city[N];

	if(use == REAL)
	{
		printf("podaj imie: ");
		scanf("%s", name);

		printf("podaj nazwisko: ");
		scanf("%s", surname);

		printf("podaj ulice: ");
		get_data(street);

		printf("podaj nr domu: ");
		scanf("%d", &nr);

		printf("miasto: ");
		get_data(city);
	}else if(use == TEST)
		{
			strcpy(name,"test_name");
			strcpy(surname,"test_sur");
			strcpy(street, "test_street");
			strcpy(city, "test_city");
			nr = 5;
		}

	dane **wsk_odb = lista_odb;

	if(contact == NEW)
	{
		for(;  *wsk_odb != NULL;  wsk_odb = &((*wsk_odb)->next_odb) ){}
		*wsk_odb = (dane*) malloc(sizeof(dane));
		ALOK_CHECK(*wsk_odb)
		(*wsk_odb)->next_odb = NULL;
	}
	strncpy((*wsk_odb)->name, name, strlen(name)+1);
	strncpy((*wsk_odb)->surname, surname, strlen(surname)+1);
	strncpy((*wsk_odb)->adress.city, city, strlen(city)+1);
	strncpy((*wsk_odb)->adress.street, street, strlen(street)+1);
	(*wsk_odb)->adress.nr = nr;

	return 0;
}
/*!
* \fn	dane **find_cont(dane **kontakty)
* \brief funkcja pobiera podwójny wskaźnik tylko dlatego, żeby móc... w sumie to chyba wystarczyłby jej jeden wskaźnik??
* \details funkcja działa tak, że najpierw wyświetla listę kontaktów wraz z ich numerami a potem pobiera numer wskazany przez niego.					
* \param[in]	**kontakty	podwójny wskaźnik na listę kontaktów
* \retval wsk	podwójny wskaźnik na szukany kontakt
*/
dane **find_cont(dane **kontakty)
{
	unsigned int choice=0, n=0;
	printf("Wpisz numer odpowiedniego kontaktu\n");
	print_list_odb(kontakty, SMALL);
	dane **wsk = kontakty;
	for(n=1; (*wsk)!=NULL;  n++, wsk=&((*wsk)->next_odb)){}
    while(scanf("%d",&choice) != 1 || choice<1 || choice >n)
    {
        printf("Wprowadz cyfre od 1 do %d\n", n);
        while(getchar() != '\n');
    }
	for(n=1, wsk=kontakty;  n!=choice && (*wsk)!=NULL;  n++, wsk=&((*wsk)->next_odb)){}
	return wsk;
}
/*!
* \fn int cmp_names(const void *l, const void *r)
* \brief funkcja porownujaca imiona do sortowania
*/
int cmp_names(const void *l, const void *r)
{
	dane **left = (dane **) l;
	dane **right = (dane **) r;
	char l_ch = tolower((*left)->name[0]);
	char r_ch = tolower((*right)->name[0]);
	if(l_ch < r_ch)
		return -1;
	else if(l_ch > r_ch)
		return 1;
	else 
		return 0;
}
/*!
* \fn int cmp_surnames(const void *l, const void *r)
* \brief funkcja porownujaca nazwiska do sortowania
*/
int cmp_surnames(const void *l, const void *r)
{
	dane **left = (dane **) l;
	dane **right = (dane **) r;
	char l_ch = tolower((*left)->surname[0]);
	char r_ch = tolower((*right)->surname[0]);
	if(l_ch < r_ch)
		return -1;
	else if(l_ch > r_ch)
		return 1;
	else
		return 0;
}
/*!
* \fn int cmp_address(const void *l, const void *r)
* \brief funkcja porownujaca miasto zamieszkania do sortowania
*/
int cmp_address(const void *l, const void *r)
{
	dane **left = (dane **) l;
	dane **right = (dane **) r;
	char l_ch = tolower((*left)->adress.city[0]);
	char r_ch = tolower((*right)->adress.city[0]);
	if(l_ch < r_ch)
		return -1;
	else if(l_ch > r_ch)
		return 1;
	else
		return 0;
}
/*!
* \fn	void print_sorted(dane *contacts)
* \details wyswietla liste kontakow posortowaną w wybrany przez uzytkownika sposob
* \param[in] *contacts lista kontaktów
*/
void print_sorted(dane *contacts)
{
	dane *wsk_odb = contacts;
	int cont_size=0;
	for(; wsk_odb!=NULL; wsk_odb=wsk_odb->next_odb)
		cont_size++;
	dane **array_odb = malloc(sizeof(dane*) * cont_size);
	int i=0;
	for(wsk_odb=contacts; wsk_odb!=NULL; wsk_odb=wsk_odb->next_odb)
		array_odb[i++] = wsk_odb;
	if((contacts) == NULL)
	{
		printf("nie masz zadnych kontaktow!!!\n");
	}
	printf("Sortuj wg: 'i' -> Imie\t'n' -> Nazwisko\t'm' -> Miasto\n");
	char ch;
	getchar();
	ch = getchar();
	switch(ch){
		case 'i': qsort(array_odb, cont_size, sizeof(wsk_odb), cmp_names);
			printf("\t**sortowanie wg imienia\n");
			break;
		case 'n': qsort(array_odb, cont_size, sizeof(wsk_odb), cmp_surnames);
			printf("\t**sortowanie wg nazwiska\n");
			break;
		case 'm': qsort(array_odb, cont_size, sizeof(wsk_odb), cmp_address);
			printf("\t**sortowanie wg miasta\n");
			break;
		default: printf("Nie m takiej opcji\n'i' -> Imie\t'n' -> Nazwisko\t'm' -> Miasto\n");
			break;
	}
	for(i=0; i<cont_size; i++)
	{
		wsk_odb = array_odb[i];
		printf("Imie: %s\n", wsk_odb->name);
		printf("Nazwisko: %s\n", wsk_odb->surname);
		printf("Adres: ul. %s %d %s\n\n", wsk_odb->adress.street, wsk_odb->adress.nr, wsk_odb->adress.city);
	}
	free(array_odb);
}

/*!
* \fn	void print_list_odb(dane **contacts, what_list list)
* \details wyswietla liste kontaktow w dwuch trybach:
*				1)mała lista - tylko imiona i nazwiska (stworzona specjalnie pod funkcję find_cont())
*				2)duża lista - wyświetla pełną listę wraz ze wszystkimi szczegułami
* \param[in] **contacts lista kontaktów
* \param[in] list określa tryb działania funkcji
*/
void print_list_odb(dane **contacts, what_list list)
{
	dane *wsk_odb = *contacts;
	if((*contacts) == NULL)
	{
		printf("nie masz zadnych kontaktow!!!\n");
	}

	if(list == BIG)
	{
		for(;  wsk_odb!=NULL;  wsk_odb=wsk_odb->next_odb)
		{
			printf("Imie: %s\n", wsk_odb->name);
			printf("Nazwisko: %s\n", wsk_odb->surname);
			printf("Adres: ul. %s %d %s\n\n", wsk_odb->adress.street, wsk_odb->adress.nr, wsk_odb->adress.city);
		}
	}else
	{
		unsigned int n = 1;
		for(;  wsk_odb!=NULL;  wsk_odb=wsk_odb->next_odb, n++)
			printf("%d: %s %s\n", n, wsk_odb->name, wsk_odb->surname);
	}
}
/*!
* \fn	int edit_contacts(dane **contacts)
* \brief główna funkcja odpowiedzialna za edycję kontaktów
* \param[in] **contacts	lista kontaktów
* \retval	0 przy zakończeniu sukcesem
* \retval	-1 przy błędzie
*/
int edit_contacts(dane **contacts)
{
	char choice;
	dane *wsk_odb;

	if(*contacts == NULL)
		printf("Nie masz jeszcze zadnych kontaktow\n");
	else
	{
		wsk_odb = *(find_cont(contacts));
		printf("aby usunac kontakt wcisnij 'd'\taby edytowac 'e'\nnacisniej enter aby wrocic do menu\n");

		if(getchar()=='\n')
		{
			choice = getchar();
		}else choice = getchar();

		switch(choice)
		{
			case 'd':
				delete_contact(contacts, wsk_odb);
				break;

			case 'e':
				FUN_CHECK(dodaj_odbiorce(&wsk_odb, EXISTING, REAL))
				break;
			default: break;
		}
	}
	return 0;
}

/*!
* \fn	void delete_contact(dane ** contacts, dane *wanted)
* \brief	usuwa kontakt z listy jednokierukowej
* \details	zwalnia pamięć zajmowaną przez kontakt i odpowiednio manipuluje sąsiednimi elementami z listy.
* \param[in]	**contacts	lista kontaktów
* \param[in]	*wanted	wskaźnik na kontakt, który chcemy usunąć
*/
void delete_contact(dane ** contacts, dane *wanted)
{
	dane **previous = contacts, *wsk = *contacts;
	if(wanted !=*contacts)
	{
		for(;;)
		{
			printf("OK\n");
			wsk = wsk->next_odb;
			if(wsk == wanted)
				break;
			else
				previous = &((*previous)->next_odb);

		}
		(*previous)->next_odb = wsk->next_odb;
		free(wsk);
		printf("usunieto kontakt");
	}else
	{
		(*contacts) = (*contacts)->next_odb;
		free(wanted);
	}

}

/*!
* \fn	void print_history(operacja *op)
* \brief wyświetla historię operacji
* \param[in]	*op	wskaźnik na historię operacji
*/
void print_history(operacja *op)
{
	operacja * wsk_op = op;
	if(wsk_op == NULL)
	{
		printf("nie ma jeszcze zadnych operacji!!\n");
	}
	for(;  wsk_op!=NULL;  wsk_op=wsk_op->next_op)
	{
		printf("%s", wsk_op->time);
		printf("odbiorca: %s\n", wsk_op->odbiorca.surname);
		printf("tytul: %s\n", wsk_op->tytul);
		printf("wartosc: %.2f\n", wsk_op->value);
		printf("saldo po operacji: %.2f\n\n", wsk_op->saldo);
	}
}
/*!
* \fn	void get_data(char *word)
* \details pobiera od użytkownika słowa, doputy dopóki nie napotka znaku '\n' o ile jest ono mniejsze niż 40 znaków
* \param[in,out]	*word	wskaźnik na tablicę gdzie mają być umieszczone pobrane słowa
*/
void get_data(char *word)
{
	unsigned int x;
	char ch;

	getchar();
	for(x=0;  (ch=getchar())!='\n' && x<=S;  x++)
	{
		word[x] = ch;
	}
	word[x] = '\0';
}



