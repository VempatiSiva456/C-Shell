#include "headers.h"

int flag1 = 0, flag2 = 0;

int calcblocks(char *dir, int total)
{
    struct stat f = {0};
    stat(dir, &f);
    total += (f.st_blocks) / 2;
    return total;
}

int printinfo(char *dir, char *fname)
{
    char time[50];

    struct stat f = {0};
    struct passwd *p;
    struct group *g;

    stat(dir, &f);
    p = getpwuid(f.st_uid);
    g = getgrgid(f.st_gid);
    strftime(time, sizeof(time), "%b %d %H:%M", localtime(&f.st_mtime));
    if (S_ISDIR(f.st_mode))
        printf("d");
    else
        printf("-");
    if (f.st_mode & S_IRUSR)
        printf("r");
    else
        printf("-");
    if (f.st_mode & S_IWUSR)
        printf("w");
    else
        printf("-");
    if (f.st_mode & S_IXUSR)
        printf("x");
    else
        printf("-");
    if (f.st_mode & S_IRGRP)
        printf("r");
    else
        printf("-");
    if (f.st_mode & S_IWGRP)
        printf("w");
    else
        printf("-");
    if (f.st_mode & S_IXGRP)
        printf("x");
    else
        printf("-");
    if (f.st_mode & S_IROTH)
        printf("r");
    else
        printf("-");
    if (f.st_mode & S_IWOTH)
        printf("w");
    else
        printf("-");
    if (f.st_mode & S_IXOTH)
        printf("x");
    else
        printf("-");
    printf(" %ld", f.st_nlink);
    printf(" %s", p->pw_name);
    printf(" %s", g->gr_name);
    printf(" %8ld ", f.st_size);
    printf(" %s", time);
    if (S_ISDIR(f.st_mode))
        printf("\e[1;34m");
    else if ((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH))
        printf("\e[1;32m");
    printf(" %s\n", fname);
    if (((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH)) || S_ISDIR(f.st_mode))
    {
        printf("\033[0m");
    }
}

