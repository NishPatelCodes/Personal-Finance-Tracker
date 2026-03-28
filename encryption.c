/*
 * encryption.c
 * 
 * Handles password encryption and decryption using the Hill Cipher.
 * Uses matrix multiplication with modular arithmetic to encrypt
 * passwords before storing them in users.txt
 */

 // Include necessary headers
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tellGCCthesefunctions.h"

// Macros, replaces all instances before the code compiles
#define MATRIX_SIZE 2
#define MOD 26


// ------------ Top Seceret ------------
// Secret Key Matrix. This is what is used to encrypt the users information
// Not the best idea to have it hard coded into the public code. Big security risk
int key[MATRIX_SIZE][MATRIX_SIZE] = {
    {3, 3},
    {2, 5}
};



// Encrypts a plaintext password using the Hill Cipher and stores the result in output
// Called during registration and login to avoid storing or comparing plaintext passwords
void encrypt_password(char *password, char *output) {


    // Variables, the length of the password (before encryption) and placeholder variables
    int len = strlen(password);
    int i, j, k;

    // Convert password to lowercase before encrypting
    // The Hill Cipher only works with lowercase letters (a=0 to z=25)
    // Uppercase letters would produce negative numbers when subtracting 'a'
    for (i = 0; i < len; i++)
        if (password[i] >= 'A' && password[i] <= 'Z')
            password[i] = password[i] + 32;

    // Checking if the password has an even amount of characters
    // If its even carry on, else if its odd we need to add a letter to ensure its even
    // Hill cipher requires it to be even
    if (len % MATRIX_SIZE != 0) {
        password[len] = 'a';
        password[len + 1] = '\0';
        len++;
    }


    // The Hill Cipher processes 2 characters at a time (block size = MATRIX_SIZE)
    // Each iteration of this loop handles one block of 2 characters
    for (i = 0; i < len; i += MATRIX_SIZE) {
        int block[MATRIX_SIZE];

        // Convert each character in the block to a number
        // This is done by subtracting 'a' from the character
        // e.g. 'a' - 'a' = 0, 'b' - 'a' = 1, 'c' - 'a' = 2 ...
        // password[i + j] accesses the current pair of characters
        for (j = 0; j < MATRIX_SIZE; j++)
            block[j] = password[i + j] - 'a';

        // Matrix multiplication: multiply the key matrix by the block vector
        // Each row j of the key matrix is dot producted with the block vector
        // e.g. for a 2x2 key matrix and block [b0, b1]:
        //      output[0] = (key[0][0] * b0 + key[0][1] * b1) % 26
        //      output[1] = (key[1][0] * b0 + key[1][1] * b1) % 26
        for (j = 0; j < MATRIX_SIZE; j++) {
            int sum = 0;
            
            // Dot product of row j of the key matrix with the block vector
            // Each element of the row is multiplied by the corresponding block element
            for (k = 0; k < MATRIX_SIZE; k++)
                sum += key[j][k] * block[k];

            // Apply mod 26 to keep the result within the alphabet range (0-25)
            // then convert back to a character by adding 'a'
            // e.g. 0 + 'a' = 'a', 1 + 'a' = 'b', 2 + 'a' = 'c' ...
            output[i + j] = (sum % MOD) + 'a';
        }
    }
    // Null terminate the output string so it can be used as a regular C string
    output[len] = '\0';
}



void decrypt_password(char *encrypted, char *output) {
    (void)encrypted;    // suppress unused parameter warning — not implemented yet
    (void)output;       // suppress unused parameter warning — not implemented yet
    // TODO: implement decryption
}

int mod_inverse(int a, int mod) {
    (void)a;            // suppress unused parameter warning — not implemented yet
    (void)mod;          // suppress unused parameter warning — not implemented yet
    // TODO: implement modular inverse
    return 0;
}