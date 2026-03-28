/* transactions_csv.h
 * CSV parsing/writing utilities for the transactions files:
 *   id,date,category,description,amount,type
 */
#ifndef TELLGCCTHESEFUNCTIONS_C
#define TRANSACTIONS_CSV_H

#include <stdio.h>


// ======================================================
// ======================= Stucts =======================
// ======================================================
typedef struct {
    int id;
    char date[11];            /* YYYY-MM-DD (10) + null terminator */
    char category[30];
    char description[100];
    float amount;
    char type[10];           /* income | expense */
} Transaction;



// ======================================================
// ================ Function Prototypes =================
// ======================================================

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

// ---- transactions_csv.c ----
void sanitize_description(char *s);
int parse_transaction_line(const char *line, Transaction *t);
void write_transaction_line(FILE *f, const Transaction *t);
int transactions_next_id(const char *username);

#endif

