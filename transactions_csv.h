/* transactions_csv.h
 * CSV parsing/writing utilities for the transactions files:
 *   id,date,category,description,amount,type
 */
#ifndef TRANSACTIONS_CSV_H
#define TRANSACTIONS_CSV_H

#include <stdio.h>

typedef struct {
    int id;
    char date[11];            /* YYYY-MM-DD (10) + null terminator */
    char category[30];
    char description[100];
    float amount;
    char type[10];           /* income | expense */
} Transaction;

/* Replaces commas inside description (CSV safety). */
void sanitize_description(char *s);

/* Parses one CSV line into a Transaction. Returns 1 on success, 0 otherwise. */
int parse_transaction_line(const char *line, Transaction *t);

/* Writes one Transaction to CSV. */
void write_transaction_line(FILE *f, const Transaction *t);

/* Finds next available ID by scanning the file for max stored ID. */
int transactions_next_id(const char *username);

#endif /* TRANSACTIONS_CSV_H */

