/*
 * transactions.c
 * 
 * Handles all transaction file operations (CRUD).
 * Contains functions to add, delete, update, and view
 * income/expense transactions stored in the user's .dat file.
 */

#include <stdlib.h>
#include <stdio.h>


void add_transaction(char *username) {
    char date[11];
    char category[30];
    char description[100];
    float amount;
    char type[10];

    printf("Date (YYYY-MM-DD): ");
    scanf("%s", date);
    while (getchar() != '\n');      // flush

    printf("Category (Food, Rent, etc): ");
    scanf("%s", category);
    while (getchar() != '\n');      // flush

    printf("Description: ");
    scanf("%s", description);
    while (getchar() != '\n');      // flush

    printf("Amount: ");
    scanf("%f", &amount);
    while (getchar() != '\n');      // flush

    printf("Type (income/expense): ");
    scanf("%s", type);
    while (getchar() != '\n');      // flush

    // Build the filename e.g. "transactions_testuser.txt"
    char filename[100];
    int i = 0, j = 0;

    // copy "transactions_" into filename
    char prefix[] = "transactions_";
    while (prefix[i] != '\0') {
        filename[i] = prefix[i];
        i++;
    }
    // copy username into filename
    while (username[j] != '\0') {
        filename[i] = username[j];
        i++;
        j++;
    }
    // copy ".txt" into filename
    char ext[] = ".txt";
    int k = 0;
    while (ext[k] != '\0') {
        filename[i] = ext[k];
        i++;
        k++;
    }
    filename[i] = '\0';

    // Open the file in append mode and write the transaction
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "%s,%s,%s,%.2f,%s\n", date, category, description, amount, type);
    fclose(file);

    printf("Transaction saved!\n");
}


int   delete_transaction(char *username, int id){
    char filename[100];
    char tempFilename[110] = "temp_";
    char line[256];
    int currentId = 0;
    int deleted = 0;

    int i = 0, j = 0;
    char prefix[] = "transactions_";
    char ext[]    = ".txt";

    while (prefix[i] != '\0') { filename[i] = prefix[i]; i++; }
    while (username[j] != '\0') { filename[i] = username[j]; i++; j++; }
    j = 0;
    while (ext[j] != '\0') { filename[i] = ext[j]; i++; j++; }
    filename[i] = '\0';

    i = 5;
    while (filename[j] != '\0') {
        tempFilename[i] = filename[j];
        i++;
        j++;
    }
    tempFilename[i] = '\0';

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
            deleted = 1;
            continue;
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

    remove(filename);
    rename(tempFilename, filename);

    printf("Transaction deleted.\n");
    return 1;
}


int   update_transaction(char *username, int id){
    char filename[100];
    char tempFilename[110] = "temp_";
    char line[256];
    int currentId = 0;
    int updated = 0;

    int i = 0, j = 0;
    char prefix[] = "transactions_";
    char ext[]    = ".txt";

    while (prefix[i] != '\0') { filename[i] = prefix[i]; i++; }
    while (username[j] != '\0') { filename[i] = username[j]; i++; j++; }
    j = 0;
    while (ext[j] != '\0') { filename[i] = ext[j]; i++; j++; }
    filename[i] = '\0';

    i = 5;
    while (filename[j] != '\0') {
        tempFilename[i] = filename[j];
        i++;
        j++;
    }
    tempFilename[i] = '\0';

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
            char fields[5][100];
            int pos = 0, col = 0, start = 0;
            int c;

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

            printf("Current transaction %d:\n", id);
            printf("Date: %s, Category: %s, Description: %s, Amount: %s, Type: %s\n",
                   fields[0], fields[1], fields[2], fields[3], fields[4]);

            printf("What do you want to update?\n");
            printf("1. Date\n");
            printf("2. Category\n");
            printf("3. Description\n");
            printf("4. Amount\n");
            printf("5. Type\n");
            printf("Enter choice: ");

            c = getchar();
            while (getchar() != '\n');

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
                    printf("New Type (income/expense): ");
                    scanf("%99s", fields[4]);
                    while (getchar() != '\n');
                    break;
                default:
                    printf("Invalid choice, no changes made to this transaction.\n");
                    break;
            }

            fprintf(tempFile, "%s,%s,%s,%s,%s\n",
                    fields[0], fields[1], fields[2], fields[3], fields[4]);
            updated = 1;
        } else {
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

    remove(filename);
    rename(tempFilename, filename);

    printf("Transaction updated.\n");
    return 1;
}


void view_transactions(char *username) {
    char filename[100];
    char line[256];

    // Build filename (same way as add_transaction)
    // "transactions_testuser.txt"
    int i = 0, j = 0;
    char prefix[] = "transactions_";
    char ext[]    = ".txt";

    while (prefix[i] != '\0') { filename[i] = prefix[i]; i++; }
    while (username[j] != '\0') { filename[i] = username[j]; i++; j++; }
    j = 0;
    while (ext[j] != '\0') { filename[i] = ext[j]; i++; j++; }
    filename[i] = '\0';

    // Open file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No transactions found.\n");
        return;
    }

    // Print header
    printf("\n========= Transactions =========\n");
    printf("%-12s %-12s %-20s %-10s %-10s\n",
           "Date", "Category", "Description", "Amount", "Type");
    printf("------------------------------------------------\n");

    // Read line by line
    while (fgets(line, sizeof(line), file) != NULL) {

        // Parse each field from the comma separated line
        char date[11], category[30], description[100], type[10];
        float amount;

        // split the line on commas manually
        int pos = 0, col = 0, start = 0;
        char fields[5][100];   // 5 fields per line

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
        fields[col][pos - start] = '\0'; // last field

        // Print formatted row
        printf("%-12s %-12s %-20s %-10s %-10s\n",
               fields[0], fields[1], fields[2], fields[3], fields[4]);
    }

    printf("================================================\n");
    fclose(file);
}
