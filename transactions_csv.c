/* transactions_csv.c
 * CSV parsing/writing utilities for:
 *   id,date,category,description,amount,type
 */

#include <stdio.h>
#include <string.h>

#include "tellGCCthesefunctions.h"

void sanitize_description(char *s) {
    // CSV safety: descriptions must not contain commas.
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == ',') s[i] = ' ';
    }
}

int parse_transaction_line(const char *line, Transaction *t) {
    // Use sscanf with [^,] so description can contain spaces (but not commas).
    return sscanf(line,
                   "%d,%10[^,],%29[^,],%99[^,],%f,%9s",
                   &t->id, t->date, t->category, t->description, &t->amount, t->type) == 6;
}

void write_transaction_line(FILE *f, const Transaction *t) {
    fprintf(f, "%d,%s,%s,%s,%.2f,%s\n",
            t->id, t->date, t->category, t->description, t->amount, t->type);
}

int transactions_next_id(const char *username) {
    char filename[100];
    sprintf(filename, "transactions_%s.txt", username);

    FILE *file = fopen(filename, "r");
    if (file == NULL) return 1;

    char line[256];
    int maxId = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        Transaction t;
        if (parse_transaction_line(line, &t)) {
            if (t.id > maxId) maxId = t.id;
        }
    }

    fclose(file);
    return maxId + 1;
}

