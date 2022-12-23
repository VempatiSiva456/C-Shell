#include "headers.h"

int scan2(char *string)
{
    long length = 0;
    // int c = getline(string, &length, stdin);
    if (!strcmp(string, "exit\n") || strlen(string) < 0)
    {
        if (!strcmp(string, "exit\n"))
            return 1;
        else
            perror("Error while reading string\n");
        return 1;
    }
    // else
    //     (*string)[c - 1] = '\0';
    return 0;
}

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    {
        die("tcsetattr");
    }
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be
 * written to the terminal, and also discards any input that hasn’t been read. The c_lflag field is for “local flags”
 */
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
        die("tcsetattr");
    }
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
 */
int scan(char *string, char *root)
{
    char *inp = malloc(sizeof(char) * 100);
    char c;
    while (1)
    {
        setbuf(stdout, NULL);
        enableRawMode();
        memset(inp, '\0', 100);
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (iscntrl(c))
            {
                if (c == 10)
                    break;
                else if (c == 27)
                {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2)
                    { // length of escape code
                      // printf("\rarrow key: %s", buf);
                    }
                }
                else if (c == 127)
                { // backspace
                    if (pt > 0)
                    {
                        if (inp[pt - 1] == 9)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                }
                else if (c == 9)
                {
                    // TAB character recognized
                    int tv = 0;
                    char str[maxl];
                    memset(str, 0, sizeof(str));
                    strcpy(str, inp);
                    int len = strlen(str);
                    char file[maxl];
                    memset(file, 0, sizeof(file));
                    for (int j = len - 1; j >= 0; j--)
                    {
                        if (str[j] == ' ')
                        {
                            tv = j + 1;
                            break;
                        }
                    }
                    int nsize = len - tv;
                    if (nsize)
                    {
                        for (int j = 0; j < nsize; j++)
                        {
                            file[j] = str[j + tv];
                        }
                        char cur_dir[maxl];
                        getcwd(cur_dir, maxl);
                        DIR *z = opendir(cur_dir);
                        struct dirent *d = {0};
                        struct dirent **list = {0};
                        struct stat f = {0};
                        int size = scandir(cur_dir, &list, 0, alphasort);
                        int nfiles = 0;
                        char tempstr[maxl];
                        for (int j = 0; j < size; j++)
                        {
                            d = list[j];
                            if (!strncmp(d->d_name, file, nsize))
                            {
                                nfiles++;
                                strcpy(tempstr, d->d_name);
                            }
                        }
                        int it = 0;
                        if (nfiles == 1)
                        {
                            stat(tempstr, &f);
                            int dflag = 0;
                            if (S_ISDIR(f.st_mode))
                                dflag = 1;
                            int leng = strlen(tempstr);
                            for (int j = nsize; j < leng; j++)
                            {
                                inp[pt++] = tempstr[j];
                                printf("%c", tempstr[j]);
                            }
                            if (dflag)
                            {
                                inp[pt++] = '/';
                                printf("/");
                            }
                            else
                            {
                                inp[pt++] = ' ';
                                printf(" ");
                            }
                        }
                        else if (nfiles > 1)
                        {
                            printf("\n");
                            for (int j = 0; j < size; j++)
                            {
                                d = list[j];
                                stat(d->d_name, &f);
                                if (!strncmp(d->d_name, file, nsize))
                                {
                                    if (S_ISDIR(f.st_mode))
                                        printf("%s/\n", d->d_name);
                                    else
                                        printf("%s \n", d->d_name);
                                }
                            }
                            shell();
                        }
                    }
                }
                else if (c == 4)
                {
                    exit(0);
                }
                else
                {
                    // printf("%d\n", c);
                }
            }
            else
            {
                inp[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();
        strcpy(string, inp);
        printf("\n");
        return scan2(inp);
    }
    return 0;
}