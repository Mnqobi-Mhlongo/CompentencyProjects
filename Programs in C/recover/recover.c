#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int BLOCK_SIZE = 512;
// Array of bytes of header
uint8_t buffer[BLOCK_SIZE];
y

int main(int argc, char *argv[])
{
    // Checks for one command line argument
    if (argc != 2)
    {
        printf("Usage: recover [file...]\n");
        return 1;
    }

    // Remember filenames
    char *inCard = argv[1];
    char outJPEG[8];

    // Open memory card
    FILE *inptr = fopen(inCard, "rb");

    // Ensure it can be opened
    if (inptr == NULL)
    {
        printf("File cannot be read %s.\n", inCard);
        return 1;
    }

    // Repeats read until the end of the card

    int jpegFound = 0;
    FILE *outptr = NULL;

    while (fread(buffer, 1, BLOCK_SIZE, inptr) == BLOCK_SIZE)
    {

        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] == 0xe0 || buffer[3] == 0xe1 || buffer[3] == 0xe2 || buffer[3] == 0xe3 ||
             buffer[3] == 0xe4))
        {

            if (jpegFound)
            {
                fclose(outptr);
            }

            sprintf(outJPEG, "%03i.jpg", jpegFound);
            printf("jpegFound = %d, filename = %s\n", jpegFound, outJPEG); // debug
            outptr = fopen(outJPEG, "wb");

            // Open JPEG
            if (outptr == NULL)
            {
                fclose(inptr);
                printf("Could not create %s.\n", outJPEG);
                return 1;
            }

            else
            {
                fwrite(buffer, 1, BLOCK_SIZE, outptr);
                jpegFound++;
            }
        }

        else
        {
            if (jpegFound)
            {
                fwrite(buffer, 1, BLOCK_SIZE, outptr);
            }
        }
    }

    // Close files
    fclose(inptr);
    fclose(outptr);
    return 0;
}
