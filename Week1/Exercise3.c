#include <stdio.h>
#include "charcode.h"

extern CharCode charCodes[];
int main()
{
    char inputCharacter;
    int numericCount = 0;
    int characterCount = 0;
    int nextLineCount = 0;
    FILE *inputFile = fopen("inputFile.txt", "r");
    while ((inputCharacter = fgetc(inputFile)) != EOF)
    {
        if (inputCharacter == '\n')
        {
            nextLineCount++;
        }
        inputCharacter = charCodes[(int)inputCharacter];
        switch (inputCharacter)
        {
        case CHAR_DIGIT:
            numericCount++;
            break;
        default:
            characterCount++;
            break;
        }
        
    }
    printf(" %d numbers \n %d characters \n %d line. \n", numericCount, characterCount, nextLineCount);
    fclose(inputFile);
}