/*
 *
 *(zrobione) ???jak zrobć, żeby w pliku zapisywać informację o historii, stanie konta, kontaktach itp, żeby można było zamknąć program, otworzyć go ponownie i nie stracić danych??(zrobione)
 *??logowanie sie do ssh na serwer??

 * dokumentacja:
 * unit_test: arg wywołania funkcji żeby szybciej odpalać tylko usuwanie(min 2)
 * obsługa błędu
 *#define ALOK_CHECK(wsk) {if(wsk == NULL){printf("Blad alokacji pamieci w lini: %d\n", __LINE__); exit(EXIT_FAILURE);}}
 *
 *
 *
 *potencjalne błędy:
 *-usuwanie pierwszego kontaktu
 *-wyszukiwanie kontaktu po nazwisku, którego nie ma
 *-nowy przelew kiedy nie ma zadnych kontaktow

 *funkcje do testowania
 *funkcja do usuwanie historii(zerowanie pliku)
 *funkcja do zerowania salda
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tests.h"
#include "files.h"
#include "templates.h"


int main(int arg_c, char *arg_v[])
{
	operacja *history = NULL;
	dane *kontakty = NULL;


	char choice;

	if(arg_c == 1)
	{
	printf("main\n");
	read_file(&kontakty, &history);
	printf("witaj w koncie bankowym!\n");
	printf("jaką operacje chcesz wykonac?\n\t\t\t %s\n", OPCJE);

	while((choice = getchar()) != 'q')
	{
		switch(choice)
		{
		case 'd': doladuj_konto(&history);
			break;
		case 'w': wysw_stan(history);
			break;
		case 'h': print_history(history);
			break;
		case 'p': nowy_przelew(&kontakty, &history);
			break;
		case 'n': dodaj_odbiorce(&kontakty, NEW);
			break;
		case 'k': print_list_odb(&kontakty, BIG);
			break;
		case 'e': edit_contacts(&kontakty);
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
		}else
			printf("use:\n<program> - normalne otwarcie programu\n"
							"<program> test1 - test usuniecia pierwszego kontaktu z listy\n"
							"<program> test2 - test sytuacji, gdy zaczynamy program z pustym plikiem\n");

		free_all(kontakty, history);
	}else if(arg_c > 2)
	{
		read_file(&kontakty, &history);
		printf("use:\n<program> - normalne otwarcie programu\n\\"
				"<program> test1 - test usuniecia pierwszego kontaktu z listy\n\\"
				"<program> test2 - test sytuacji, gdy zaczynamy program z pustym plikiem\n");
	}

	return 0;

}

void doladuj_konto(operacja **lista_op)
{
	float n = 0;
	float saldo = 0;
	printf("Wpisz wartosc o ile chcesz doladowac swoje konto:\n");
	scanf("%f", &n);

    time_t mytime;
    mytime = time(NULL);

	operacja **wsk_op = lista_op;
	while(*wsk_op != NULL)
	{
		saldo = (*wsk_op)->saldo;
		wsk_op = &((*wsk_op)->next_op);
	}
	*wsk_op =(operacja*) malloc(sizeof(operacja));
	strcpy((*wsk_op)->tytul, "Przelew zewnetrzny przychodzacy");
	(*wsk_op)->value = n;
	(*wsk_op)->saldo = saldo + n;
	strncpy((*wsk_op)->odbiorca.surname, "Kalinski", 9);			// ??czemu to nie działa??
	(*wsk_op)->next_op = NULL;
	strcpy((*wsk_op)->time, ctime(&mytime));
	printf("doladowano o %.2f zl\n", n);

}
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
void nowy_przelew(dane **lista_odb, operacja **lista_op)
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
		(*wsk_op)->odbiorca = *wsk_odb;	//znaleziona po nazwisku osoba jest przypisywana do operacji
		strncpy((*wsk_op)->tytul, tytul, strlen(tytul)+1);
		(*wsk_op)->value = n;
		(*wsk_op)->saldo = saldo - n;
		(*wsk_op)->next_op = NULL;
		strcpy((*wsk_op)->time, ctime(&mytime));
		printf("Odjeto %.2f zl z Twojego konta\n", n);
	}
}
void dodaj_odbiorce(dane **lista_odb, cont contact)
{
	char name[N];
	char surname[S];
	char street[S];
	int nr;
	char city[N];

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

	dane **wsk_odb = lista_odb;

	if(contact == NEW)
	{
		for(;  *wsk_odb != NULL;  wsk_odb = &((*wsk_odb)->next_odb) ){}
		*wsk_odb = (dane*) malloc(sizeof(dane));
	}
	strncpy((*wsk_odb)->name, name, strlen(name)+1);
	strncpy((*wsk_odb)->surname, surname, strlen(surname)+1);
	strncpy((*wsk_odb)->adress.city, city, strlen(city)+1);
	strncpy((*wsk_odb)->adress.street, street, strlen(street)+1);
	(*wsk_odb)->adress.nr = nr;
	(*wsk_odb)->next_odb = NULL;

}
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
void edit_contacts(dane **contacts)
{
	char choice;
	dane *wsk_odb;

	if(*contacts == NULL)
		printf("Nie masz jeszcze zadnych kontaktow\n");
	else
	{
		wsk_odb = *(find_cont(contacts));
		printf("aby usunac kontakt wcisnij 'd'\taby edytowac 'e'\n");

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
				dodaj_odbiorce(&wsk_odb, EXISTING);
				break;
			default: break;
		}
	}
}


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



