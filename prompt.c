#include "headers.h"

int prompt(char *dir)
{
    char user[maxl], path[maxl], new_path[maxl], system[maxl];

    getlogin_r(user, maxl);
    gethostname(system, maxl);
    getcwd(path, maxl);
    printf("\e[1;32m");
    printf("<%s@%s:~>", user, system);
    printf("\033[0m");

    int dir_len = strlen(dir);
    int path_len = strlen(path);

    if (dir_len <= path_len)
    {
        strncpy(new_path, path, dir_len);
        new_path[dir_len] = '\0';
        if (!strcmp(new_path, dir) && dir_len == path_len)
            printf(" ");
        else if (!strcmp(new_path, dir))
        {
            printf("\e[1;34m");
            if (path[dir_len] == '/')
            {
                int i;
                for (i = dir_len; i < path_len; i++)
                    printf("%c", path[i]);
            }
            printf("\033[0m");
            printf("$");
        }
    }
    else
    {
        printf("\e[1;34m");
        printf("%s$", path);
        printf("\033[0m");
    }

    return 0;
}