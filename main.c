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
#include <string.h>


// ======================================================
// ================ Function Prototypes =================
// ======================================================
// These are declarations of all functions used in the program
// They must be declared before main() so the compiler knows they exist
// The actual logic for each function is in their respective .c files

// ---- main.c functions ----
void print_menu(int *running);
void print_manage_transactions_menu();
void print_search_filter_menu();
void print_reports_menu();
void print_welcome_message();
void user_login_menu();

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
void  generate_overall_report(char *username);


// ======================================================
// ================ Test Variables ======================
// ======================================================
// These variables are used for testing purposes only
// They allow us to test the program without implementing the full login system
// They will be removed once auth.c is fully implemented

// Hardcoded test credentials — replace with real login once auth.c is done
char username[50] = "testuser";
char password[50] = "testpass";



// ======================================================
// ================ Menu Functions ======================
// ======================================================

// This function displays the manage transactions submenu and handles the users choice
// It allows the user to add, delete, update or view their transactions
// When the user chooses back, return is called which brings them back to the main menu loop
void print_manage_transactions_menu() {

    int menuTransactionChoice;  // stores the users menu choice as a character

    printf("\n\n\n\n========= Manage Transactions ==========\n");
    printf("1. Add Transaction\n");
    printf("2. Delete Transaction\n");
    printf("3. Update Transaction\n");
    printf("4. View Transactions\n");
    printf("5. Back to Main Menu\n");
    printf("\nEnter your choice: ");

    // Read the users choice as a single character
    menuTransactionChoice = getchar();
    getchar();  // consume the leftover '\n' from the input buffer

    switch (menuTransactionChoice) {

        case '1': {
            // Call add_transaction with the current username
            // add_transaction will prompt the user for all transaction details
            add_transaction(username);
            break;
        }

        case '2': {
            // Ask the user for the ID of the transaction to delete
            // The ID corresponds to the line number in the transactions file
            int id;
            printf("\nEnter transaction ID to delete: ");
            scanf("%d", &id);
            while (getchar() != '\n'); // flush input buffer after reading int
            delete_transaction(username, id);
            break;
        }

        case '3': {
            // Display all transactions first so the user can see the IDs
            // then ask which ID they want to update
            int id;
            view_transactions(username);
            printf("Enter transaction ID to update: ");
            scanf("%d", &id);
            while (getchar() != '\n'); // flush input buffer after reading int
            update_transaction(username, id);
            break;
        }

        case '4': {
            // Display all transactions in a formatted table
            view_transactions(username);
            break;
        }

        case '5': {
            // Return exits this function and goes back to the while loop in main()
            // which will then call print_menu() again naturally
            return;
        }

        default: {
            printf("Invalid choice\n");
            break;
        }
    }
}



// This function displays the search and filter submenu and handles the users choice
// It allows the user to search transactions by date, category or type
// When the user chooses back, return is called which brings them back to the main menu loop
void print_search_filter_menu() {

    int menuSearchFilterChoice; // stores the users menu choice as a character

    printf("\n\n\n\n========= Search/Filter Transactions ==========\n");
    printf("1. Search by Date\n");
    printf("2. Filter by Category\n");
    printf("3. Filter by Type (Income/Expense)\n");
    printf("4. Back to Main Menu\n");
    printf("\nEnter your choice: ");

    // Read the users choice as a single character
    menuSearchFilterChoice = getchar();
    getchar();  // consume the leftover '\n' from the input buffer

    switch (menuSearchFilterChoice) {

        case '1':
            // TODO: implement search_by_date in reports.c
            // search_by_date(username, date);
            break;

        case '2':
            // TODO: implement filter_by_category in reports.c
            // filter_by_category(username, category);
            break;

        case '3':
            // TODO: implement filter_by_type in reports.c
            // filter_by_type(username, type);
            break;

        case '4':
            // Return exits this function and goes back to the while loop in main()
            // which will then call print_menu() again naturally
            return;

        default:
            break;
    }
}



