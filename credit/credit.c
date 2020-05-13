#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long oriNumb = get_long("Number: ");
    
    //check the length of the cc number
    long checkNumb = oriNumb;
    int lengthOfNumb = 0;
    while (checkNumb > 0)
    {
        checkNumb = checkNumb / 10;
        lengthOfNumb++;
    }
    checkNumb = oriNumb;
    
    //display invalid for wrong length for cc number    
    if (lengthOfNumb != 13 && lengthOfNumb != 15 && lengthOfNumb != 16)
    {
        printf("INVALID\n");
    }
    //check for card type
    else
    {
        //check Luhn's Algo, 0 being last digit entered
        int digit[lengthOfNumb];
        for (int i = 0; i < lengthOfNumb; i++)
        {
            digit[i] = checkNumb % 10;
            checkNumb = checkNumb / 10;
        }
        int checkSum = 0;
        for (int i = 0; i < lengthOfNumb; i++)
        {
            if (i % 2 != 0)
            {
                int checkSum_0 = 2 * digit[i];
                int checkSum_1 = checkSum_0 / 10;
                int checkSum_2 = checkSum_0 % 10;
                checkSum = checkSum + checkSum_1 + checkSum_2;
            }
            else
            {
                checkSum = checkSum + digit[i];    
            }
        }
        
        //check if checkSum is dividable by 0
        if (checkSum % 10 != 0)
        {
            printf("INVALID\n");
        }
        else 
        {
            //check AMEX
            if (lengthOfNumb == 15 && digit[14] == 3 && (digit[13] == 4 || digit[13] == 7))
            {
                printf("AMEX\n");
            }
            //check MASTERCARD
            else if (lengthOfNumb == 16 && digit[15] == 5 && digit[14] > 0 && digit[14] < 6)
            {
                printf("MASTERCARD\n");
            }
            //check VISA
            else if ((lengthOfNumb == 13 || lengthOfNumb == 16) && (digit[12] == 4 || digit[15] == 4))
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
    }
}
