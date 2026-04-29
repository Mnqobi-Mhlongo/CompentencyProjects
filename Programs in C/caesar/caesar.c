#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isDigit(char *plainText);
char *cipherText(const char *plainText, int k);
char rotate(char c, int k);

int main(int argc, string argv[])
{
    // Make sure program was run with just one command-line argument

    // Make sure every character in argv[1] is a digit
    if (argc == 2 && isDigit(argv[1]) == true)
    {

        // Convert argv[1] from a `string` to an `int`
        int k = atoi(argv[1]);

        // Prompt user for plaintext
        string plainText = get_string("Plaintext:  \n");
        char *result = cipherText(plainText, k);
        printf("ciphertext: %s\n", result);
        free(result);
        return 0;
    }

    else
    {

        printf("Usage: ./caesar key \n");
        printf("Missing command-line argument \n");
        return 1;
    }
}

bool isDigit(char *plainText)
{
    for (int i = 0; plainText[i] != '\0'; i++)
    {
        // Checks if plaintext has a digit
        if (!isdigit(plainText[i]))
        {
            return false;
        }
    }
    return true;
}

// Rotate the character if it's a letter
char rotate(char c, int k)
{
    if (isupper(c))
    {
        return (char) (((c - 'A') + k) % 26 + 'A');
    }

    else if (islower(c))
    {
        return (char) (((c - 'a') + k) % 26 + 'a');
    }

    else
    {
        return c;
    }
}


char *cipherText(const char *plainText, int k)
{

    int l = strlen(plainText);

    char *cipherTextOutput = malloc(l + 1);

    // Loops through the array plainText
    for (int i = 0; i < l; i++)
    {
        // For each character in the plaintext:
        char c = plainText[i];

        // Rotate the character if it's a letter

        char rotatedLetter = rotate(c, k);
        cipherTextOutput[i] = rotatedLetter;
    }
    cipherTextOutput[l] = '\0';
    return cipherTextOutput;
}
