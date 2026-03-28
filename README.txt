Personal Finance Tracker
========================
Created by Owen Fugger and Nish Patel
Last updated: March 2026

Overview
--------
Personal Finance Tracker is a command-line application written in C that allows
users to register, log in, and manage their personal income and expenses. All data
is stored persistently using file operations, enabling users to track their finances
over time in a secure and organized manner.

Passwords are encrypted using the Hill Cipher — a matrix-based encryption algorithm
that uses modular arithmetic to protect user credentials.


Features
--------
- User registration and login with Hill Cipher password encryption
- Add, delete, update, and view transactions
- Search transactions by date
- Filter transactions by category or type (income / expense)
- Generate an overall financial report (total income, expenses, net balance)
- Generate a category breakdown report
- Export a summary report to a .txt file


File Structure
--------------
main.c               Entry point. Contains all menus and the main loop.
auth.c               User registration, login, and credential checking.
encryption.c         Hill Cipher password encryption.
transactions.c       Add, delete, update, and view transactions (CRUD).
transactions_csv.c   CSV parsing and writing utilities for transaction files.
transactions_csv.h   Header file for transactions_csv.c
reports.c            Report generation, search, and filter functions.
Makefile             Build configuration.

Data Files (generated at runtime, not to be modified manually)
--------------------------------------------------------------
users.txt                        Stores usernames and encrypted passwords.
transactions_<username>.txt      Stores all transactions for a given user.
report_<username>.txt            Generated when a report is exported.


How to Build and Run
--------------------
Requirements: GCC and a Linux-based terminal (e.g. Ubuntu)

Build:
    make

Run:
    make run

Clean build files:
    make clean


Usage
-----
On launch you will be prompted to log in or register:

    0. Exit program
    1. Log in
    2. Register

After logging in, the main menu provides access to:

    0. Exit
    1. Manage Transactions
       - Add, delete, update, or view your transactions
    2. Search / Filter Transactions
       - Search by date (YYYY-MM-DD)
       - Filter by category (exact match e.g. Groceries)
       - Filter by type (income or expense)
    3. Reports & Summaries
       - Overall report (total income, expenses, net balance)
       - Category breakdown
       - Export report to report_<username>.txt

Notes
-----
- Passwords are converted to lowercase before encryption.
  Passwords are not case sensitive.
- Transaction descriptions support spaces but not commas.
- Categories are case sensitive (e.g. "Groceries" not "groceries").
- Transaction IDs are assigned automatically and used for delete and update.


Contributing
------------
This project is not currently accepting pull requests.
It was created for academic purposes and may be opened in the future.