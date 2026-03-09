/*
 * transactions.c
 * 
 * Handles all transaction file operations (CRUD).
 * Contains functions to add, delete, update, and view
 * income/expense transactions stored in the user's .dat file.
 */

#include <stdlib.h>
#include <stdio.h>


void  add_transaction(char *username){
    // Add a new transaction (income or expense) for the specified user
    printf("Adding transaction for user: %s\n", username);
}


int   delete_transaction(char *username, int id){
    // Delete the transaction with the specified ID for the user

    return 0;
}


int   update_transaction(char *username, int id){
    // Update the transaction with the specified ID for the user
    return 0;
}


void  view_transactions(char *username){
    // Display all transactions for the specified user in a readable format
}
