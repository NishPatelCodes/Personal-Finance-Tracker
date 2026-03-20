/*
 * transactions.c
 * 
 * Handles all transaction file operations (CRUD).
 * Contains functions to add, delete, update, and view
 * income/expense transactions stored in the user's .txt file.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "transactions_csv.h"

static void trim_newlines(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
        len--;
    }
}

static void read_line_input(const char *prompt, char *out, size_t outSize) {
    printf("%s", prompt);
    if (fgets(out, (int)outSize, stdin) == NULL) {
        out[0] = '\0';
        return;
    }
    trim_newlines(out);
}


// This function prompts the user for transaction details and appends it to their transactions file
// It accepts a pointer to the username string to build the correct filename
void add_transaction(char *username) {

    // Variables to hold each field of the transaction
    // date is 11 chars: 10 for "YYYY-MM-DD" + 1 for null terminator
    char date[11];
    char category[30];
    char description[100];
    float amount;
    char type[10];  // either "income" or "expense"


    // Prompt the user for each transaction field one at a time

    // Date — scanf reads a single word (no spaces), stops at newline
    printf("Date (YYYY-MM-DD): ");
    scanf("%s", date);
    while (getchar() != '\n');  // Clear the leftover '\n' from the input buffer
                                // Without this, the next input call would read the '\n' instead of waiting

    // Category — scanf reads a single word (no spaces), stops at newline
    printf("Category (Food, Rent, etc): ");
    scanf("%s", category);
    while (getchar() != '\n');  // Clear input buffer after scanf


    // Description — fgets is used instead of scanf to allow spaces in the input
    // e.g. "Monthly Rent" would be cut to "Monthly" with scanf
    printf("Description: ");
    fgets(description, sizeof(description), stdin);

    // fgets includes the '\n' character at the end of the string, so we strip it
    // by replacing it with a null terminator '\0'
    int len = strlen(description);
    if (description[len - 1] == '\n')
        description[len - 1] = '\0';

    sanitize_description(description);

    // Amount — scanf reads a float value
    printf("Amount: ");
    scanf("%f", &amount);
    while (getchar() != '\n');  // Clear input buffer after scanf


    // Type — either "income" or "expense"
    printf("Type (income/expense): ");
    scanf("%s", type);
    while (getchar() != '\n');  // Clear input buffer after scanf


    // Build the filename using the username
    // e.g. username "testuser" → "transactions_testuser.txt"
    // sprintf formats the string and stores it in filename
    char filename[100];
    sprintf(filename, "transactions_%s.txt", username);

    int nextId = transactions_next_id(username);

    // Open the file in append mode "a"
    // If the file does not exist, fopen will create it automatically
    // If it does exist, new transactions will be added to the end
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        // fopen returns NULL if the file could not be opened
        printf("Error opening file.\n");
        return;
    }

    // Write the transaction to the file as a comma separated line
    // %.2f formats the amount to 2 decimal places e.g. 45.50
    // Format: id,date,category,description,amount,type
    fprintf(file, "%d,%s,%s,%s,%.2f,%s\n", nextId, date, category, description, amount, type);
    fclose(file);   // Always close the file after writing to free system resources

    printf("Transaction saved!\n");
}


// This function deletes a transaction by its ID
// Since we cannot remove a single line from a file in C, we use a temp file
// Strategy: copy all lines EXCEPT the target ID into a temp file, then replace the original file with the temp file
// Accepts a pointer to the username string and the ID of the transaction to delete
// Returns 1 if the transaction was deleted, 0 if not found or an error occurs
int delete_transaction(char *username, int id) {

    // filename holds the original transactions file e.g. "transactions_testuser.txt"
    // tempFilename holds the temporary file e.g. "temp_transactions_testuser.txt"
    // line holds the current line being read from the file
    // deleted tracks whether we successfully found and skipped the target transaction
    char filename[100];
    char tempFilename[110];
    char line[256];
    int deleted = 0;

    // Build both filenames using the username
    // e.g. username "testuser" → "transactions_testuser.txt"
    //                          → "temp_transactions_testuser.txt"
    sprintf(filename, "transactions_%s.txt", username);
    sprintf(tempFilename, "temp_transactions_%s.txt", username);


    // Open the original file for reading "r"
    // If the file does not exist, there are no transactions to delete
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions found.\n");
        return 0;
    }

    // Open the temp file for writing "w"
    // This is where we will copy all lines except the one to delete
    // If it fails we close the original file to avoid a resource leak
    FILE *tempFile = fopen(tempFilename, "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);   // close the original file before returning to free system resources
        return 0;
    }


    // Copy all lines except the one with the matching stored ID.
    while (fgets(line, sizeof(line), file) != NULL) {
        Transaction t;
        if (parse_transaction_line(line, &t) && t.id == id) {
            deleted = 1;
            continue; // skip this line (effectively deleting it)
        }
        fputs(line, tempFile);
    }

    // Always close both files after we are done reading and writing
    fclose(file);
    fclose(tempFile);


    if (!deleted) {
        // The target ID was never found meaning it does not exist
        // Remove the temp file since we don't need it
        printf("Transaction ID not found.\n");
        remove(tempFilename);
        return 0;
    }

    // Replace the original file with the temp file
    // remove() deletes the original file
    // rename() renames the temp file to the original filename
    // The result is the original file now exists without the deleted transaction
    remove(filename);
    rename(tempFilename, filename);

    printf("Transaction deleted.\n");
    return 1;
}


// This function updates a single field of a transaction by its ID
// Since we cannot edit a single line in a file in C, we use the same temp file strategy as delete
// Strategy: copy all lines into a temp file, but replace the target line with the updated data
// Accepts a pointer to the username string and the ID of the transaction to update
// Returns 1 if the transaction was updated, 0 if not found or an error occurs
int update_transaction(char *username, int id) {
    char filename[100];
    char tempFilename[110];
    char line[256];
    int updated = 0;

    sprintf(filename, "transactions_%s.txt", username);
    sprintf(tempFilename, "temp_transactions_%s.txt", username);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions found.\n");
        return 0;
    }

    FILE *tempFile = fopen(tempFilename, "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        Transaction t;

        if (!(parse_transaction_line(line, &t) && t.id == id)) {
            fputs(line, tempFile);
            continue;
        }

        printf("Current transaction %d:\n", t.id);
        printf("Date: %s | Category: %s | Description: %s | Amount: %.2f | Type: %s\n",
               t.date, t.category, t.description, t.amount, t.type);

        printf("\nWhat do you want to update?\n");
        printf("1. Date\n");
        printf("2. Category\n");
        printf("3. Description\n");
        printf("4. Amount\n");
        printf("5. Type\n");
        printf("6. Back\n");

        char choice[16];
        read_line_input("Enter choice: ", choice, sizeof(choice));
        char c = choice[0];

        switch (c) {
            case '1':
                read_line_input("New Date (YYYY-MM-DD): ", t.date, sizeof(t.date));
                break;
            case '2':
                read_line_input("New Category: ", t.category, sizeof(t.category));
                break;
            case '3':
                read_line_input("New Description: ", t.description, sizeof(t.description));
                sanitize_description(t.description);
                break;
            case '4': {
                char amtBuf[64];
                read_line_input("New Amount: ", amtBuf, sizeof(amtBuf));
                t.amount = (float)atof(amtBuf);
                break;
            }
            case '5':
                read_line_input("New Type (income/expense): ", t.type, sizeof(t.type));
                break;
            case '6':
                fclose(file);
                fclose(tempFile);
                remove(tempFilename);
                printf("No changes made.\n");
                return 0;
            default:
                printf("Invalid choice, no changes made.\n");
                break;
        }

        write_transaction_line(tempFile, &t);
        updated = 1;
    }

    fclose(file);
    fclose(tempFile);

    if (!updated) {
        printf("Transaction ID not found.\n");
        remove(tempFilename);
        return 0;
    }

    remove(filename);
    rename(tempFilename, filename);

    printf("Transaction updated.\n");
    return 1;
}


// This function reads and displays all transactions from the user's file in a formatted table
// Each line in the file is one transaction, displayed with its ID, date, category,
// description, amount and type
// Accepts a pointer to the username string to build the correct filename
void view_transactions(char *username) {
    char filename[100];
    char line[256];

    sprintf(filename, "transactions_%s.txt", username);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions found.\n");
        return;
    }

    printf("\n\n================================= Transactions ===================================\n");
    printf("%-5s %-12s %-12s %-30s %-10s %-10s\n",
           "ID", "Date", "Category", "Description", "Amount", "Type");
    printf("----------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), file) != NULL) {
        Transaction t;
        if (!parse_transaction_line(line, &t)) continue;

        printf("%-5d %-12s %-12s %-30s %-10.2f %-10s\n",
               t.id, t.date, t.category, t.description, t.amount, t.type);
    }

    printf("==================================================================================\n");
    fclose(file);
}