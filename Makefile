CC = gcc
CFLAGS = -Wall -Wextra -pedantic

SRCS = main.c auth.c transactions.c transactions_csv.c encryption.c reports.c
OBJS = main.o auth.o transactions.o transactions_csv.o encryption.o reports.o


all: main


main: $(OBJS)
	$(CC) $(CFLAGS) -o main $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

auth.o: auth.c
	$(CC) $(CFLAGS) -c auth.c

transactions.o: transactions.c
	$(CC) $(CFLAGS) -c transactions.c

transactions_csv.o: transactions_csv.c
	$(CC) $(CFLAGS) -c transactions_csv.c

encryption.o: encryption.c
	$(CC) $(CFLAGS) -c encryption.c

reports.o: reports.c
	$(CC) $(CFLAGS) -c reports.c


run: main
	./main


clean:
	rm -f main $(OBJS)