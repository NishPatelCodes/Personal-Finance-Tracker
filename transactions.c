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

#include "tellGCCthesefunctions.h"


// Strips trailing newline and carriage return characters from a string
static void trim_newlines(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
        len--;
    }
}

// Prints a prompt and reads a full line of input into out, stripping the trailing newline
static void read_line_input(const char *prompt, char *out, size_t outSize) {
    printf("%s", prompt);
    if (fgets(out, (int)outSize, stdin) == NULL) {
        out[0] = '\0';
        return;
    }
    trim_newlines(out);
}


// Prompts the user for transaction details and appends it to their transactions file
// date is 11 chars: 10 for "YYYY-MM-DD" + 1 for null terminator
void add_transaction(char *username) {

    char date[11];
    char category[30];
    char description[100];
    float amount;
    char type[10];  // either "income" or "expense"

    // Date and category use scanf since they are single words with no spaces
    printf("Date (YYYY-MM-DD): ");
    scanf("%s", date);
    while (getchar() != '\n');

    printf("Category (Food, Rent, etc): ");
    scanf("%s", category);
    while (getchar() != '\n');

    // fgets is used for description to allow spaces e.g. "Monthly Rent"
    // The trailing newline left by fgets is stripped after reading
    printf("Description: ");
    fgets(description, sizeof(description), stdin);
    int len = strlen(description);
    if (description[len - 1] == '\n')
        description[len - 1] = '\0';

    // sanitize_description replaces any commas with spaces to preserve CSV structure
    sanitize_description(description);

    printf("Amount: ");
    scanf("%f", &amount);
    while (getchar() != '\n');

    printf("Type (income/expense): ");
    scanf("%s", type);
    while (getchar() != '\n');

    // Build filename e.g. "transactions_testuser.txt"
    char filename[100];
    sprintf(filename, "transactions_%s.txt", username);

    // Get the next available ID by scanning the file for the current highest ID
    int nextId = transactions_next_id(username);

    // Open in append mode — creates the file if it doesn't exist
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Write the transaction as a comma separated line
    // Format: id,date,category,description,amount,type
    fprintf(file, "%d,%s,%s,%s,%.2f,%s\n", nextId, date, category, description, amount, type);
    fclose(file);

    printf("Transaction saved!\n");
}


// Deletes a transaction by its ID
// Strategy: copy all lines EXCEPT the target ID into a temp file,
// then replace the original file with the temp file
// Returns 1 if deleted, 0 if not found or an error occurs
int delete_transaction(char *username, int id) {

    char filename[100];
    char tempFilename[110];
    char line[256];
    int deleted = 0;

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

    // Read line by line — skip the line whose ID matches the target
    while (fgets(line, sizeof(line), file) != NULL) {
        Transaction t;
        if (parse_transaction_line(line, &t) && t.id == id) {
            deleted = 1;
            continue;   // skip this line to effectively delete it
        }
        fputs(line, tempFile);
    }

    fclose(file);
    fclose(tempFile);

    if (!deleted) {
        printf("Transaction ID not found.\n");
        remove(tempFilename);
        return 0;
    }

    // Replace the original file with the temp file
    remove(filename);
    rename(tempFilename, filename);

    printf("Transaction deleted.\n");
    return 1;
}


// Updates a single field of a transaction by its ID
// Strategy: copy all lines into a temp file, replacing the target line with updated data,
// then replace the original file with the temp file
// Returns 1 if updated, 0 if not found or an error occurs
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

        // If this line is not the target, copy it unchanged and move on
        if (!(parse_transaction_line(line, &t) && t.id == id)) {
            fputs(line, tempFile);
            continue;
        }

        // Found the target — show current values and ask what to update
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

        // Overwrite only the chosen field directly in the Transaction struct
        switch (c) {
            case '1':
                read_line_input("New Date (YYYY-MM-DD): ", t.date, sizeof(t.date));
                break;
            case '2':
                read_line_input("New Category: ", t.category, sizeof(t.category));
                break;
            case '3':
                read_line_input("New Description: ", t.description, sizeof(t.description));
                sanitize_description(t.description);    // strip any commas from the new description
                break;
            case '4': {
                char amtBuf[64];
                read_line_input("New Amount: ", amtBuf, sizeof(amtBuf));
                t.amount = (float)atof(amtBuf);     // convert string input to float
                break;
            }
            case '5':
                read_line_input("New Type (income/expense): ", t.type, sizeof(t.type));
                break;
            case '6':
                // Discard the temp file and exit with no changes
                fclose(file);
                fclose(tempFile);
                remove(tempFilename);
                printf("No changes made.\n");
                return 0;
            default:
                printf("Invalid choice, no changes made.\n");
                break;
        }

        // Write the updated transaction back to the temp file
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

    // Replace the original file with the updated temp file
    remove(filename);
    rename(tempFilename, filename);

    printf("Transaction updated.\n");
    return 1;
}


// Reads and displays all transactions from the user's file in a formatted table
void view_transactions(char *username) {

    char filename[100];
    char line[256];

    sprintf(filename, "transactions_%s.txt", username);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions found.\n");
        return;
    }

    // Print table header — columns are left aligned with fixed widths
    printf("\n\n================================= Transactions ===================================\n");
    printf("%-5s %-12s %-12s %-30s %-10s %-10s\n",
           "ID", "Date", "Category", "Description", "Amount", "Type");
    printf("----------------------------------------------------------------------------------\n");

    // Parse and display each transaction line by line
    while (fgets(line, sizeof(line), file) != NULL) {
        Transaction t;
        if (!parse_transaction_line(line, &t)) continue;    // skip malformed lines

        printf("%-5d %-12s %-12s %-30s %-10.2f %-10s\n",
               t.id, t.date, t.category, t.description, t.amount, t.type);
    }

    printf("==================================================================================\n");
    fclose(file);
}