#include "headers.h"

int read_history(char **cmd, char *root)
{
    char path[maxl], user[maxl];
    memset(path, 0, maxl);
    getlogin_r(user, maxl);
    strcat(path, root);
    strcat(path, "/history.txt");

    size_t size = maxl;

    int i, nc = 0;

    cmd[0] = NULL;

    FILE *f = NULL;
    f = fopen(path, "r");

    if (f != NULL)
    {
        i = 1;
        while (i < flimit)
        {
            cmd[i] = NULL;
            if (getline(&cmd[i - 1], &size, f) > 0)
                nc++;
            else
                break;
            i++;
        }
        fclose(f);
    }
    return nc;
}
void add_history(char *ip, char *root)
{
    char path[maxl], user[maxl];
    memset(path, 0, maxl);
    getlogin_r(user, maxl);
    strcat(path, root);
    strcat(path, "/history.txt");

    char temp[maxl];
    memset(temp, 0, maxl);
    strcat(temp, ip);
    strcat(temp, "\n");

    char *cmd[flimit];

    int nc = read_history(cmd, root);

    if (!(nc && !strcmp(temp, cmd[nc - 1])))
    {

        FILE *of;
        of = fopen(path, "w");

        int i;

        if (of != NULL)
        {
            if (nc && nc < flimit - 1)
                fprintf(of, "%s", cmd[0]);
            i = 1;
            while (i < nc)
            {
                fprintf(of, "%s", cmd[i]);
                i++;
            }

            fprintf(of, "%s", ip);
            fprintf(of, "\n");
        }
        fclose(of);
    }
}

void show_history(char *limitStr, char *root)
{
    char cpy[maxl];
    strcpy(cpy, limitStr);
    int limit = 10;

    char *token;
    token = strtok(NULL, " \t");

    if (token)
        limit = atoi(token);

    if (limit > 10)
    {
        perror("Error\n");
    }
    else
    {
        char *cmd[flimit];

        int nc = read_history(cmd, root);

        if (nc < limit)
            limit = nc;

        for (int i = nc - limit; i < nc; i++)
            printf("%s", cmd[i]);
    }
}