#include "headers.h"

void ioredirecting(char *str, int pflag, int ifd, int ofd)
{
    pid_t PID;

    char cpy1[maxl], cpy2[maxl], cpy3[maxl];
    strcpy(cpy1, str);
    strcpy(cpy2, cpy1);
    strcpy(cpy3, cpy2);

    int i, j, len, ip = -1, op = -1, ap = 0;
    len = strlen(str);

    for (i = 0; i < len; i++)
    {
        if (str[i] == '<')
        {
            ip = i;
            break;
        }
    }
    for (i = 0; i < len; i++)
    {
        if (str[i] == '>')
        {
            op = i;
            break;
        }
    }

    if (op != len - 1)
    {
        if (str[op + 1] == '>')
        {
            ap = 1;
        }
    }

    char **string1 = (char **)malloc(maxl * sizeof(char *));
    char **string2 = (char **)malloc(maxl * sizeof(char *));

    string1[0] = strtok(cpy1, ">");
    string2[0] = strtok(string1[0], "<");

    char *token[maxl];

    i = 0;
    token[i] = strtok(string2[i], " \t");
    while (token[i] != NULL)
    {
        i++;
        token[i] = strtok(NULL, " \t");
    }

    char *ip_file = (char *)malloc(maxl * sizeof(char));
    char *op_file = (char *)malloc(maxl * sizeof(char));

    if (ip != -1)
    {
        char *temp[maxl];
        i = 0;
        temp[i] = strtok(cpy2, "<");
        while (temp[i] != NULL)
        {
            i++;
            temp[i] = strtok(NULL, "<");
        }
        temp[i] = 0;
        ip_file = temp[i - 1];
        char tcpy[maxl];

        ip_file = strtok(ip_file, ">");
        strcpy(tcpy, ip_file);
        ip_file = strtok(tcpy, " \t");
    }

    if (op != -1)
    {
        char *temp[maxl];
        i = 0;
        temp[i] = strtok(cpy3, ">");
        while (temp[i] != NULL)
        {
            i++;
            temp[i] = strtok(NULL, ">");
        }
        temp[i] = 0;
        op_file = temp[i - 1];
        char tcpy[maxl];

        op_file = strtok(op_file, "<");
        strcpy(tcpy, op_file);
        op_file = strtok(tcpy, " \t");
    }

    PID = fork();

    if (!PID)
    {
        if (ip != -1)
        {
            int ifd = open(ip_file, O_RDONLY);

            if (ifd == -1)
            {
                perror("Error opening the input file\n");
                return;
            }
            else if (dup2(ifd, STDIN_FILENO) == -1)
            {
                perror("dup2\n");
                return;
            }
        }
        int ofd;
        if (op != -1)
        {
            if (ap == 1)
                ofd = open(op_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            else if (!ap)
                ofd = open(op_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (ofd == -1)
            {
                perror("Error opening the output file\n");
                return;
            }
            if (dup2(ofd, STDOUT_FILENO) == -1)
            {
                perror("dup2\n");
                return;
            }
        }

        if (execvp(token[0], token) == -1)
        {
            perror(token[0]);
            return;
        }
    }
    else
    {
        if (pflag)
            exit(0);
        else
            wait(NULL);
    }
}

void piperedirecting(char *str, int pflag, int ifd, int ofd)
{
    pflag = 1;
    int count = 0;
    char **ptoken = (char **)malloc(maxl * sizeof(char *));

    ptoken[count] = strtok(str, "|");
    while (ptoken[count] != NULL)
    {
        count++;
        ptoken[count] = strtok(NULL, "|");
    }
    ptoken[count] = 0;

    int np = count - 1;

    int pfd1[2], pfd2[2];

    int i = 0, tidx = 0;
    while (i < count)
    {
        if (i % 2)
        {
            if (pipe(pfd1) == -1)
            {
                printf("Pipe error 1\n");
                return;
            }
        }
        else
        {
            if (pipe(pfd2) == -1)
            {
                printf("Pipe error 2\n");
                return;
            }
        }

        char temp[maxl];
        char **string = (char **)malloc(maxl * sizeof(char *));

        strcpy(temp, ptoken[tidx]);

        int j = 0;
        string[j] = strtok(ptoken[tidx], " \t");
        tidx++;
        while (string[j] != NULL)
        {
            j++;
            string[j] = strtok(NULL, " \t");
        }
        string[j] = 0;

        int flag = 0;
        pid_t PID;

        PID = fork();

        if (PID == -1)
        {
            perror("fork\n");
        }
        else if (!PID)
        {
            if (i % 2)
            {
                dup2(pfd2[0], STDIN_FILENO);
                if (i != np)
                    dup2(pfd1[1], STDOUT_FILENO);
            }
            else
            {
                if (i != 0)
                    dup2(pfd1[0], STDIN_FILENO);
                if (i != np)
                    dup2(pfd2[1], STDOUT_FILENO);
            }
            int k, len = strlen(temp);
            for (k = 0; k < len; k++)
            {
                if (temp[k] == '>')
                {
                    flag = 1;
                    break;
                }
                else if (temp[k] == '<')
                {
                    flag = 1;
                    break;
                }
            }
            if (flag)
                ioredirecting(temp, pflag, ifd, ofd);
            else
            {
                if (execvp(string[0], string) == -1)
                {
                    printf("Execution Error\n");
                }
                return;
            }
        }
        else
        {
            if (i % 2)
            {
                close(pfd2[0]);
                if (i != np)
                    close(pfd1[1]);
            }
            else
            {
                if (i != 0)
                    close(pfd1[0]);
                if (i != np)
                    close(pfd2[1]);
            }
            i++;
            wait(NULL);
        }
    }
}