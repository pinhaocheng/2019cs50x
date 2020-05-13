#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // given constant size of one pic
    const int SIZE = 512;

    // used unsigned to hold 512
    unsigned char buffer[SIZE];

    // create pointer for output file
    FILE *outptr = NULL;

    //naming for each pic
    char name[8];

    //initialized number of image created
    int n = 0;

    while (fread(buffer, SIZE, 1, inptr) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] >= 0xe0 && buffer[3] <= 0xef)
        {
            // close previous image created
            if (n > 0)
            {
                fclose(outptr);
            }

            // create name for nth image
            sprintf(name, "%03i.jpg", n);

            // open image
            outptr = fopen(name, "w");
            if (outptr == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", name);
                return 3;
            }

            n++;
        }

        if (outptr != NULL)
        {
            fwrite(buffer, SIZE, 1, outptr);
        }
    }

    // close last image
    fclose(outptr);

    // close card.raw
    fclose(inptr);

    return 0;
}
