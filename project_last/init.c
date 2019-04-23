//#include "utype.h"
#include "ucode.c"

int console, console1, console2;
int parent()  // P1's code
{
    int pid, status;
    while(1)
    {
        prints("INIT : wait for ZOMBIE child\n");
        pid = wait(&status);
        if (pid == console)  // if console login process died
        {
            prints("INIT Nofal parent: forks a new console login\n");
            console = fork();  // fork another one

            if (console)
                continue;
            else
                exec("login /dev/tty0");  // new console login process
        }
    }
    printf("INIT: I just buried an orphan child proc %d\n", pid);
}

int main()
{
    //ubody("one");
    int in1, out1, in2, out2, in3, out3;   // file descriptors for terminal I/O
    in1 = open("/dev/tty0", O_RDONLY);   // file descriptor 0
    out1 = open("/dev/tty0", O_WRONLY);   // for disply to console

    in2 = open("/dev/ttyS0", O_RDONLY);   // file descriptor 0
    out2 = open("/dev/ttyS0", O_WRONLY);   // for disply to console

    in3 = open("/dev/ttyS1", O_RDONLY);   // file descriptor 0
    out3 = open("/dev/ttyS1", O_WRONLY);   // for disply to console

    prints("INIT Nofal: fork a login proc on console\n");
    console = fork();
    //console1 = fork();

    if (console)  // parent
    {
        console1 = fork();
        if (console1)  // parent
        {
            console2 = fork();
            if (console2)  // parent
                parent();
            else        // child: exec to login on ttyS1
                exec("login /dev/ttyS1");
        }
        else        // child: exec to login on ttyS0
            exec("login /dev/ttyS0");
    }
    else        // child: exec to login on tty0
        exec("login /dev/tty0");
}
