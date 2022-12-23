#include "headers.h"

int cd(char *prev_dir, char *root)
{
    int fg = 0, check = 0, flag = 0;
    char *path, *new_path, *token;
    char dir[maxl], cpy[maxl];

    getcwd(dir, maxl);

    token = strtok(NULL, " \t");
    path = token;

    if (token == NULL)
    {
        strcpy(cpy, root);
        fg = 1;
    }
    else
    {
        if (!strcmp(path, "-"))
            flag = 1;
        else if (!strcmp(path, "~"))
            path = root;
        else if (token == NULL)
            path = root;
        else
        {
            if (path[0] == '~' && path[1] == '/')
            {
                strcpy(cpy, root);
                strcat(cpy, &path[1]);
                fg = 1;
            }
        }
        token = strtok(NULL, " \t");
    }

    if (token == NULL)
    {
        if (!fg)
            new_path = path;
        else
            new_path = cpy;
    }
    else
    {
        perror("Excessive arguments for cd\n");
        return 0;
    }

    if (flag == 0)
        check = chdir(new_path);
    else
    {
        printf("%s\n", prev_dir);
        check = chdir(prev_dir);
    }

    if (check == 0)
    {
        if (strcmp(new_path, dir) != 0)
            strcpy(prev_dir, dir);
    }
    else
        perror("Error\n");

    return 0;
}