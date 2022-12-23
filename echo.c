#include "headers.h"

int echo(char *ip)
{
    char *token;
    token = strtok(NULL, " \t");

    while (token != NULL)
    {
        printf("%s ", token);
        token = strtok(NULL, "\t");
    }

    printf("\n");

    return 0;
}