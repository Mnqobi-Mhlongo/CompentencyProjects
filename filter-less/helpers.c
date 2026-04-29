#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // Update pixel values

            double average =
                (round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0));

            // Update pixel values

            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // Update pixel values

            double sepiaRed = (round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen +
                                     .189 * image[i][j].rgbtBlue));
            double sepiaGreen = (round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen +
                                       .168 * image[i][j].rgbtBlue));
            double sepiaBlue = (round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen +
                                      .131 * image[i][j].rgbtBlue));

            // Update pixel values
            if (sepiaRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = sepiaRed;
            }

            if (sepiaGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = sepiaGreen;
            }

            if (sepiaBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = sepiaBlue;
            }
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels

    RGBTRIPLE copy[height][width];

    // Make a copy of immage

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Read from copy

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // Swap places
            image[i][j] = copy[i][width - j - 1];
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE copy[height][width];
    // Loop over all pixels

    int i;
    int j;

    // Make a copy of immage

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Read from copy
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {

            int totalRed = 0;
            int totalGreen = 0;
            int totalBlue = 0;

            int counter = 0;
            for (int di = -1; di <= 1; di++)
            {

                // Establish confines of pixels

                for (int dj = -1; dj <= 1; dj++)
                {
                    if (i + di >= 0 && i + di < height && j + dj >= 0 && j + dj < width)
                    {
                        totalRed = totalRed + copy[i + di][j + dj].rgbtRed;
                        totalGreen = totalGreen + copy[i + di][j + dj].rgbtGreen;
                        totalBlue = totalBlue + copy[i + di][j + dj].rgbtBlue;
                        counter++;
                    }
                }
            }

            // Work out the average for the pixel

            double averageRed = round((double) totalRed / counter);
            double averageGreen = round((double) totalGreen / counter);
            double averageBlue = round((double) totalBlue / counter);

            // Assign to original image

            image[i][j].rgbtRed = averageRed;
            image[i][j].rgbtGreen = averageGreen;
            image[i][j].rgbtBlue = averageBlue;
        }
    }

    return;
}
