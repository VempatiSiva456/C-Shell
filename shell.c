#include "headers.h"

void EXECVP(char *input)
{
    time_t start, end;
    int dif;
    time(&start);
    char *delimt = " \t";
    char **store = (char **)malloc(maxl * sizeof(char *));
    int j = 0;
    store[j] = input;
    j++;
    // fgrunning = 1;
    int f = 0;
    while (store[j - 1] != NULL)
    {
        store[j] = strtok(NULL, delimt);
        j++;
    }
    f = strcmp(store[j - 2], "&") == 0;
    int retu = fork();
    if (retu < 0)
    {
        perror("error");
        return;
    }
    else if (retu > 0)
    {

        if (f == 1)
        {

            int status;
            printf("[%d]\n", retu);

            bgpid[nbgps] = retu;
            stoppedpids[nbgps] = retu;
            free(bgp[nbgps]);
            bgp[nbgps] = malloc(maxl * sizeof(char));
            strcpy(bgp[nbgps], store[0]);
            bgpstopped[nbgps] = 0;
            nbgps++;

            char cwd[maxl];
            getcwd(cwd, maxl);
            prompt(cwd);
        }
        else
        {
            int status;

            fgrunning = 1;
            free(fgp[nfgps]);
            fgpid[nfgps] = retu;
            fgp[nfgps] = malloc(maxl * sizeof(char));
            strcpy(fgp[nfgps], store[0]);
            fgpstopped[nfgps] = 0;
            nfgps++;

            processpid = retu;

            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);

            tcsetpgrp(0, retu);

            waitpid(retu, &status, WUNTRACED);

            tcsetpgrp(0, getpgid(0));

            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
        }
    }
    else
    {
        if (f == 1)
        {
            store[j - 2] = NULL;
        }
        // printf("execvp\n");
        setpgid(0, 0);
        // signal(SIGTSTP, sigintHandlerZ);
        int x = execvp(store[0], store);
        if (x == -1)
            printf("\nInvalid Command\n");
        exit(1);
        // return;
    }
    fgrunning = 0;
    time(&end);
    dif = difftime(end, start);
    if (dif >= 1)
        printf("(took %ds) ", dif);

    return;
}

int shell()
{
    char new[maxl], prev[maxl];
    char **str = (char **)malloc(maxl * sizeof(char *));
    char *ip = (char *)malloc(maxl * sizeof(char)), *path, *string;
    int pflag = 0;
    char root[maxl];
    getcwd(root, maxl);

    getcwd(new, maxl);
    strcpy(prev, new);

    int ifd, ofd;

    int i, j, k, flag = 0;
    flag = prompt(new);

    while (!flag)
    {
        i = 0;
        flag = scan(ip, root);
        if (ip[0] != ' ')
            add_history(ip, root);
        char ipcpy[maxl];
        strcpy(ipcpy, ip);
        if (flag)
            break;
        int tflag1 = 0, tflag2 = 0;
        int it, len = strlen(ipcpy);
        int background = 0;
        for (it = 0; it < len; it++)
        {
            if (ipcpy[it] == '&')
            {
                background = 1;
                break;
            }
        }

        for (it = 0; it < len; it++)
        {
            if (ipcpy[it] == '|')
            {
                tflag1 = 1;
                break;
            }
        }
        if (!tflag1)
            for (it = 0; it < len; it++)
            {
                if (ipcpy[it] == '>' || ipcpy[it] == '<')
                {
                    tflag2 = 1;
                    break;
                }
            }
        if (tflag1)
        {
            piperedirecting(ipcpy, pflag, ifd, ofd);
            close(ifd);
            close(ofd);
            // freopen("/dev/tty", "w", stdout);
        }
        else if (tflag2)
        {
            ioredirecting(ipcpy, pflag, ifd, ofd);
        }
        else
        {
            str[i] = strtok(ip, ";");
            while (str[i] != NULL)
            {
                i++;
                str[i] = strtok(NULL, ";");
            }
            j = 0;
            while (j < i)
            {
                string = strtok(str[j], " \t");

                k = 0;
                while (string != NULL)
                {
                    if (!k)
                    {
                        if (!strcmp(string, "exit"))
                        {
                            flag = 1;
                            break;
                        }
                        else if (!strcmp(string, "echo") && !background)
                        {
                            echo(str[j]);
                        }
                        else if (!strcmp(string, "cd") && !background)
                        {
                            cd(prev, new);
                        }
                        else if (!strcmp(string, "pwd") && !background)
                        {
                            get_pwd();
                        }
                        else if (!strcmp(string, "ls") && !background)
                        {
                            ls(str[j], root);
                        }
                        else if (!strcmp(string, "history") && !background)
                        {
                            show_history(ipcpy, root);
                        }
                        else if (!strcmp(string, "pinfo") && !background)
                        {
                            pinfo(ipcpy, root);
                        }
                        else if (!strcmp(string, "discover") && !background)
                        {
                            discover(str[j], root);
                        }
                        else if (!strcmp(string, "jobs") && !background)
                        {
                            printjobs();
                        }
                        else if (!strcmp(string, "fg") && !background)
                        {
                            fg(str[j]);
                        }
                        else if (!strcmp(string, "bg") && !background)
                        {
                            bg(str[j]);
                        }
                        else if (!strcmp(string, "sig") && !background)
                        {
                            sig(str[j]);
                        }
                        else
                        {
                            EXECVP(str[j]);
                        }
                    }
                    string = strtok(NULL, " \t");
                    k++;
                }
                j++;
            }
        }
        if (!flag && !background)
            flag = prompt(new);
    }
    return 0;
}