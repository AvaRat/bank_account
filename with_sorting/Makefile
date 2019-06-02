bank_account : account.o files.o tests.o
	cc -o account account.o files.o tests.o

account.o : account.c
	cc -c account.c
files.o : files.c files.h
	cc -c files.c
tests.o : tests.c
	cc -c tests.c

clean : 
	rm account *.o

