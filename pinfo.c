#include "headers.h"

int pinfo(char *str, char *dir)
{
    int i = 0;
    int dir_len = strlen(dir);

    pid_t PID;

    char path[maxl], pid[maxl], temp[maxl];

    char *token;
    token = strtok(NULL, " \t");

    if (token == NULL)
    {
        PID = getpid();
        sprintf(pid, "%d", PID);
    }
    else
    {
        PID = atoi(token);
        strcpy(pid, token);
        token = strtok(NULL, " \t");
        if (token != NULL)
        {
            printf("Too many arguments for pinfo\n");
            return 0;
        }
    }

    strcpy(path, "/proc/");
    strcat(path, pid);
    strcat(path, "/stat");

    FILE *f = NULL;
    f = fopen(path, "r");
    if (f == NULL)
    {
        perror("Error opening the file\n");
        return 0;
    }

    int idx = 0;
    char group_id[maxl], ppid[maxl], store[maxl];

    fgets(store, maxl, f);

    // printf("%s\n", store);

    char *token_t;
    token_t = strtok(store, " \t");

    char statusp[maxl], memoryp[maxl];
    int flag = 0;

    while (token_t != NULL)
    {
        if (idx == 2)
        {
            strcpy(statusp, token_t);
        }
        if (idx == 4)
        {
            strcpy(group_id, token_t);
        }
        if (idx == 7)
        {
            strcpy(ppid, token_t);
        }
        if (idx == 22)
        {
            if (!strcmp(group_id, ppid))
            {
                flag = 1;
            }
            strcpy(memoryp, token_t);
        }
        token_t = strtok(NULL, " \t");
        idx++;
    }

    printf("pid : %d\n", PID);
    if (flag)
        printf("process Status : {%s+}\n", statusp);
    else
        printf("process Status : {%s}\n", statusp);
    printf("memory : %s ", memoryp);
    printf("{Virtual Memory}\n");

    strcpy(path, "/proc/");
    strcat(path, pid);
    strcat(path, "/exe");

    memset(store, 0, maxl);

    int rl = readlink(path, store, maxl);
    if (rl == -1)
    {
        printf("Error while accessing /proc/%d/exe\n", PID);
        return 0;
    }

    store[rl] = '\0';
    int st_len = strlen(store);

    if (dir_len <= st_len)
    {
        printf("Executable Path : ");
        strncpy(temp, store, dir_len);
        temp[dir_len] = '\0';
        if (dir_len == st_len && !strcmp(temp, dir))
        {
            printf("~");
        }
        else if (!strcmp(temp, dir) && store[dir_len] == '/')
        {
            printf("~");
            for (i = dir_len; i < st_len; i++)
                printf("%c", store[i]);
        }
        else
        {
            printf("%s", store);
        }
        printf("\n");
    }
    else
    {
        printf("Executable path : %s\n", store);
    }
    return 0;
}