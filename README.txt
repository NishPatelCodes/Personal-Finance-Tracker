Project Title 
Personal Finance Tracker 

What Will Your Application Do? 

The Personal Finance Tracker is a command-line application written in C that allows users to register, log in, and manage their personal income and expenses. All data is stored persistently using file operations, enabling users to track their finances over time in a secure and organized manner. 
User Authentication 
Users can register a new account with a username and password 
Users can log in securely; each user can only access their own data 
Passwords are stored in hashed form in a binary file 
Transaction Management (File Insert / Delete / Update) 
Insert: Add a new income or expense transaction 
Delete: Delete an existing transaction by ID 
Update: Edit a transaction's amount, category, date, or description 
All transactions are stored persistently in a per-user binary data file 
Search & Filter Records 
Search transactions by date range 
Filter transactions by category (e.g. Food, Rent, Transport) 
Filter by transaction type: Income or Expense 
Reports & Summaries 
View a monthly summary showing total income, total expenses, and net balance 
View a category breakdown of spending 
Export a summary report to a .txt file for external viewing 
