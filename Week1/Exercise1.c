#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char *trimString(char *str)
{
    int len = 0;
    len = sizeof(str) / sizeof(str[0]);
    for (int i = 0; i < len; i++)
    {
        if (str[i] == ' ')
        {
            for (int j = i; j < len; j++)
            {
                str[j] = str[j + 1];
            }
            len--;
        }
    }
    return str;
}

int main()
{
    char input[100];
    int check = 0;
    printf("Input the string: ");
    scanf("%[^\n]%*c", input);
    if (strlen(input) >= 15)
    {
        printf("Exceed 15 character!");
        return 0;
    }
    strcpy(input, trimString(input));
    for (int i = 0; input[i] != '\0'; i++)
    {
        if (isdigit(input[i]))
        {
            if ((check * 10) / 10 != check || check * 10 + (input[i] - '0') < 0)
            {
                printf("Error: Number too long");
                return 0;
            }
            check = check * 10 + (input[i] - '0');
        }
        else
        {
            printf("Not a numeric number");
            return 0;
        }
    }
    printf("%d", check);
    return 0;
}