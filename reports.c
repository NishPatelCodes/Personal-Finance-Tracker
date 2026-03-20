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
        int id = 0;
        char date[11];
        char category[30];
        char description[100];
        float amount = 0.0f;
        char type[10];

        if (sscanf(line, "%d,%10[^,],%29[^,],%99[^,],%f,%9s",
                   &id, date, category, description, &amount, type) != 6) {
            continue;
        }
    
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
        int id = 0;
        char date[11];
        char fieldsCategory[30];
        char description[100];
        float amount = 0.0f;
        char type[10];

        if (sscanf(line, "%d,%10[^,],%29[^,],%99[^,],%f,%9s",
                   &id, date, fieldsCategory, description, &amount, type) != 6) {
            continue;
        }

        if (strcmp(fieldsCategory, target_category) == 0) {

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