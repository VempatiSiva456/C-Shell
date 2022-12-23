#include "headers.h"
void bgHandler()
{
    int status;
    int pid = waitpid(-1, &status, WNOHANG);

    if (pid > 0)
    {
        int i = 0;
        while (i < nbgps)
        {
            if (bgpid[i] == pid)
            {
                break;
            }
            i++;
        }
        if (WIFSTOPPED(status))
            return;
        printf("\n%s with PID %d exited %s\n", bgp[i], pid,
               WIFEXITED(status) ? "normally" : "abnormally");
        bgpstopped[i] = 1;
        char cwd[maxl];
        getcwd(cwd, maxl);
        prompt(cwd);
    }
}

int main()
{
    shell_pid = getpid();
    nbgps = 0;
    nfgps = 0;
    clear;
    signal(SIGTSTP, sigintHandlerZ);
    signal(SIGINT, sigintHandlerC);
    signal(SIGCHLD, bgHandler);
    fgrunning = 0;

    while (1)
    {
        shell();
        // signal(SIGINT, sigintHandlerC);
        // signal(SIGTSTP, sigintHandlerZ);
        // signal(SIGCHLD, bgHandler);
        char cwd[maxl];
        getcwd(cwd, maxl);
        prompt(cwd);
    }
}