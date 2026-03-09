/* Created by Owen Fugger and Nish Patel on 6/11/24.
 *
 * Personal Finance Tracker is a C program designed to help users manage their finances by tracking income, 
 * expenses, and generating reports. 
 * The program provides a simple command-line interface for users to input their financial data and 
 * view summaries of their financial status.
 */


/*
 * main.c
 * 
 * Entry point of the Personal Finance Tracker application.
 * Contains the main menu loop and program startup logic.
 * All functions from other files are declared here before main().
 */


#include <stdlib.h>
#include <stdio.h>


// ======================================================
// ================ Function Prototypes =================
// ======================================================

// ---- utils.c functions ----
int   my_strlen(char *str);
void  my_strcpy(char *dest, char *src);
int   my_strcmp(char *a, char *b);

// ---- encryption.c functions ----
void  encrypt_password(char *password, char *output);
void  decrypt_password(char *encrypted, char *output);
int   mod_inverse(int a, int mod);

// ---- auth.c functions ----
int   register_user(char *username, char *password);
int   login_user(char *username, char *password);
int   user_exists(char *username);

// ---- transactions.c functions ----
void  add_transaction(char *username);
int   delete_transaction(char *username, int id);
int   update_transaction(char *username, int id);
void  view_transactions(char *username);

// ---- reports.c functions ----
void  generate_monthly_report(char *username);
void  filter_by_category(char *username, char *category);
void  filter_by_type(char *username, char *type);
void  search_by_date(char *username, char *date);
void  export_report(char *username);

// ======================================================
// ================ Test variables ====================
// ======================================================

int menuIteration = 0;  // This is for testing purposes. It allows us to see the menu multiple times without having to implement the full login system yet.
    
// Test case
char username[50] = "testuser";
char password[50] = "testpass";


// ======================================================
// ================ Main Menu Functions =================
// ======================================================

void  print_menu(int *running){
    int menuChoice;

    printf("\nMenu Iteration: %d", menuIteration);   // Test purposes

    // Display menu options
    printf("\n========= Main Menu ==========\n");
    printf("0. Exit\n");
    printf("1. Add Transaction\n");
    printf("2. Remove Transaction\n");
    printf("3. Update Transaction\n");
    printf("4. View Transactions\n");
    printf("5. Generate Monthly Report\n");
    printf("\nEnter your choice: ");
    
    menuIteration++;   

    // Read user input for menu choice
    menuChoice = getchar();
    getchar();  

    // Handle menu choice using a switch statement
    switch (menuChoice){
        case '0':
            printf("Goodbye!\n");
            *running = 0;
            break;

        case '1':
            add_transaction(username);
            break;

        case '2':
            printf("Removing Transaction\n");
            break;
        
        case '3':
            printf("Updating Transaction\n");
            break;
        
        case '4':
            printf("Viewing Transactions\n");
            break;

        case '5':
            printf("Generating Monthly Report\n");
            break;
    
        default:
            printf("Invalid choice\n");
            break;
    }


}


void print_welcome_message(){
    // Display welcome message

    printf("\n==================================\n");
    printf("Welcome to Personal Finance Tracker");
    printf("\n==================================\n\n");

    printf("Created by: Owen Fugger and Nish Patel\n\n");
}


int main() {
    print_welcome_message();

    // Main menu loop
    int running = 1;
    while (running) {
        print_menu(&running);
    }

    return 0;
}