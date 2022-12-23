#include "headers.h"

void printjobs()
{
    char *token;
    int rflag = 0, sflag = 0;
    token = strtok(NULL, " \t");
    if (token != NULL)
    {
        char flag[maxl];
        strcpy(flag, token);
        if (!strcmp(flag, "-r"))
        {
            rflag = 1;
        }
        else if (!strcmp(flag, "-s"))
        {
            sflag = 1;
        }
        else
        {
            printf("wrong flag\n");
            return;
        }
    }
    for (int i = 0; i < nbgps; i++)
    {
        if (rflag)
        {
            if (!bgpstopped[i])
            {
                printf("[%d] %s %s [%d]\n", i + 1, bgpstopped[i] ? "Stopped" : "Running", bgp[i], bgpid[i]);
            }
        }
        else if (sflag)
        {
            if (bgpstopped[i])
            {
                printf("[%d] %s %s [%d]\n", i + 1, bgpstopped[i] ? "Stopped" : "Running", bgp[i], bgpid[i]);
            }
        }
        else
        {
            printf("[%d] %s %s [%d]\n", i + 1, bgpstopped[i] ? "Stopped" : "Running", bgp[i], bgpid[i]);
        }
    }

    // if (nfgps > 0)
    // {
    //     printf("\n \n \nForeground Processes after fg:\n");
    // }
    // for (int i = 0; i < nfgps; i++)
    // {
    //     printf("[%d] %s %s [%d]\n", i + 1, fgpstopped[i] ? "Stopped" : "Running", fgp[i], fgpid[i]);
    // }
}

void fg(char *str)
{
    pid_t shellpid = getpid();
    // printf("%d\n", shellpid);
    char *token[maxl];
    int i = 0;
    token[i] = strtok(NULL, " \t");
    // printf("%s\n", token[0]);
    while (token[i] != NULL)
    {
        i++;
        token[i] = strtok(NULL, "\t");
    }
    int no1 = atoi(token[0]);
    if (no1 > nbgps)
    {
        printf("No such job number exists\n");
        return;
    }
    if (bgpstopped[no1 - 1] == 1)
    {
        printf("Process Stopped Already\n");
    }
    else
    {
        printf("brings [%d] %s to the foreground\n", no1, bgp[no1 - 1]);
        pid_t pidnumber;
        pidnumber = stoppedpids[no1 - 1];
        processpid = pidnumber;
        // printf("%d\n", pidnumber);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        tcsetpgrp(0, getpgid(pidnumber));
        kill(pidnumber, SIGCONT);

        waitpid(pidnumber, NULL, WUNTRACED);

        tcsetpgrp(0, shellpid);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);

        fgrunning = 1;
        free(fgp[nfgps]);
        fgp[nfgps] = malloc(maxl * sizeof(char));
        strcpy(fgp[nfgps], bgp[no1 - 1]);
        bgpstopped[no1 - 1] = 1;
        fgpid[nfgps] = bgpid[no1 - 1];
        fgpstopped[nfgps] = 0;
        nfgps++;
    }
}

void bg(char *str)
{
    char *token[maxl];
    int i = 0;
    token[i] = strtok(NULL, " \t");
    // printf("%s\n", token[0]);
    while (token[i] != NULL)
    {
        i++;
        token[i] = strtok(NULL, "\t");
    }

    int no1 = atoi(token[0]);

    if (no1 > nbgps)
    {
        printf("No such job number exists\n");
        return;
    }

    if (bgpstopped[no1 - 1] == 0)
    {
        printf("[%d] %s is already in running state\n", no1, bgp[no1 - 1]);
    }
    {
        kill(stoppedpids[no1 - 1], SIGTTIN);
        kill(stoppedpids[no1 - 1], SIGCONT);
        printf("changes the state of [%d] %s to running\n", no1, bgp[no1 - 1]);
        bgpstopped[no1 - 1] = 0;
    }
}

void sig(char *str)
{
    char *token[maxl];
    int i = 0;
    token[i] = strtok(NULL, " \t");
    // printf("%s\n", token[0]);
    while (token[i] != NULL)
    {
        i++;
        token[i] = strtok(NULL, "\t");
    }
    // printf("%s\n", token[1]);
    if (i != 2)
    {
        printf("Incorrect arguments\n");
        return;
    }
    int no1 = atoi(token[0]);
    int no2 = atoi(token[1]);
    if (kill(stoppedpids[no1 - 1], no2) < 0)
    {
        perror("Error");
        return;
    }
}