bank_account : account.o files.o templates.o tests.o
	cc -o bank_account account.o files.o templates.o tests.o

account.o : account.c
	cc -c account.c
files.o : files.c files.h
	cc -c files.c
templates.o : templates.c templates.h
	cc -c templates.c
tests.o : tests.c
	cc -c tests.c

clean : rm bank_account account.o files.o templates.o tests.o

