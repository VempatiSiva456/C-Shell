#ifndef HEADERS_H
#define HEADERS_H

#include "cd.h"
#include "discover.h"
#include "echo.h"
#include "history.h"
#include "ioredirection.h"
#include "ls.h"
#include "pinfo.h"
#include "prompt.h"
#include "pwd.h"
#include "scan.h"
#include "shell.h"
#include "signal.h"
#include "jobs.h"
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <libgen.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define clear printf("\033[H\033[J")

#define maxl 1000
#define flimit 21

int background_flag;
pid_t processpid;
int bgpid[maxl];
int stoppedpids[maxl];
int nbgps;
char *bgp[maxl];
int bgpstopped[maxl];

int shell_pid;
int childpid;
char *fgp[maxl];
int nfgps;
int fgrunning;
int fgpstopped[maxl];
int fgpid[maxl];

#endif