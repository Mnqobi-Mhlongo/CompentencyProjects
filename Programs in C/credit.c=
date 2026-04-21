#include <cs50.h>
#include <math.h>
#include <stdio.h>

int getLength(long cardNo);
bool isValidChecksum(int digits, long cardNo);
int getCardType(long cardNo, int digits);

// Main branch
int main(void)
{

    long n = get_long("Please enter your card number \n");
    int numberLength = getLength(n);
    int bankNumber = (getCardType((n), numberLength));
    if (isValidChecksum(numberLength, n) == true)
    {

        if (bankNumber == 1)
        {
            printf("MASTERCARD\n");
        }
        if (bankNumber == 2)
        {
            printf("AMEX\n");
        }
        if (bankNumber == 3)
        {
            printf("VISA\n");
        }
        if (bankNumber == 0)
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

// Get Length of number sequence to be used in loop and verification
int getLength(long cardNo)
{

    int digits = 0;
    while (cardNo != 0)
    {
        cardNo = cardNo / 10;
        digits++;
    }

    return digits;
}

// Convert long to string check sum of 2nd to last
bool isValidChecksum(int digits, long cardNo)
{
    int currentDigit = 0;
    int counter = 0;
    int currentSum = 0;
    int products = 0;

    for (int i = 0; i < digits; i++)
    {
        currentDigit = cardNo % 10;
        counter++;

        if (counter % 2 == 0)
        {
            if ((currentDigit * 2) > 9)
            {
                products = ((currentDigit * 2) % 10) + ((currentDigit * 2) / 10);
                currentSum += products;
            }
            else
            {
                currentSum = (currentDigit * 2) + currentSum;
            }
        }
        else
        {
            currentSum = currentDigit + currentSum;
        }

        cardNo = cardNo / 10;
    }
    if (currentSum % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int getCardType(long cardNo, int digits)
{
    int counter = 0;
    int currentDigit = 0;
    int bankVerification = 0;

    for (int i = 0; i < digits; i++)
    {
        currentDigit = cardNo % 10;
        counter++;
    }

    if (counter > 12 && counter < 17)
    {
        bankVerification = cardNo / pow(10, (digits - 2));

        if ((bankVerification == 51 || bankVerification == 52 || bankVerification == 53 ||
             bankVerification == 54 || bankVerification == 55) &&
            counter == 16)
        {
            return 1;
        }

        if ((bankVerification == 34 || bankVerification == 37) && counter == 15)
        {
            return 2;
        }

        else
        {

            bankVerification = bankVerification / 10;
            if ((bankVerification == 4) && (counter == 16 || counter == 13))
            {
                return 3;
            }
            else
            {
                return 0;
            }
            return 0;
        }
    }
    return 0;
}
