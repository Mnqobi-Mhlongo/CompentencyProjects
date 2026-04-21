#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

float countLetters(string text);
int countWords(string text);
float countSentences(string text);

int main(void)
{
    // Prompt the user for some text
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text
    int letters = countLetters(text);
    int words = countWords(text);
    int sentences = countSentences(text);

    // Compute the Coleman-Liau index

    float L = ((float) letters / (float) words) * 100;
    float S = ((float) sentences / (float) words) * 100;
    float index = (0.0588 * L - 0.296 * S - 15.8);
    if (index > 16)
    {
        printf(" Grade: 16+ \n");
        // Print the grade level
    }
    else
    {
        printf(" Grade: %f \n", round(index));

        // Print the grade level
    }
}

float countLetters(string text)
{

    int counter = 0;
    int letters = 0;

    // Return the number of letters in text

    for (int i = 0, t = strlen(text); i < t; i++)
    {
        if (text[i] != ' ')
        {
            counter++;
        }
        else
        {
            continue;
        }
    }
    letters = counter;

    return letters;
}

int countWords(string text)
{
    int words = 0;
    int whiteSpaces = 0;

    for (int i = 0, t = strlen(text); i < t; i++)
    {
        if (text[i] != ' ')
        {
            continue;
        }
        else
        {
            whiteSpaces++;
        }

        words = whiteSpaces + 1;
    }
    // Return the number of words in text

    return words;
}

float countSentences(string text)
{
    int sentences = 0;

    for (int i = 0, t = strlen(text); i < t; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
        else
        {
            continue;
        }
    }

    // Return the number of sentences in text
    return sentences;
}
