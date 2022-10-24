#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{
    char inputCharacter;
    int numericCount = 0;
    int characterCount = 0;
    int nextLineCount = 0;
    FILE *inputFile = fopen("inputFile.txt", "r");
    while ((inputCharacter = fgetc(inputFile)) != EOF)
    {
        if (isdigit(inputCharacter))
        {
            numericCount++;
        }
        if (isalpha(inputCharacter))
        {
            characterCount++;
        }
        if (inputCharacter=='\n')
        {
            nextLineCount++;
        }
    }
    printf(" %d numbers \n %d characters \n %d line. \n", numericCount, characterCount,nextLineCount);
    fclose(inputFile);
}