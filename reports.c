/*
 * reports.c
 * 
 * Handles all reporting, filtering, and search functionality.
 * Contains functions to generate monthly summaries, filter transactions
 * by category or type, search by date, and export reports to a .txt file.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void  generate_overall_report(char *username){
    char filename[100];
    char line[256];
    char type[10];
    char date[11];
    char category[30];
    char description[100];
    float amount;
    int currentId = 0;
    float totalIncome = 0;
    float totalExpenses = 0;
    float netBalance = 0;

    sprintf(filename, "transactions_%s.txt", username);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions found.\n");
        return;
    }

    
    while (fgets(line, sizeof(line), file) != NULL) {
        char fields[5][100];
        int pos = 0, col = 0, start = 0;
    
        // Split line by commas into 5 fields
        while (line[pos] != '\0' && line[pos] != '\n' && line[pos] != '\r') {
            if (line[pos] == ',') {
                fields[col][pos - start] = '\0';
                col++;
                start = pos + 1;
            } else {
                fields[col][pos - start] = line[pos];
            }
            pos++;
        }
        fields[col][pos - start] = '\0';

        float amount = (float)atof(fields[3]);
        char *type = fields[4];
    
        // compare case-insensitively to \"income\" / \"expense\"
        if (strcmp(type, "income") == 0 || strcmp(type, "Income") == 0) {
            totalIncome += amount;
        } else if (strcmp(type, "expense") == 0 || strcmp(type, "Expense") == 0) {
            totalExpenses += amount;
        }
    }
    fclose(file);
    netBalance = totalIncome - totalExpenses;
    printf("Total Income: $%.2f\n", totalIncome);
    printf("Total Expenses: $%.2f\n", totalExpenses);
    printf("Net Balance: $%.2f\n", netBalance);
}

void generate_category_report(char *username, char *category) {
    char filename[100];
    char line[256];
    char target_category[30];
    float totalIncome = 0.0f;
    float totalExpenses = 0.0f;

    strcpy(target_category, category);

    sprintf(filename, "transactions_%s.txt", username);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions found.\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char fields[5][100];
        int pos = 0, col = 0, start = 0;

        // Split line by commas into 5 fields
        while (line[pos] != '\0' && line[pos] != '\n' && line[pos] != '\r') {
            if (line[pos] == ',') {
                fields[col][pos - start] = '\0';
                col++;
                start = pos + 1;
            } else {
                fields[col][pos - start] = line[pos];
            }
            pos++;
        }
        fields[col][pos - start] = '\0';

        // fields[1] = category, fields[3] = amount, fields[4] = type
        if (strcmp(fields[1], target_category) == 0) {
            float amount = (float)atof(fields[3]);
            char *type = fields[4];

            if (strcmp(type, "income") == 0 || strcmp(type, "Income") == 0) {
                totalIncome += amount;
            } else if (strcmp(type, "expense") == 0 || strcmp(type, "Expense") == 0) {
                totalExpenses += amount;
            }
        }
    }

    fclose(file);

    printf("Category: %s\n", category);
    printf("  Total Income:   $%.2f\n", totalIncome);
    printf("  Total Expenses: $%.2f\n", totalExpenses);
    printf("  Net:            $%.2f\n", totalIncome - totalExpenses);
}