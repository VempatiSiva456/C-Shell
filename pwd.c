#include "headers.h"

int get_pwd()
{
    char dir[maxl];
    char *temp = getcwd(dir, maxl);

    if (temp != NULL)
        printf("%s\n", dir);
    else
    {
        perror("error getting current directory\n");
        return -1;
    }

    return 0;
}