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
    // Delete the transaction with the specified ID for the user

    return 0;
}


int   update_transaction(char *username, int id){
    // Update the transaction with the specified ID for the user
    return 0;
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
