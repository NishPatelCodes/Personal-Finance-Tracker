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

#include "tellGCCthesefunctions.h"

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

/*
 * Export overall summary to a text file.
 * Output file format: report_<username>.txt
 */
void export_report_to_txt(char *username) {
    char filename[100];
    char outputFile[120];
    char line[256];
    float totalIncome = 0.0f;
    float totalExpenses = 0.0f;
    float netBalance = 0.0f;

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

        if (strcmp(type, "income") == 0 || strcmp(type, "Income") == 0) {
            totalIncome += amount;
        } else if (strcmp(type, "expense") == 0 || strcmp(type, "Expense") == 0) {
            totalExpenses += amount;
        }
    }
    fclose(file);

    netBalance = totalIncome - totalExpenses;

    sprintf(outputFile, "report_%s.txt", username);
    FILE *out = fopen(outputFile, "w");
    if (out == NULL) {
        printf("Error creating report file.\n");
        return;
    }

    fprintf(out, "Personal Finance Report\n");
    fprintf(out, "User: %s\n\n", username);
    fprintf(out, "Total Income:   $%.2f\n", totalIncome);
    fprintf(out, "Total Expenses: $%.2f\n", totalExpenses);
    fprintf(out, "Net Balance:    $%.2f\n", netBalance);
    fclose(out);

    printf("Report exported to %s\n", outputFile);
}

/*
 * Show every transaction on a given date (exact match YYYY-MM-DD).
 */
void search_by_date(char *username, char *searchDate) {
    char filename[100];
    char line[256];
    int found = 0;

    sprintf(filename, "transactions_%s.txt", username);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions file found.\n");
        return;
    }

    printf("\nTransactions on date: %s\n", searchDate);
    printf("%-5s %-12s %-12s %-30s %-10s %-10s\n",
           "ID", "Date", "Category", "Description", "Amount", "Type");
    printf("------------------------------------------------------------------------\n");

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

        if (strcmp(date, searchDate) == 0) {
            printf("%-5d %-12s %-12s %-30s %-10.2f %-10s\n",
                   id, date, category, description, amount, type);
            found = 1;
        }
    }

    fclose(file);

    if (!found) {
        printf("(No matching transactions.)\n");
    }
}

/*
 * Show every transaction in one category (exact spelling, e.g. Groceries).
 */
void filter_by_category(char *username, char *category) {
    char filename[100];
    char line[256];
    int found = 0;

    sprintf(filename, "transactions_%s.txt", username);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions file found.\n");
        return;
    }

    printf("\nTransactions in category: %s\n", category);
    printf("%-5s %-12s %-12s %-30s %-10s %-10s\n",
           "ID", "Date", "Category", "Description", "Amount", "Type");
    printf("------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), file) != NULL) {
        int id = 0;
        char date[11];
        char cat[30];
        char description[100];
        float amount = 0.0f;
        char type[10];

        if (sscanf(line, "%d,%10[^,],%29[^,],%99[^,],%f,%9s",
                   &id, date, cat, description, &amount, type) != 6) {
            continue;
        }

        if (strcmp(cat, category) == 0) {
            printf("%-5d %-12s %-12s %-30s %-10.2f %-10s\n",
                   id, date, cat, description, amount, type);
            found = 1;
        }
    }

    fclose(file);

    if (!found) {
        printf("(No matching transactions.)\n");
    }
}

/*
 * Show every transaction of one type: income or expense (same rules as elsewhere).
 */
void filter_by_type(char *username, char *filterType) {
    char filename[100];
    char line[256];
    int found = 0;
    int wantIncome = 0;
    int wantExpense = 0;

    if (strcmp(filterType, "income") == 0 || strcmp(filterType, "Income") == 0) {
        wantIncome = 1;
    } else if (strcmp(filterType, "expense") == 0 || strcmp(filterType, "Expense") == 0) {
        wantExpense = 1;
    } else {
        printf("Please type exactly: income  or  expense\n");
        return;
    }

    sprintf(filename, "transactions_%s.txt", username);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions file found.\n");
        return;
    }

    printf("\nTransactions of type: %s\n", filterType);
    printf("%-5s %-12s %-12s %-30s %-10s %-10s\n",
           "ID", "Date", "Category", "Description", "Amount", "Type");
    printf("------------------------------------------------------------------------\n");

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

        if (wantIncome &&
            (strcmp(type, "income") == 0 || strcmp(type, "Income") == 0)) {
            printf("%-5d %-12s %-12s %-30s %-10.2f %-10s\n",
                   id, date, category, description, amount, type);
            found = 1;
        } else if (wantExpense &&
                   (strcmp(type, "expense") == 0 || strcmp(type, "Expense") == 0)) {
            printf("%-5d %-12s %-12s %-30s %-10.2f %-10s\n",
                   id, date, category, description, amount, type);
            found = 1;
        }
    }

    fclose(file);

    if (!found) {
        printf("(No matching transactions.)\n");
    }
}