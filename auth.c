/*
 * auth.c
 *
 * Handles all user authentication for the application.
 * Contains functions for registering new users, logging in,
 * and checking if a user already exists in users.txt
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// encrypt_password is defined in encryption.c
void encrypt_password(char *password, char *output);

// This function checks if a username already exists in users.txt
// Accepts a pointer to the username string to search for
// Returns 1 if the username is found, 0 if not found
int user_exists(char *username) {

    char line[256];

    // Open users.txt for reading "r"
    // If the file does not exist, no users have registered yet so return 0
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
        return 0;

    // Read through each line looking for a matching username
    // Each line is formatted as: username,encrypted_password
    while (fgets(line, sizeof(line), file) != NULL) {
        char stored_username[50];
        int i = 0;

        // Extract just the username part — everything before the comma
        while (line[i] != ',' && line[i] != '\0' && line[i] != '\n') {
            stored_username[i] = line[i];
            i++;
        }
        // Null terminate the extracted username so it can be compared
        stored_username[i] = '\0';

        // Compare the stored username with the one we are looking for
        // strcmp returns 0 if they match
        if (strcmp(stored_username, username) == 0) {
            fclose(file);
            return 1;   // username found
        }
    }

    fclose(file);
    return 0;   // username not found
}


// This function registers a new user by encrypting their password
// and storing their credentials in users.txt
// Accepts pointers to the username and password strings
// Returns 1 if registration was successful, 0 if the username is taken or an error occurs
int register_user(char *username, char *password) {

    // Check if the username is already taken before registering
    // user_exists reads through users.txt to check
    if (user_exists(username)) {
        printf("Username already taken. Please choose a different username.\n");
        return 0;
    }

    char encrypted[100];

    // Encrypt the password using the Hill Cipher before storing it
    // We never store the plaintext password — only the encrypted version
    encrypt_password(password, encrypted);

    // Open users.txt in append mode "a"
    // If the file does not exist, fopen will create it automatically
    // If it does exist, the new user will be added to the end
    FILE *file = fopen("users.txt", "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    // Write the username and encrypted password as a comma separated line
    // Format: username,encrypted_password
    fprintf(file, "%s,%s\n", username, encrypted);
    fclose(file);   // always close the file after writing to free system resources

    printf("User registered successfully!\n");
    return 1;
}


// This function verifies a users login credentials against users.txt
// Accepts pointers to the username and password strings entered by the user
// Returns 1 if the credentials match, 0 if not found or password is wrong
int login_user(char *username, char *password) {

    char line[256];
    char encrypted[100];

    // Encrypt the entered password so we can compare it to the stored encrypted version
    // We never decrypt — instead we encrypt the input and compare the two encrypted values
    encrypt_password(password, encrypted);

    // Open users.txt for reading "r"
    // If the file does not exist, no users have registered yet
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("No users found. Please register first.\n");
        return 0;
    }

    // Read through each line looking for a matching username and password
    // Each line is formatted as: username,encrypted_password
    while (fgets(line, sizeof(line), file) != NULL) {
        char stored_username[50];
        char stored_password[100];
        int i = 0, j = 0;

        // Extract the username — everything before the comma
        while (line[i] != ',' && line[i] != '\0' && line[i] != '\n') {
            stored_username[i] = line[i];
            i++;
        }
        stored_username[i] = '\0';  // null terminate the username
        i++;    // skip past the comma

        // Extract the password — everything after the comma until end of line.
        // On Windows text files are usually CRLF (\r\n), so stop on both.
        while (line[i] != '\0' && line[i] != '\n' && line[i] != '\r') {
            stored_password[j] = line[i];
            i++;
            j++;
        }
        stored_password[j] = '\0';  // null terminate the password

        // Check if both the username and encrypted password match
        // strcmp returns 0 if they match
        if (strcmp(stored_username, username) == 0 &&
            strcmp(stored_password, encrypted) == 0) {
            fclose(file);
            printf("Login successful! Welcome, %s.\n", username);
            return 1;   // credentials match — login success
        }
    }

    fclose(file);
    printf("Invalid username or password.\n");
    return 0;   // no matching credentials found — login failed
}