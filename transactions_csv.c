/* transactions_csv.c
 *
 * CSV parsing and writing utilities for transaction files.
 * All transaction lines follow this format:
 *   id,date,category,description,amount,type
 */

#include <stdio.h>
#include <string.h>

#include "tellGCCthesefunctions.h"


// Replaces commas in a description with spaces to preserve CSV structure
void sanitize_description(char *s) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] == ',')
            s[i] = ' ';
    }
}


// Parses a single CSV line into a Transaction struct
// Returns 1 on success, 0 if the line is malformed or missing fields
int parse_transaction_line(const char *line, Transaction *t) {
    return sscanf(line,
                  "%d,%10[^,],%29[^,],%99[^,],%f,%9s",
                  &t->id, t->date, t->category, t->description, &t->amount, t->type) == 6;
}


// Writes a Transaction struct to a file as a formatted CSV line
void write_transaction_line(FILE *f, const Transaction *t) {
    fprintf(f, "%d,%s,%s,%s,%.2f,%s\n",
            t->id, t->date, t->category, t->description, t->amount, t->type);
}


// Scans the user's transaction file and returns the next available ID
// Returns 1 if the file does not exist yet
int transactions_next_id(const char *username) {
    char filename[100];
    sprintf(filename, "transactions_%s.txt", username);

    FILE *file = fopen(filename, "r");
    if (file == NULL) return 1;

    char line[256];
    int maxId = 0;

    // Read through all lines and track the highest ID found
    while (fgets(line, sizeof(line), file) != NULL) {
        Transaction t;
        if (parse_transaction_line(line, &t)) {
            if (t.id > maxId) maxId = t.id;
        }
    }

    fclose(file);
    return maxId + 1;  // next ID is one above the current highest
}