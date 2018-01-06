/*
 * files.h
 *
 *  Created on: 4 sty 2018
 *      Author: marcel
 */
#include "templates.h"

#ifndef FILES_H_
#define FILES_H_


#define KONTAKTY "contacts.dat"
#define HISTORIA "history.dat"

#define CHECK(LEN,F) {size_t k = F; if \
		( k != LEN){printf("linia: %d: \
		pobrano/zapisano %zu,  zamiast %d\n",\
		__LINE__, k, LEN); exit(EXIT_FAILURE);}}


void read_contacts(FILE *, dane **);
void read_history(FILE *, operacja **);
void read_file(dane **, operacja **);

void save_contacts(FILE *, dane *);
void save_history(FILE *, operacja *);
void save_and_free(operacja *, dane *);
void free_all(dane *, operacja *);


#endif /* FILES_H_ */