int ls(char *ip, char *root)
{
    char cur_dir[maxl], cpy[maxl];
    int af = 0, lf = 0;
    int count = 0;

    getcwd(cur_dir, maxl);

    DIR *z = opendir(cur_dir);
    struct dirent *d = {0};
    struct dirent **list = {0};
    struct stat f = {0};

    int total = 0;

    int i = 0, tc[maxl], flag_arr[maxl];

    memset(flag_arr, 0, sizeof(flag_arr));

    char **token_arr = (char **)malloc(10000 * sizeof(char *));
    token_arr[i] = strtok(NULL, "\n");
    char **copy = (char **)malloc(10000 * sizeof(char *));
    char **copy2 = (char **)malloc(10000 * sizeof(char *));
    char **copy3 = (char **)malloc(10000 * sizeof(char *));

    copy2[i] = token_arr[i];

    if (token_arr[i] == NULL)
    {
        int size = scandir(cur_dir, &list, 0, alphasort);
        for (int j = 0; j < size; j++)
        {
            d = list[j];

            if (d->d_name[0] != '.')
            {
                stat(d->d_name, &f);
                if (S_ISDIR(f.st_mode))
                    printf("\e[1;34m");
                else if ((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH))
                    printf("\e[1;32m");

                printf("%s\n", d->d_name);

                if (((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH)) || S_ISDIR(f.st_mode))
                    printf("\033[0m");
            }
        }
    }
    else
    {
        i = 0;
        copy[i] = strtok(copy2[i], "\n");
        // printf("%s\n", copy[i]);
        copy3[i] = copy[i];
        int leng = strlen(copy[i]);
        for (int j = 0; j < leng; j++)
        {
            if ((copy[i][j] == '-' && copy[i][j + 1] == 'l' && copy[i][j + 2] == 'a') || (copy[i][j] == '-' && copy[i][j + 1] == 'a' && copy[i][j + 2] == 'l'))
            {
                lf = 1;
                af = 1;
            }
            else if (copy[i][j] == '-' && copy[i][j + 1] == 'a')
                af = 1;
            else if (copy[i][j] == '-' && copy[i][j + 1] == 'l')
                lf = 1;
        }
        // printf("%d %d\n", lf, af);
        i = 0;
        copy[i] = strtok(copy3[i], " \t");
        while (copy[i] != NULL)
        {
            i++;
            copy[i] = strtok(NULL, " \t");
        }
        i = 0;
        while (copy[i] != NULL)
        {
            // printf("%s\n", copy[i]);
            if (copy[i][0] != '-')
            {
                if (!strcmp(copy[i], "~"))
                {
                    1;
                    strcpy(cpy, cur_dir);
                    chdir(root);
                    getcwd(cur_dir, maxl);
                    int size = scandir(cur_dir, &list, 0, alphasort);
                    for (int j = 0; j < size; j++)
                    {
                        d = list[j];
                        if (lf && !af)
                        {
                            if (d->d_name[0] != '.')
                            {
                                total += calcblocks(d->d_name, 0);
                            }
                        }
                        else if (lf && af)
                        {
                            total += calcblocks(d->d_name, 0);
                        }
                    }
                    chdir(cpy);
                    getcwd(cur_dir, maxl);
                    count++;
                }
                else if (!strcmp(copy[i], "."))
                {
                    1;
                    int size = scandir(cur_dir, &list, 0, alphasort);
                    for (int j = 0; j < size; j++)
                    {
                        d = list[j];
                        if (lf && !af)
                        {
                            if (d->d_name[0] != '.')
                                total += calcblocks(d->d_name, 0);
                        }
                        else if (lf && af)
                        {
                            total += calcblocks(d->d_name, 0);
                        }
                    }
                    count++;
                }
                else if (!strcmp(copy[i], ".."))
                {
                    1;
                    strcpy(cpy, cur_dir);
                    chdir("..");
                    getcwd(cur_dir, maxl);
                    int size = scandir(cur_dir, &list, 0, alphasort);
                    for (int j = 0; j < size; j++)
                    {
                        d = list[j];
                        if (lf && !af)
                        {
                            if (d->d_name[0] != '.')
                                total += calcblocks(d->d_name, 0);
                        }
                        else if (lf && af)
                        {
                            total += calcblocks(d->d_name, 0);
                        }
                    }
                    chdir(cpy);
                    getcwd(cur_dir, maxl);
                    count++;
                }
                else
                {
                    stat(copy[i], &f);
                    if (S_ISDIR(f.st_mode))
                    {
                        1;
                        int size = scandir(copy[i], &list, 0, alphasort);
                        z = opendir(copy[i]);
                        if (z)
                        {
                            for (int j = 0; j < size; j++)
                            {
                                d = list[j];
                                if (lf && !af)
                                {
                                    if (d->d_name[0] != '.')
                                    {
                                        char curr[maxl], cat[maxl];
                                        getcwd(curr, maxl);
                                        strcpy(cat, curr);
                                        strcat(cat, "/");
                                        strcat(cat, copy[i]);
                                        strcat(cat, "/");
                                        strcat(cat, d->d_name);
                                        total += calcblocks(cat, 0);
                                    }
                                }
                                else if (lf && af)
                                {
                                    char curr[maxl], cat[maxl];
                                    getcwd(curr, maxl);
                                    strcpy(cat, curr);
                                    strcat(cat, "/");
                                    strcat(cat, copy[i]);
                                    strcat(cat, "/");
                                    strcat(cat, d->d_name);
                                    total += calcblocks(cat, 0);
                                }
                            }
                        }
                        count++;
                    }
                    else
                    {
                        flag2 = 1;
                        if (!access(copy[i], F_OK))
                        {
                            if (lf && !af)
                            {
                                if (strcmp(copy[i], ".") && strcmp(copy[i], ".."))
                                    total += calcblocks(copy[i], 0);
                            }
                            else if (lf && af)
                            {
                                total += calcblocks(copy[i], 0);
                            }
                        }
                        count++;
                    }
                }
                if (!flag2)
                    flag_arr[i] = 1;
                tc[i] = total;
                total = 0;
                flag2 = 0;
            }
            i++;
        }
        int total_f = 0;
        if (!count)
        {
            flag1 = 1;
            int size = scandir(cur_dir, &list, 0, alphasort);
            for (int j = 0; j < size; j++)
            {
                d = list[j];
                if (lf && !af)
                {
                    if (d->d_name[0] != '.')
                        total += calcblocks(d->d_name, 0);
                }
                else if (lf && af)
                {
                    total += calcblocks(d->d_name, 0);
                }
            }
            total_f += total;
        }
        int pc = count;
        count = 0;
        i = 0;

        while (copy[i] != NULL)
        {
            if (copy[i][0] != '-')
            {
                if (!strcmp(copy[i], "~"))
                {
                    if (pc > 1)
                        printf("\n%s:\n", copy[i]);
                    if (lf && flag_arr[i])
                    {
                        printf("total %d\n", tc[i]);
                    }
                    strcpy(cpy, cur_dir);
                    chdir(root);
                    getcwd(cur_dir, maxl);
                    int size = scandir(cur_dir, &list, 0, alphasort);
                    for (int j = 0; j < size; j++)
                    {
                        d = list[j];
                        if (!lf && !af)
                        {
                            if (d->d_name[0] != '.')
                            {
                                stat(d->d_name, &f);
                                if (S_ISDIR(f.st_mode))
                                    printf("\e[1;34m");
                                else if ((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH))
                                    printf("\e[1;32m");

                                printf("%s\n", d->d_name);

                                if (((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH)) || S_ISDIR(f.st_mode))
                                    printf("\033[0m");
                            }
                        }
                        else if (lf && !af)
                        {
                            if (d->d_name[0] != '.')
                            {
                                printinfo(d->d_name, d->d_name);
                            }
                        }
                        else if (!lf && af)
                        {
                            stat(d->d_name, &f);
                            if (S_ISDIR(f.st_mode))
                                printf("\e[1;34m");
                            else if ((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH))
                                printf("\e[1;32m");

                            printf("%s\n", d->d_name);

                            if (((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH)) || S_ISDIR(f.st_mode))
                                printf("\033[0m");
                        }
                        else if (lf && af)
                        {
                            printinfo(d->d_name, d->d_name);
                        }
                    }
                    chdir(cpy);
                    getcwd(cur_dir, maxl);
                    count++;
                }
                else if (!strcmp(copy[i], "."))
                {
                    if (pc > 1)
                        printf("\n%s:\n", copy[i]);
                    if (lf && flag_arr[i])
                    {
                        printf("total %d\n", tc[i]);
                    }
                    int size = scandir(cur_dir, &list, 0, alphasort);
                    for (int j = 0; j < size; j++)
                    {
                        d = list[j];
                        if (!lf && !af)
                        {
                            if (d->d_name[0] != '.')
                            {
                                stat(d->d_name, &f);
                                if (S_ISDIR(f.st_mode))
                                    printf("\e[1;34m");
                                else if ((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH))
                                    printf("\e[1;32m");

                                printf("%s\n", d->d_name);

                                if (((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH)) || S_ISDIR(f.st_mode))
                                    printf("\033[0m");
                            }
                        }
                        else if (lf && !af)
                        {
                            if (d->d_name[0] != '.')
                            {
                                printinfo(d->d_name, d->d_name);
                            }
                        }
                        else if (!lf && af)
                        {
                            stat(d->d_name, &f);
                            if (S_ISDIR(f.st_mode))
                                printf("\e[1;34m");
                            else if ((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH))
                                printf("\e[1;32m");

                            printf("%s\n", d->d_name);

                            if (((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH)) || S_ISDIR(f.st_mode))
                                printf("\033[0m");
                        }
                        else if (lf && af)
                        {
                            printinfo(d->d_name, d->d_name);
                        }
                    }
                    count++;
                }
                else if (!strcmp(copy[i], ".."))
                {
                    if (pc > 1)
                        printf("\n%s:\n", copy[i]);
                    if (lf && flag_arr[i])
                    {
                        printf("total %d\n", tc[i]);
                    }
                    strcpy(cpy, cur_dir);
                    chdir("..");
                    getcwd(cur_dir, maxl);
                    int size = scandir(cur_dir, &list, 0, alphasort);
                    for (int j = 0; j < size; j++)
                    {
                        d = list[j];
                        if (!lf && !af)
                        {
                            if (d->d_name[0] != '.')
                            {
                                stat(d->d_name, &f);
                                if (S_ISDIR(f.st_mode))
                                    printf("\e[1;34m");
                                else if ((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH))
                                    printf("\e[1;32m");

                                printf("%s\n", d->d_name);

                                if (((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH)) || S_ISDIR(f.st_mode))
                                    printf("\033[0m");
                            }
                        }
                        else if (lf && !af)
                        {
                            if (d->d_name[0] != '.')
                                printinfo(d->d_name, d->d_name);
                        }
                        else if (!lf && af)
                        {
                            stat(d->d_name, &f);
                            if (S_ISDIR(f.st_mode))
                                printf("\e[1;34m");
                            else if ((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH))
                                printf("\e[1;32m");

                            printf("%s\n", d->d_name);

                            if (((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH)) || S_ISDIR(f.st_mode))
                                printf("\033[0m");
                        }
                        else if (lf && af)
                        {
                            printinfo(d->d_name, d->d_name);
                        }
                    }
                    chdir(cpy);
                    getcwd(cur_dir, maxl);
                    count++;
                }
                else
                {
                    if (pc > 1)
                        printf("\n%s:\n", copy[i]);
                    if (lf && flag_arr[i])
                    {
                        printf("total %d\n", tc[i]);
                    }
                    stat(copy[i], &f);
                    if (S_ISDIR(f.st_mode))
                    {
                        int size = scandir(copy[i], &list, 0, alphasort);
                        z = opendir(copy[i]);
                        if (z)
                        {
                            for (int j = 0; j < size; j++)
                            {
                                d = list[j];
                                if (!lf && !af)
                                {
                                    if (d->d_name[0] != '.')
                                    {
                                        stat(d->d_name, &f);
                                        if (S_ISDIR(f.st_mode))
                                            printf("\e[1;34m");
                                        else if ((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH))
                                            printf("\e[1;32m");

                                        printf("%s\n", d->d_name);

                                        if (((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH)) || S_ISDIR(f.st_mode))
                                            printf("\033[0m");
                                    }
                                }
                                else if (lf && !af)
                                {
                                    if (d->d_name[0] != '.')
                                    {
                                        char curr[maxl], cat[maxl];
                                        getcwd(curr, maxl);
                                        strcpy(cat, curr);
                                        strcat(cat, "/");
                                        strcat(cat, copy[i]);
                                        strcat(cat, "/");
                                        strcat(cat, d->d_name);
                                        printinfo(cat, d->d_name);
                                    }
                                }
                                else if (!lf && af)
                                {
                                    stat(d->d_name, &f);
                                    if (S_ISDIR(f.st_mode))
                                        printf("\e[1;34m");
                                    else if ((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH))
                                        printf("\e[1;32m");

                                    printf("%s\n", d->d_name);

                                    if (((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH)) || S_ISDIR(f.st_mode))
                                        printf("\033[0m");
                                }
                                else if (lf && af)
                                {
                                    char curr[maxl], cat[maxl];
                                    getcwd(curr, maxl);
                                    strcpy(cat, curr);
                                    strcat(cat, "/");
                                    strcat(cat, copy[i]);
                                    strcat(cat, "/");
                                    strcat(cat, d->d_name);
                                    printinfo(cat, d->d_name);
                                }
                            }
                        }
                        count++;
                    }
                    else
                    {
                        if (!access(copy[i], F_OK))
                        {
                            if (!lf && !af)
                            {
                                if (strcmp(copy[i], ".") && strcmp(copy[i], ".."))
                                    printf("%s\n", copy[i]);
                            }
                            else if (lf && !af)
                            {
                                if (strcmp(copy[i], ".") && strcmp(copy[i], ".."))
                                    printinfo(copy[i], copy[i]);
                            }
                            else if (!lf && af)
                            {
                                printf("%s\n", copy[i]);
                            }
                            else if (lf && af)
                            {
                                printinfo(copy[i], copy[i]);
                            }
                            count++;
                        }
                        else if (copy[i] != NULL)
                        {
                            printf("ls: cannot access '%s': No such file or directory\n", copy[i]);
                            count++;
                        }
                    }
                }
            }
            i++;
        }
        if (!count)
        {
            if (pc > 1)
                printf("\n%s:\n", copy[i]);
            if (lf && flag1)
            {
                printf("total %d\n", total_f);
            }
            int size = scandir(cur_dir, &list, 0, alphasort);
            for (int j = 0; j < size; j++)
            {
                d = list[j];
                if (!lf && !af)
                {
                    if (d->d_name[0] != '.')
                    {
                        stat(d->d_name, &f);
                        if (S_ISDIR(f.st_mode))
                            printf("\e[1;34m");
                        else if ((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH))
                            printf("\e[1;32m");

                        printf("%s\n", d->d_name);

                        if (((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH)) || S_ISDIR(f.st_mode))
                            printf("\033[0m");
                    }
                    // printf("Given unknown flag or just space\n");
                }
                else if (lf && !af)
                {
                    if (d->d_name[0] != '.')
                        printinfo(d->d_name, d->d_name);
                }
                else if (!lf && af)
                {
                    stat(d->d_name, &f);
                    if (S_ISDIR(f.st_mode))
                        printf("\e[1;34m");
                    else if ((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH))
                        printf("\e[1;32m");

                    printf("%s\n", d->d_name);

                    if (((f.st_mode & S_IXUSR) && (f.st_mode & S_IXGRP) && (f.st_mode & S_IXOTH)) || S_ISDIR(f.st_mode))
                        printf("\033[0m");
                }
                else if (lf && af)
                {
                    printinfo(d->d_name, d->d_name);
                }
            }
        }
    }
}