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
    // Format: date,category,description,amount,type
    fprintf(file, "%s,%s,%s,%.2f,%s\n", date, category, description, amount, type);
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
    // currentId tracks which line/transaction we are currently reading
    // deleted tracks whether we successfully found and skipped the target transaction
    char filename[100];
    char tempFilename[110];
    char line[256];
    int currentId = 0;
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


    // Read through the original file line by line
    // fgets reads one line at a time and returns NULL when it reaches the end of the file
    while (fgets(line, sizeof(line), file) != NULL) {
        currentId++;    // increment the ID counter for each line we read

        if (currentId == id) {
            // We found the transaction to delete
            // Set deleted to 1 to confirm we found it
            // continue skips fputs so this line is never written to the temp file
            // effectively deleting it
            deleted = 1;
            continue;
        }

        // This is not the target line so copy it unchanged into the temp file
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

    // filename holds the original transactions file e.g. "transactions_testuser.txt"
    // tempFilename holds the temporary file e.g. "temp_transactions_testuser.txt"
    // line holds the current line being read from the file
    // currentId tracks which line/transaction we are currently reading
    // updated tracks whether we successfully found and updated the target transaction
    char filename[100];
    char tempFilename[110];
    char line[256];
    int currentId = 0;
    int updated = 0;

    // Build both filenames using the username
    // e.g. username "testuser" → "transactions_testuser.txt"
    //                          → "temp_transactions_testuser.txt"
    sprintf(filename, "transactions_%s.txt", username);
    sprintf(tempFilename, "temp_transactions_%s.txt", username);


    // Open the original file for reading "r"
    // If the file does not exist, there are no transactions to update
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions found.\n");
        return 0;
    }

    // Open the temp file for writing "w"
    // This is where we copy all lines, replacing the target line with updated data
    // If it fails we close the original file to avoid a resource leak
    FILE *tempFile = fopen(tempFilename, "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);   // close the original file before returning to free system resources
        return 0;
    }


    // Read through the original file line by line
    // fgets reads one line at a time and returns NULL when it reaches the end of the file
    while (fgets(line, sizeof(line), file) != NULL) {
        currentId++;    // increment the ID counter for each line we read

        if (currentId == id) {
            // We found the target transaction — parse it and let the user update one field

            // fields[5][100] holds each of the 5 CSV fields for this transaction
            // fields[0] = date, fields[1] = category, fields[2] = description
            // fields[3] = amount, fields[4] = type
            char fields[5][100];

            // pos  = current position in the line string
            // col  = which field we are currently building (0-4)
            // start = where the current field started in the line
            // c    = the users menu choice for which field to update
            int pos = 0, col = 0, start = 0;
            int c;

            // Parse the CSV line character by character
            // When we hit a comma we know the current field has ended
            // so we null terminate it and move to the next field
            while (line[pos] != '\0' && line[pos] != '\n') {
                if (line[pos] == ',') {
                    fields[col][pos - start] = '\0'; // null terminate the current field
                    col++;
                    start = pos + 1;    // the next field starts right after the comma
                } else {
                    // Copy the character into the current field
                    fields[col][pos - start] = line[pos];
                }
                pos++;
            }
            // Null terminate the last field since there is no trailing comma
            fields[col][pos - start] = '\0';


            // Display the current values of the transaction so the user
            // knows what they are about to change
            printf("Current transaction %d:\n", id);
            printf("Date: %s | Category: %s | Description: %s | Amount: %s | Type: %s\n",
                   fields[0], fields[1], fields[2], fields[3], fields[4]);

            // Ask the user which field they want to update
            printf("\nWhat do you want to update?\n");
            printf("1. Date\n");
            printf("2. Category\n");
            printf("3. Description\n");
            printf("4. Amount\n");
            printf("5. Type\n");
            printf("6. Back\n");
            printf("Enter choice: ");

            // Read the users choice as a single character
            c = getchar();
            while (getchar() != '\n');  // flush the leftover '\n' from the input buffer

            // Based on the users choice, overwrite the corresponding field
            // scanf reads the new value directly into the field array
            // %99s limits input to 99 characters to prevent buffer overflow
            switch (c) {
                case '1':
                    printf("New Date (YYYY-MM-DD): ");
                    scanf("%99s", fields[0]);   // overwrite fields[0] with the new date
                    while (getchar() != '\n');
                    break;
                case '2':
                    printf("New Category: ");
                    scanf("%99s", fields[1]);   // overwrite fields[1] with the new category
                    while (getchar() != '\n');
                    break;
                case '3':
                    printf("New Description: ");
                    scanf("%99s", fields[2]);   // overwrite fields[2] with the new description
                    while (getchar() != '\n');
                    break;
                case '4':
                    printf("New Amount: ");
                    scanf("%99s", fields[3]);   // overwrite fields[3] with the new amount
                    while (getchar() != '\n');
                    break;
                case '5':
                    printf("New Type (Income/Expense): ");
                    scanf("%99s", fields[4]);   // overwrite fields[4] with the new type
                    while (getchar() != '\n');
                    break;
                case '6':
                    // User chose to go back — discard the temp file and exit with no changes
                    // We must close both files before removing the temp file
                    fclose(file);
                    fclose(tempFile);
                    remove(tempFilename);   // delete the temp file since we don't need it
                    printf("No changes made.\n");
                    return 0;
                default:
                    printf("Invalid choice, no changes made.\n");
                    break;
            }

            // Write the updated fields back to the temp file as a CSV line
            // One of the fields has been overwritten with the new value
            // the rest remain unchanged from when we parsed them
            fprintf(tempFile, "%s,%s,%s,%s,%s\n",
                    fields[0], fields[1], fields[2], fields[3], fields[4]);
            updated = 1;    // mark that we successfully found and updated the target transaction

        } else {
            // This is not the target line — copy it unchanged into the temp file
            fputs(line, tempFile);
        }
    }

    // Always close both files after we are done reading and writing
    fclose(file);
    fclose(tempFile);


    if (!updated) {
        // The target ID was never found meaning it does not exist
        // Remove the temp file since we don't need it
        printf("Transaction ID not found.\n");
        remove(tempFilename);
        return 0;
    }

    // Replace the original file with the temp file
    // remove() deletes the original file
    // rename() renames the temp file to the original filename
    // The result is the original file now exists with the updated transaction
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

    // filename holds the transactions file e.g. "transactions_testuser.txt"
    // line holds the current line being read from the file
    // id is a counter used to display the transaction ID (line number in the file)
    //    since we don't store the ID in the file, we calculate it by counting lines
    char filename[100];
    char line[256];
    int id = 1;

    // Build the filename using the username
    // e.g. username "testuser" → "transactions_testuser.txt"
    sprintf(filename, "transactions_%s.txt", username);

    // Open the file for reading "r"
    // If the file does not exist, there are no transactions to display
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions found.\n");
        return;
    }

    // Print the table header with column names
    // %-5s, %-12s etc. left align each column with a fixed width
    // This ensures the table stays aligned regardless of the data length
    printf("\n\n================================= Transactions ===================================\n");
    printf("%-5s %-12s %-12s %-30s %-10s %-10s\n",
           "ID", "Date", "Category", "Description", "Amount", "Type");
    printf("----------------------------------------------------------------------------------\n");


    // Read and display each transaction line by line
    // fgets reads one line at a time and returns NULL when it reaches the end of the file
    while (fgets(line, sizeof(line), file) != NULL) {

        // pos   = current position in the line string
        // col   = which field we are currently building (0-4)
        // start = where the current field started in the line
        // fields[5][100] holds each of the 5 CSV fields for this transaction
        // fields[0] = date, fields[1] = category, fields[2] = description
        // fields[3] = amount, fields[4] = type
        int pos = 0, col = 0, start = 0;
        char fields[5][100];

        // Parse the comma separated line character by character
        // When we hit a comma we know the current field has ended
        // so we null terminate it and move to the next field
        while (line[pos] != '\0' && line[pos] != '\n') {
            if (line[pos] == ',') {
                fields[col][pos - start] = '\0'; // null terminate the current field
                col++;
                start = pos + 1;    // the next field starts right after the comma
            } else {
                // Copy the character into the current field
                fields[col][pos - start] = line[pos];
            }
            pos++;
        }
        // Null terminate the last field since there is no trailing comma
        fields[col][pos - start] = '\0';

        // Print the transaction as a formatted row with its ID
        // id is printed first since it is not stored in the file
        // %-5d left aligns the ID in a 5 character wide column
        printf("%-5d %-12s %-12s %-30s %-10s %-10s\n",
               id, fields[0], fields[1], fields[2], fields[3], fields[4]);
        id++;   // increment the ID counter for the next transaction
    }

    printf("==================================================================================\n");
    fclose(file);   // always close the file after reading to free system resources
}