// This function displays the reports and summaries submenu and handles the users choice
// It allows the user to generate monthly reports, view categorized breakdowns
// and export reports to a .txt file
// When the user chooses back, return is called which brings them back to the main menu loop
void print_reports_menu() {

    int menuReportChoice;   // stores the users menu choice as a character

    printf("\n\n\n\n========= Reports & Summaries ==========\n");
    printf("1. Generate Overall Report\n");
    printf("2. Categorized Breakdown\n");
    printf("3. Export Report to .txt\n");
    printf("4. Back to Main Menu\n");
    printf("\nEnter your choice: ");

    // Read the users choice as a single character
    menuReportChoice = getchar();
    getchar();  // consume the leftover '\n' from the input buffer

    switch (menuReportChoice) {

        case '1':
            // Generate a monthly summary report for the current user
            generate_overall_report(username);
            break;

        case '2':
            // TODO: implement filter_by_category in reports.c
            // filter_by_category(username, category);
            char category[30];
            printf("Enter category: ");
            scanf("%s", category);
            while (getchar() != '\n');
            generate_category_report(username, category);
            break;

        case '3':
            // TODO: implement export_report in reports.c
            // export_report(username);
            break;

        case '4':
            // Return exits this function and goes back to the while loop in main()
            // which will then call print_menu() again naturally
            return;

        default:
            break;
    }
}



// This function displays the main menu and handles the users choice
// It accepts a pointer to the running flag so it can stop the while loop in main()
// when the user chooses to exit
void print_menu(int *running) {

    int menuChoice; // stores the users menu choice as a character

    printf("\n\n\n========= Main Menu ==========\n");
    printf("0. Exit\n");
    printf("1. Manage Transactions\n");
    printf("2. Search/Filter Transactions\n");
    printf("3. Reports & Summaries\n");
    printf("\nEnter your choice: ");

    // Read the users choice as a single character
    menuChoice = getchar();
    getchar();  // consume the leftover '\n' from the input buffer

    switch (menuChoice) {

        case '0':
            // Set running to 0 to stop the while loop in main()
            // This causes the program to exit cleanly after printing goodbye
            printf("Goodbye!\n");
            *running = 0;
            break;

        case '1':
            // Navigate to the manage transactions submenu
            print_manage_transactions_menu();
            break;

        case '2':
            // Navigate to the search and filter submenu
            print_search_filter_menu();
            break;

        case '3':
            // Navigate to the reports and summaries submenu
            print_reports_menu();
            break;

        default:
            printf("Invalid choice\n");
            break;
    }
}



// This function displays the welcome message when the program starts
// It is called once at the beginning of main() before the menu loop
void print_welcome_message() {
    printf("\n========================================\n");
    printf("   Welcome to Personal Finance Tracker   ");
    printf("\n========================================");
    printf("\nCreated by: Owen Fugger and Nish Patel\n\n");
}



// ======================================================
// ================== User Log in =======================
// ======================================================


// This function displays the log in startup screen
// Offering users to log in or register for an account
void user_login_menu() {

    int loggedIn = 0;

    while (!loggedIn) {
        printf("\n0. Exit program");
        printf("\n1. Log in");
        printf("\n2. Register");
        printf("\n\nEnter your choice: ");

        int loginMenuChoice = getchar();
        getchar();


        char inputUsername[50];
        char inputPassword[50];


        switch (loginMenuChoice) {
            // Exits the program
            case '0':
                printf("Goodbye!\n");
                exit(0);
                break;
            
            // Prompts user to enter user name and password to log in
            case '1':
                printf("Username: ");
                scanf("%s", inputUsername);
                while (getchar() != '\n');

                printf("Password: ");
                scanf("%s", inputPassword);
                while (getchar() != '\n');

                loggedIn = login_user(inputUsername, inputPassword);
                if (!loggedIn)
                    printf("Invalid username or password. Try again.\n");
                break;

            
            case '2':
                printf("Username: ");
                scanf("%s", inputUsername);
                while (getchar() != '\n');

                printf("Password: ");
                scanf("%s", inputPassword);
                while (getchar() != '\n');

                loggedIn = register_user(inputUsername, inputPassword);
                if (!loggedIn)
                    printf("Registration failed. Try again.\n");
                break;

            case '3':
                loggedIn = 1;
                strcpy(username, "testuser");
                printf("\nDev bypass activated. Logged in as testuser.\n");
                break;
            
            default:
                printf("Invalid choice\n");
                break;
        }

        // copy the username into the global variable once logged in
            if (loggedIn && loginMenuChoice != '3')
            strcpy(username, inputUsername);
    }
}



// Displays the welcome message then enters the main menu loop
// The loop continues until the user chooses to exit (running = 0)
int main() {
    print_welcome_message();
    user_login_menu();

    // running controls the main menu loop
    // it starts as 1 (true) and is set to 0 (false) when the user chooses to exit
    int running = 1;
    while (running) {
        print_menu(&running);
    }

    return 0;   // return 0 to the OS to indicate the program exited successfully
}