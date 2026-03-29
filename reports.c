/*
 * reports.c
 *
 * Handles all reporting, filtering, and search functionality.
 * Contains functions to generate overall summaries, filter transactions
 * by category or type, search by date, and export reports to a .txt file.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tellGCCthesefunctions.h"


// This function reads all transactions for a given user and displays an overall financial summary
// It calculates total income, total expenses, and net balance across all transactions
// Accepts a pointer to the username string to build the correct filename
void generate_overall_report(char *username) {

    // filename holds the transactions file e.g. "transactions_testuser.txt"
    // line holds the current line being read from the file
    // totalIncome, totalExpenses, netBalance accumulate the running totals
    char filename[100];
    char line[256];
    float totalIncome    = 0;
    float totalExpenses  = 0;
    float netBalance     = 0;

    // Build the filename using the username
    // e.g. username "testuser" → "transactions_testuser.txt"
    sprintf(filename, "transactions_%s.txt", username);

    // Open the file for reading "r"
    // If the file does not exist, there are no transactions to report on
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions found.\n");
        return;
    }

    // Read through the file line by line
    // fgets reads one line at a time and returns NULL when it reaches the end of the file
    while (fgets(line, sizeof(line), file) != NULL) {

        // Declare local variables to hold each parsed field of the transaction
        int id = 0;
        char date[11];
        char category[30];
        char description[100];
        float amount = 0.0f;
        char type[10];

        // sscanf parses the CSV line into individual fields
        // [^,] means read everything up to the next comma — allows spaces in descriptions
        // If parsing fails or the line doesn't have exactly 6 fields, skip it
        if (sscanf(line, "%d,%10[^,],%29[^,],%99[^,],%f,%9s",
                   &id, date, category, description, &amount, type) != 6) {
            continue;
        }

        // Add the amount to the correct running total based on the transaction type
        // We check both lowercase and uppercase versions since data may vary
        if (strcmp(type, "income") == 0 || strcmp(type, "Income") == 0) {
            totalIncome += amount;
        } else if (strcmp(type, "expense") == 0 || strcmp(type, "Expense") == 0) {
            totalExpenses += amount;
        }
    }
    fclose(file);   // always close the file after reading to free system resources

    // Calculate net balance and display the final summary
    netBalance = totalIncome - totalExpenses;
    printf("Total Income:   $%.2f\n", totalIncome);
    printf("Total Expenses: $%.2f\n", totalExpenses);
    printf("Net Balance:    $%.2f\n", netBalance);
}


// This function reads all transactions for a given user and displays a summary
// for one specific category only (e.g. Groceries, Rent, Coffee)
// Accepts a pointer to the username string and the category to filter by
void generate_category_report(char *username, char *category) {

    // filename holds the transactions file e.g. "transactions_testuser.txt"
    // line holds the current line being read from the file
    // target_category holds a copy of the category we are filtering by
    // totalIncome and totalExpenses accumulate running totals for this category only
    char filename[100];
    char line[256];
    char target_category[30];
    float totalIncome    = 0.0f;
    float totalExpenses  = 0.0f;

    // Copy the category into a local variable so we have our own copy to work with
    strcpy(target_category, category);

    // Build the filename using the username
    // e.g. username "testuser" → "transactions_testuser.txt"
    sprintf(filename, "transactions_%s.txt", username);

    // Open the file for reading "r"
    // If the file does not exist, there are no transactions to report on
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions found.\n");
        return;
    }

    // Read through the file line by line
    // fgets reads one line at a time and returns NULL when it reaches the end of the file
    while (fgets(line, sizeof(line), file) != NULL) {

        // Declare local variables to hold each parsed field of the transaction
        int id = 0;
        char date[11];
        char fieldsCategory[30];
        char description[100];
        float amount = 0.0f;
        char type[10];

        // sscanf parses the CSV line into individual fields
        // If parsing fails or the line doesn't have exactly 6 fields, skip it
        if (sscanf(line, "%d,%10[^,],%29[^,],%99[^,],%f,%9s",
                   &id, date, fieldsCategory, description, &amount, type) != 6) {
            continue;
        }

        // Only process this transaction if its category matches the target category
        // strcmp returns 0 if the two strings match exactly
        if (strcmp(fieldsCategory, target_category) == 0) {

            // Add the amount to the correct running total based on transaction type
            // We check both lowercase and uppercase versions since data may vary
            if (strcmp(type, "income") == 0 || strcmp(type, "Income") == 0) {
                totalIncome += amount;
            } else if (strcmp(type, "expense") == 0 || strcmp(type, "Expense") == 0) {
                totalExpenses += amount;
            }
        }
    }

    fclose(file);   // always close the file after reading to free system resources

    // Display the category summary
    // totalIncome - totalExpenses gives the net for this category
    printf("Category: %s\n", category);
    printf("  Total Income:   $%.2f\n", totalIncome);
    printf("  Total Expenses: $%.2f\n", totalExpenses);
    printf("  Net:            $%.2f\n", totalIncome - totalExpenses);
}


// This function generates an overall financial summary and exports it to a .txt file
// The output file is named report_<username>.txt
// Accepts a pointer to the username string to build the correct filenames
void export_report_to_txt(char *username) {

    // filename holds the transactions file e.g. "transactions_testuser.txt"
    // outputFile holds the report file e.g. "report_testuser.txt"
    // line holds the current line being read from the file
    // totalIncome, totalExpenses, netBalance accumulate the running totals
    char filename[100];
    char outputFile[120];
    char line[256];
    float totalIncome    = 0.0f;
    float totalExpenses  = 0.0f;
    float netBalance     = 0.0f;

    // Build the transactions filename using the username
    sprintf(filename, "transactions_%s.txt", username);

    // Open the transactions file for reading "r"
    // If the file does not exist, there are no transactions to export
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions found.\n");
        return;
    }

    // Read through the file line by line and accumulate totals
    // This is the same logic as generate_overall_report
    while (fgets(line, sizeof(line), file) != NULL) {

        int id = 0;
        char date[11];
        char category[30];
        char description[100];
        float amount = 0.0f;
        char type[10];

        // sscanf parses the CSV line into individual fields
        // If parsing fails or the line doesn't have exactly 6 fields, skip it
        if (sscanf(line, "%d,%10[^,],%29[^,],%99[^,],%f,%9s",
                   &id, date, category, description, &amount, type) != 6) {
            continue;
        }

        // Add the amount to the correct running total based on transaction type
        if (strcmp(type, "income") == 0 || strcmp(type, "Income") == 0) {
            totalIncome += amount;
        } else if (strcmp(type, "expense") == 0 || strcmp(type, "Expense") == 0) {
            totalExpenses += amount;
        }
    }
    fclose(file);   // close the transactions file after reading

    netBalance = totalIncome - totalExpenses;

    // Build the output report filename using the username
    // e.g. username "testuser" → "report_testuser.txt"
    sprintf(outputFile, "report_%s.txt", username);

    // Open the report file for writing "w"
    // This creates the file if it does not exist, or overwrites it if it does
    FILE *out = fopen(outputFile, "w");
    if (out == NULL) {
        printf("Error creating report file.\n");
        return;
    }

    // Write the formatted report to the output file
    fprintf(out, "Personal Finance Report\n");
    fprintf(out, "User: %s\n\n", username);
    fprintf(out, "Total Income:   $%.2f\n", totalIncome);
    fprintf(out, "Total Expenses: $%.2f\n", totalExpenses);
    fprintf(out, "Net Balance:    $%.2f\n", netBalance);
    fclose(out);    // always close the file after writing to free system resources

    printf("Report exported to %s\n", outputFile);
}


// This function searches for all transactions on a specific date and displays them in a table
// Uses an exact match on the date field (format YYYY-MM-DD)
// Accepts a pointer to the username string and the date to search for
void search_by_date(char *username, char *searchDate) {

    // filename holds the transactions file e.g. "transactions_testuser.txt"
    // line holds the current line being read from the file
    // found tracks whether any matching transactions were found
    char filename[100];
    char line[256];
    int found = 0;

    // Build the filename using the username
    sprintf(filename, "transactions_%s.txt", username);

    // Open the file for reading "r"
    // If the file does not exist, there are no transactions to search
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions file found.\n");
        return;
    }

    // Print the table header — same format as view_transactions
    printf("\nTransactions on date: %s\n", searchDate);
    printf("%-5s %-12s %-12s %-30s %-10s %-10s\n",
           "ID", "Date", "Category", "Description", "Amount", "Type");
    printf("------------------------------------------------------------------------\n");

    // Read through the file line by line looking for matching dates
    while (fgets(line, sizeof(line), file) != NULL) {

        int id = 0;
        char date[11];
        char category[30];
        char description[100];
        float amount = 0.0f;
        char type[10];

        // sscanf parses the CSV line into individual fields
        // If parsing fails or the line doesn't have exactly 6 fields, skip it
        if (sscanf(line, "%d,%10[^,],%29[^,],%99[^,],%f,%9s",
                   &id, date, category, description, &amount, type) != 6) {
            continue;
        }

        // Compare the transaction date to the search date
        // strcmp returns 0 if they match exactly
        if (strcmp(date, searchDate) == 0) {
            printf("%-5d %-12s %-12s %-30s %-10.2f %-10s\n",
                   id, date, category, description, amount, type);
            found = 1;  // at least one matching transaction was found
        }
    }

    fclose(file);   // always close the file after reading to free system resources

    // If no transactions matched, let the user know
    if (!found) {
        printf("(No matching transactions.)\n");
    }
}


// This function filters and displays all transactions belonging to a specific category
// Uses an exact match on the category field (e.g. Groceries, Rent, Coffee)
// Accepts a pointer to the username string and the category to filter by
void filter_by_category(char *username, char *category) {

    // filename holds the transactions file e.g. "transactions_testuser.txt"
    // line holds the current line being read from the file
    // found tracks whether any matching transactions were found
    char filename[100];
    char line[256];
    int found = 0;

    // Build the filename using the username
    sprintf(filename, "transactions_%s.txt", username);

    // Open the file for reading "r"
    // If the file does not exist, there are no transactions to filter
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions file found.\n");
        return;
    }

    // Print the table header — same format as view_transactions
    printf("\nTransactions in category: %s\n", category);
    printf("%-5s %-12s %-12s %-30s %-10s %-10s\n",
           "ID", "Date", "Category", "Description", "Amount", "Type");
    printf("------------------------------------------------------------------------\n");

    // Read through the file line by line looking for matching categories
    while (fgets(line, sizeof(line), file) != NULL) {

        int id = 0;
        char date[11];
        char cat[30];
        char description[100];
        float amount = 0.0f;
        char type[10];

        // sscanf parses the CSV line into individual fields
        // If parsing fails or the line doesn't have exactly 6 fields, skip it
        if (sscanf(line, "%d,%10[^,],%29[^,],%99[^,],%f,%9s",
                   &id, date, cat, description, &amount, type) != 6) {
            continue;
        }

        // Compare the transaction category to the filter category
        // strcmp returns 0 if they match exactly — this is case sensitive
        if (strcmp(cat, category) == 0) {
            printf("%-5d %-12s %-12s %-30s %-10.2f %-10s\n",
                   id, date, cat, description, amount, type);
            found = 1;  // at least one matching transaction was found
        }
    }

    fclose(file);   // always close the file after reading to free system resources

    // If no transactions matched, let the user know
    if (!found) {
        printf("(No matching transactions.)\n");
    }
}


// This function filters and displays all transactions of a specific type
// Type must be either "income" or "expense" (case insensitive)
// Accepts a pointer to the username string and the type to filter by
void filter_by_type(char *username, char *filterType) {

    // filename holds the transactions file e.g. "transactions_testuser.txt"
    // line holds the current line being read from the file
    // found tracks whether any matching transactions were found
    // wantIncome and wantExpense are flags set based on the filterType entered
    char filename[100];
    char line[256];
    int found      = 0;
    int wantIncome  = 0;
    int wantExpense = 0;

    // Determine which type the user is filtering for
    // We check both lowercase and uppercase versions since input may vary
    // If neither matches, the input is invalid — inform the user and exit
    if (strcmp(filterType, "income") == 0 || strcmp(filterType, "Income") == 0) {
        wantIncome = 1;
    } else if (strcmp(filterType, "expense") == 0 || strcmp(filterType, "Expense") == 0) {
        wantExpense = 1;
    } else {
        printf("Please type exactly: income  or  expense\n");
        return;
    }

    // Build the filename using the username
    sprintf(filename, "transactions_%s.txt", username);

    // Open the file for reading "r"
    // If the file does not exist, there are no transactions to filter
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions file found.\n");
        return;
    }

    // Print the table header — same format as view_transactions
    printf("\nTransactions of type: %s\n", filterType);
    printf("%-5s %-12s %-12s %-30s %-10s %-10s\n",
           "ID", "Date", "Category", "Description", "Amount", "Type");
    printf("------------------------------------------------------------------------\n");

    // Read through the file line by line looking for matching types
    while (fgets(line, sizeof(line), file) != NULL) {

        int id = 0;
        char date[11];
        char category[30];
        char description[100];
        float amount = 0.0f;
        char type[10];

        // sscanf parses the CSV line into individual fields
        // If parsing fails or the line doesn't have exactly 6 fields, skip it
        if (sscanf(line, "%d,%10[^,],%29[^,],%99[^,],%f,%9s",
                   &id, date, category, description, &amount, type) != 6) {
            continue;
        }

        // Check the transaction type against the flag set earlier
        // wantIncome = 1 means we are looking for income transactions
        // wantExpense = 1 means we are looking for expense transactions
        if (wantIncome &&
            (strcmp(type, "income") == 0 || strcmp(type, "Income") == 0)) {
            printf("%-5d %-12s %-12s %-30s %-10.2f %-10s\n",
                   id, date, category, description, amount, type);
            found = 1;  // at least one matching transaction was found
        } else if (wantExpense &&
                   (strcmp(type, "expense") == 0 || strcmp(type, "Expense") == 0)) {
            printf("%-5d %-12s %-12s %-30s %-10.2f %-10s\n",
                   id, date, category, description, amount, type);
            found = 1;  // at least one matching transaction was found
        }
    }

    fclose(file);   // always close the file after reading to free system resources

    // If no transactions matched, let the user know
    if (!found) {
        printf("(No matching transactions.)\n");
    }
}