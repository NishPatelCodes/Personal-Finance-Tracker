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


void add_transaction(char *username) {
    // Prompts the user for transaction details and appends it to their transactions file

    char date[11];
    char category[30];
    char description[100];
    float amount;
    char type[10];

    // Prompt the user for each transaction field
    printf("Date (YYYY-MM-DD): ");
    scanf("%s", date);
    while (getchar() != '\n');      // Clear input buffer after scanf

    printf("Category (Food, Rent, etc): ");
    scanf("%s", category);
    while (getchar() != '\n');      // Clear input buffer after scanf

    printf("Description: ");
    fgets(description, sizeof(description), stdin);  // fgets used to allow spaces
    int len = strlen(description);
    if (description[len - 1] == '\n')
        description[len - 1] = '\0';    // Strip trailing newline left by fgets

    printf("Amount: ");
    scanf("%f", &amount);
    while (getchar() != '\n');      // Clear input buffer after scanf

    printf("Type (income/expense): ");
    scanf("%s", type);
    while (getchar() != '\n');      // Clear input buffer after scanf

    // Build filename e.g. "transactions_testuser.txt"
    char filename[100];
    sprintf(filename, "transactions_%s.txt", username);

    // Open in append mode — creates the file if it doesn't exist
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Write transaction as a comma separated line: date,category,description,amount,type
    fprintf(file, "%s,%s,%s,%.2f,%s\n", date, category, description, amount, type);
    fclose(file);

    printf("Transaction saved!\n");
}


int delete_transaction(char *username, int id) {
    // Deletes a transaction by ID by copying all other lines into a temp file,
    // then replacing the original file with the temp file

    char filename[100];
    char tempFilename[110];
    char line[256];
    int currentId = 0;
    int deleted = 0;

    // Build filenames
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

    // Copy all lines except the one matching the target ID into the temp file
    while (fgets(line, sizeof(line), file) != NULL) {
        currentId++;
        if (currentId == id) {
            deleted = 1;    // mark that we found and skipped the target line
            continue;       // skip writing this line to effectively delete it
        }
        fputs(line, tempFile);
    }

    fclose(file);
    fclose(tempFile);

    if (!deleted) {
        printf("Transaction ID not found.\n");
        remove(tempFilename);   // clean up temp file if ID wasn't found
        return 0;
    }

    // Replace original file with the temp file
    remove(filename);
    rename(tempFilename, filename);

    printf("Transaction deleted.\n");
    return 1;
}


int update_transaction(char *username, int id) {
    // Updates a single field of a transaction by ID.
    // Reads all lines into a temp file, replacing the target line with updated data,
    // then replaces the original file with the temp file

    char filename[100];
    char tempFilename[110];
    char line[256];
    int currentId = 0;
    int updated = 0;

    // Build filenames
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
        currentId++;

        if (currentId == id) {
            // Parse the CSV line into individual fields
            char fields[5][100];
            int pos = 0, col = 0, start = 0;
            int c;

            while (line[pos] != '\0' && line[pos] != '\n') {
                if (line[pos] == ',') {
                    fields[col][pos - start] = '\0'; // null terminate current field
                    col++;
                    start = pos + 1;    // next field starts after the comma
                } else {
                    fields[col][pos - start] = line[pos];
                }
                pos++;
            }
            fields[col][pos - start] = '\0';    // null terminate last field

            // Show current values so user knows what they are changing
            printf("Current transaction %d:\n", id);
            printf("Date: %s | Category: %s | Description: %s | Amount: %s | Type: %s\n",
                   fields[0], fields[1], fields[2], fields[3], fields[4]);

            printf("\nWhat do you want to update?\n");
            printf("1. Date\n");
            printf("2. Category\n");
            printf("3. Description\n");
            printf("4. Amount\n");
            printf("5. Type\n");
            printf("6. Back\n");
            printf("Enter choice: ");

            c = getchar();
            while (getchar() != '\n');  // flush buffer after reading choice

            switch (c) {
                case '1':
                    printf("New Date (YYYY-MM-DD): ");
                    scanf("%99s", fields[0]);
                    while (getchar() != '\n');
                    break;
                case '2':
                    printf("New Category: ");
                    scanf("%99s", fields[1]);
                    while (getchar() != '\n');
                    break;
                case '3':
                    printf("New Description: ");
                    scanf("%99s", fields[2]);
                    while (getchar() != '\n');
                    break;
                case '4':
                    printf("New Amount: ");
                    scanf("%99s", fields[3]);
                    while (getchar() != '\n');
                    break;
                case '5':
                    printf("New Type (Income/Expense): ");
                    scanf("%99s", fields[4]);
                    while (getchar() != '\n');
                    break;
                case '6':
                    // User chose to go back — discard temp file and exit with no changes
                    fclose(file);
                    fclose(tempFile);
                    remove(tempFilename);
                    printf("No changes made.\n");
                    return 0;
                default:
                    printf("Invalid choice, no changes made.\n");
                    break;
            }

            // Write the updated fields back as a CSV line
            fprintf(tempFile, "%s,%s,%s,%s,%s\n",
                    fields[0], fields[1], fields[2], fields[3], fields[4]);
            updated = 1;

        } else {
            // Not the target line — copy it unchanged
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (!updated) {
        printf("Transaction ID not found.\n");
        remove(tempFilename);
        return 0;
    }

    // Replace original file with updated temp file
    remove(filename);
    rename(tempFilename, filename);

    printf("Transaction updated.\n");
    return 1;
}


void view_transactions(char *username) {
    // Reads and displays all transactions from the user's file in a formatted table

    char filename[100];
    char line[256];
    int id = 1;     // used to display the transaction ID (line number)

    // Build filename e.g. "transactions_testuser.txt"
    sprintf(filename, "transactions_%s.txt", username);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions found.\n");
        return;
    }

    // Print table header
    printf("\n\n================================= Transactions ===================================\n");
    printf("%-5s %-12s %-12s %-30s %-10s %-10s\n",
           "ID", "Date", "Category", "Description", "Amount", "Type");
    printf("----------------------------------------------------------------------------------\n");

    // Read and display each transaction line by line
    while (fgets(line, sizeof(line), file) != NULL) {
        int pos = 0, col = 0, start = 0;
        char fields[5][100];

        // Parse the comma separated line into fields
        while (line[pos] != '\0' && line[pos] != '\n') {
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

        // Print each transaction as a formatted row with its ID
        printf("%-5d %-12s %-12s %-30s %-10s %-10s\n",
               id, fields[0], fields[1], fields[2], fields[3], fields[4]);
        id++;
    }

    printf("==================================================================================\n");
    fclose(file);
}