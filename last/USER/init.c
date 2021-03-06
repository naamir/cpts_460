#include "utype.h"
#include "ucode.c"
//#include "open_close_lseek.c"

int console;
int parent()  // P1's code
{
    int pid, status;
    while(1)
    {
        printf("INIT : wait for ZOMBIE child\n");
        pid = uwait(&status);
        if (pid == console)  // if console login process died
        {
            printf("INIT: forks a new console login\n");
            console = ufork();  // fork another one
            if (console)
                continue;
            else
                uexec("login /dev/tty0");  // new console login process
        }
    }
    printf ("INIT: I just buried an orphan chald proc %d\n", pid);
}

int main()
{
    //ubody("one");
    int in, out;   // file descriptors for terminal I/O
    //in = open("/dev/tty0", O_RDONLY);   // file descriptor 0
    //out = open("/dev/tty0", O_WRONLY);   // for disply to console
    printf("INIT: fork a login proc on console\n");
    console = ufork();
    if (console)  // parent
        parent();
    else        // child: exec to login on tty0
        uexec("login /dev/tty0");
}
