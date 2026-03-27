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
void  generate_category_report(char *username, char *category);
void  export_report_to_txt(char *username);
void  search_by_date(char *username, char *date);
void  filter_by_category(char *username, char *category);
void  filter_by_type(char *username, char *filterType);


// ======================================================
// ================ Global user (set after login) =======
// ======================================================

char username[50] = "testuser";
char password[50] = "testpass";


// ======================================================
// ================ Menu Functions ======================
// ======================================================

void print_manage_transactions_menu() {

    int menuTransactionChoice;

    printf("\n\n\n\n========= Manage Transactions ==========\n");
    printf("1. Add Transaction\n");
    printf("2. Delete Transaction\n");
    printf("3. Update Transaction\n");
    printf("4. View Transactions\n");
    printf("5. Back to Main Menu\n");
    printf("\nEnter your choice: ");

    menuTransactionChoice = getchar();
    getchar();

    switch (menuTransactionChoice) {

        case '1': {
            add_transaction(username);
            break;
        }

        case '2': {
            int id;
            view_transactions(username);
            printf("\nEnter transaction ID to delete: ");
            scanf("%d", &id);
            while (getchar() != '\n');
            delete_transaction(username, id);
            break;
        }

        case '3': {
            int id;
            view_transactions(username);
            printf("Enter transaction ID to update: ");
            scanf("%d", &id);
            while (getchar() != '\n');
            update_transaction(username, id);
            break;
        }

        case '4': {
            view_transactions(username);
            break;
        }

        case '5': {
            return;
        }

        default: {
            printf("Invalid choice\n");
            break;
        }
    }
}


void print_search_filter_menu() {

    int menuSearchFilterChoice;

    printf("\n\n\n\n========= Search/Filter Transactions ==========\n");
    printf("1. Search by Date\n");
    printf("2. Filter by Category\n");
    printf("3. Filter by Type (Income/Expense)\n");
    printf("4. Back to Main Menu\n");
    printf("\nEnter your choice: ");

    menuSearchFilterChoice = getchar();
    getchar();

    switch (menuSearchFilterChoice) {

        case '1': {
            char searchDate[11];
            printf("Enter date (YYYY-MM-DD): ");
            scanf("%10s", searchDate);
            while (getchar() != '\n');
            search_by_date(username, searchDate);
            break;
        }

        case '2': {
            char category[30];
            printf("Enter category (exact match, e.g. Groceries): ");
            scanf("%29s", category);
            while (getchar() != '\n');
            filter_by_category(username, category);
            break;
        }

        case '3': {
            char t[10];
            printf("Enter type (income or expense): ");
            scanf("%9s", t);
            while (getchar() != '\n');
            filter_by_type(username, t);
            break;
        }

        case '4':
            return;

        default:
            break;
    }
}


void print_reports_menu() {

    int menuReportChoice;

    printf("\n\n\n\n========= Reports & Summaries ==========\n");
    printf("1. Generate Overall Report\n");
    printf("2. Categorized Breakdown\n");
    printf("3. Export Report to .txt\n");
    printf("4. Back to Main Menu\n");
    printf("\nEnter your choice: ");

    menuReportChoice = getchar();
    getchar();

    switch (menuReportChoice) {

        case '1':
            generate_overall_report(username);
            break;

        case '2': {
            char category[30];
            printf("Enter category: ");
            scanf("%s", category);
            while (getchar() != '\n');
            generate_category_report(username, category);
            break;
        }

        case '3':
            export_report_to_txt(username);
            break;

        case '4':
            return;

        default:
            break;
    }
}


void print_menu(int *running) {

    int menuChoice;

    printf("\n\n\n========= Main Menu ==========\n");
    printf("0. Exit\n");
    printf("1. Manage Transactions\n");
    printf("2. Search/Filter Transactions\n");
    printf("3. Reports & Summaries\n");
    printf("\nEnter your choice: ");

    menuChoice = getchar();
    getchar();

    switch (menuChoice) {

        case '0':
            printf("Goodbye!\n");
            *running = 0;
            break;

        case '1':
            print_manage_transactions_menu();
            break;

        case '2':
            print_search_filter_menu();
            break;

        case '3':
            print_reports_menu();
            break;

        default:
            printf("Invalid choice\n");
            break;
    }
}


void print_welcome_message() {
    printf("\n========================================\n");
    printf("   Welcome to Personal Finance Tracker   ");
    printf("\n========================================");
    printf("\nCreated by: Owen Fugger and Nish Patel\n\n");
}


// ======================================================
// ================== User Log in =======================
// ======================================================

void user_login_menu() {

    int loggedIn = 0;

    while (!loggedIn) {
        printf("\n0. Exit program");
        printf("\n1. Log in");
        printf("\n2. Register");
        printf("\n\nEnter your choice: ");

        int loginMenuChoice = getchar();
        // to clear the enter key from the input buffer
        getchar();

        char inputUsername[50];
        char inputPassword[50];

        switch (loginMenuChoice) {

            case '0':
                printf("Goodbye!\n");
                exit(0);
                break;

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

        if (loggedIn && loginMenuChoice != '3')
            strcpy(username, inputUsername);
    }
}


int main() {
    print_welcome_message();
    user_login_menu();

    int running = 1;
    while (running) {
        print_menu(&running);
    }

    return 0;
}
