#include "headers.h"

void print_fd(char *path, int ff, int df, char *target, int tf)
{
    DIR *d;
    struct dirent *dir;
    struct stat f;
    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char *temp = (char *)malloc(maxl * sizeof(char));
            strcpy(temp, path);
            strcat(temp, "/");
            strcat(temp, dir->d_name);
            stat(temp, &f);
            if (S_ISDIR(f.st_mode))
            {
                if (df)
                {
                    if (!tf || (tf && !strcmp(target, dir->d_name)))
                    {
                        if (dir->d_name[0] != '.')
                            printf("%s\n", temp);
                    }
                }
                if (dir->d_name[0] != '.')
                    print_fd(temp, ff, df, target, tf);
            }
            else
            {
                if (ff)
                {
                    if (!tf || (tf && !strcmp(target, dir->d_name)))
                    {
                        if (dir->d_name[0] != '.')
                            printf("%s\n", temp);
                    }
                }
            }
            free(temp);
        }
        closedir(d);
    }
}
int discover(char *str, char *root)
{
    char cur_dir[maxl], cpy[maxl], user[maxl];
    int df = 0, ff = 0;
    int count = 0;
    getcwd(cur_dir, maxl);
    strcpy(cpy, cur_dir);

    struct dirent *d;
    struct dirent **list;
    struct stat f;

    int i = 0;

    char cmd[maxl];

    char **token_arr = (char **)malloc(maxl * sizeof(char *));
    token_arr[i] = strtok(NULL, "\n");
    if (token_arr[i] == NULL)
    {
        token_arr[i] = (char *)malloc(sizeof(char) * 10);
        strcpy(token_arr[i], ".");
    }
    char **copy = (char **)malloc(maxl * sizeof(char *));
    char **copy2 = (char **)malloc(maxl * sizeof(char *));
    char **copy3 = (char **)malloc(maxl * sizeof(char *));

    copy2[i] = token_arr[i];
    i = 0;
    copy[i] = strtok(copy2[i], "\n");
    copy3[i] = copy[i];
    int leng = strlen(copy3[i]);
    printf("%d\n", leng);
    int j;
    for (j = 0; j < leng; j++)
    {
        if (copy[i][j] == '-' && copy[i][j + 1] == 'f')
        {
            ff = 1;
        }
        else if (copy[i][j] == '-' && copy[i][j + 1] == 'd')
        {
            df = 1;
        }
    }
    if (!df && !ff)
    {
        df = 1;
        ff = 1;
    }
    printf("%d %d\n", ff, df);
    i = 0;
    copy[i] = strtok(copy3[i], " \t");
    while (copy[i] != NULL)
    {
        i++;
        copy[i] = strtok(NULL, " \t");
    }
    int pl = i;
    i = 0;
    char temp[maxl];
    char path[maxl];
    int target_flag = 0, dir_flag = 0;
    while (copy[i] != NULL)
    {
        if (copy[i][0] != '-')
        {
            if (copy[i][0] != 34)
            {
                if (!strcmp(copy[i], "~"))
                {
                    strcpy(path, root);
                }
                else if (!strcmp(copy[i], ".."))
                {
                    char cpy[maxl];
                    strcpy(cpy, path);
                    chdir("..");
                    getcwd(path, maxl);
                }
                else
                {
                    strcpy(path, ".");
                    if (strcmp(copy[i], "."))
                    {
                        strcat(path, "/");
                        strcat(path, copy[i]);
                    }
                }
                dir_flag = 1;
            }
            else
            {
                strncpy(temp, &copy[i][1], strlen(copy[i]) - 2);
                printf("%s\n", temp);
                target_flag = 1;
            }
        }
        i++;
    }
    if (dir_flag)
    {
        print_fd(path, ff, df, temp, target_flag);
    }
    else
    {
        print_fd(".", ff, df, temp, target_flag);
    }
    chdir(cpy);
}