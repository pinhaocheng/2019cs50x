#include <cs50.h>
#include <stdio.h>

int get_posHeight_int(string prompt);

int main(void)
{
    int h = get_posHeight_int("Height: "); //var for height
    //going row by row
    for (int i = 0; i < h; i++) 
    {
        //printing empty spaces before the wall
        for (int j = 1; j < h - i; j++)
        {
            printf(" ");
        }
        
        //printing the left part wall
        for (int w = 0; w <= i; w++)
        {
            printf("#");
        }
        
        //print blank space
        printf("  ");
        
        //printing the right part wall
        for (int w = 0; w <= i; w++)
        {
            printf("#");
        }
               
        //printing next line
        printf("\n");
    }
}

//making sure the height entered is between 1 and 8 inclusive
int get_posHeight_int(string prompt)
{
    int n;
    do
    {
        n = get_int("%s", prompt);
    }
    while (n < 1 || n > 8);
    return n;
}
