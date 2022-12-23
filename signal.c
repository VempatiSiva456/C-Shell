#include "headers.h"

void sigintHandlerC(int signum)
{
    if (getpid() != shell_pid)
        return;

    if (childpid != 0)
    {
        fprintf(stdout, "\nSending SIGINT to %d.\n", childpid);
        kill(childpid, SIGINT);
    }
    signal(SIGINT, sigintHandlerC);
}

void sigintHandlerZ(int sig_num)
{
    // printf("hii\n");
    if (getpid() != shell_pid)
        return;
    // printf("%d,%d\n", shell_pid, processpid);

    if (processpid != shell_pid && processpid != 0)
    {
        int i = 0;
        while (fgpid[i] != processpid && i < 100)
            i++;
        // printf("%d", i);
        // char *name[maxl];
        // free(name[i]);
        // name[i] = malloc(maxl * sizeof(char));
        // strcpy(name[i], fgp[i]);
        // printf("%s\n", name);
        fgpstopped[i] = 1;
        bgpid[nbgps] = processpid;
        stoppedpids[nbgps] = processpid;
        free(bgp[nbgps]);
        bgp[nbgps] = malloc(maxl * sizeof(char));
        strcpy(bgp[nbgps], fgp[i]);
        bgpstopped[nbgps] = 1;
        nbgps++;
        kill(processpid, SIGTTIN);
        kill(processpid, SIGTSTP);
    }
    signal(SIGTSTP, sigintHandlerZ);
}