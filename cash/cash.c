#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)  
{
    float dollars;
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0);
    
    int cents = round(dollars * 100);
    int change = 0;
    while (cents >= 25)
    {
        cents = cents - 25;
        change = change + 1;
    }
    
    while (cents >= 10)
    {
        cents = cents - 10;
        change = change + 1;
    }
        
    while (cents >= 5)
    {
        cents = cents - 5;
        change = change + 1;
    }
            
    while (cents >= 1)
    {
        cents = cents - 1;
        change = change + 1;
    }
 
    printf("%i\n", change);
}
