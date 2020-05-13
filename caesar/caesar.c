#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    //making sure there's only one input 
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        //check if the string is pure number
        int n = strlen(argv[1]);
        for (int i = 0; i < n; i++)
        {
            int j = (int) argv[1][i];
            if (j > 57)
            {
                printf("Usage: ./caesar key\n");
                return 1;            
            }
        }
        
        //convert pure number string to int
        int k = atoi(argv[1]);
        
        //prompt for plain text
        string st = get_string("plaintext: ");
        int pn = strlen(st);
        printf("ciphertext: ");

        //start cipher
        int output;
        for (int i = 0; i < pn; i++)
        {
            int plain = (int) st[i];
            output = (plain + k) % 26;
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
            }
            char cipher = (char) output;
            printf("%c", cipher);
        }
    }
    printf("\n");
}
