#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int shift(char c);

int main(int argc, string argv[])
{
    //making sure there's only one input 
    if (argc != 2)
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
    else
    {
        //check if the key is pure text
        int n = strlen(argv[1]);
        for (int i = 0; i < n; i++)
        {
            int j = (int) argv[1][i];
            if (j < 65 || (j > 90 && j < 97) || j > 122)
            {
                printf("Usage: ./vigenere keyword\n");
                return 1;            
            }
        }
            
        //prompt for plain text
        string st = get_string("plaintext: ");
        int pn = strlen(st);
        printf("ciphertext: ");

        //going through plaintest word by word
        for (int i = 0, p = 0; i < pn; i++, p++)
        {
            int plain = (int) st[i];            
            
            //get the order of alphabet in the key
            int k = p % n;
            int key = shift(argv[1][k]);
       
            //start cipher
            int output;
            output = (plain + key) % 26;
            
            //capital letter
            if (plain > 64 && plain < 91)
            {
                if (output < 13)
                {
                    output = 78 + output;
                }
                else
                {
                    output = 65 - 13 + output;
                }
            }
            
            //normal letter
            else if (plain > 96 && plain < 123)
            {
                if (output < 19)
                {
                    output = 104 + output;
                }
                else
                {
                    output = 97 - 19 + output;
                }
            }
            
            //other stuff
            else
            {
                output =  plain;
                //offset other character
                p--;
            }
            //p++;
            char cipher = (char) output;
            printf("%c", cipher);
        }
    }
    printf("\n");
}

int shift(char c)
{
    int keyOut = (int) c;       
    int keyMod = keyOut % 26;
    
    //capital letter
    if (keyOut > 64 && keyOut < 91)
    {
        if (keyMod < 13)
        {
            keyOut = 78 + keyMod - 65;
        }
        else
        {
            keyOut = keyMod - 13;
        }
    }

    //normal letter
    else if (keyOut > 96 && keyOut < 123)
    {
        if (keyMod < 19)
        {
            keyOut = 104 + keyMod - 97;
        }
        else
        {
            keyOut = keyMod - 19;
        }
    }
    return keyOut;
}
