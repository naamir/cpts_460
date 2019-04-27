#include "ucode.c"

int tknum, saved_stdin, saved_stdout, intty, outtty;
int numpipes, fd;//, pipeflag;
char *tok[32], tty[32];

static char cmd1[64];
char cmd2[64];
static char splittemp1[128], splittemp2[128];

void tokenize_line(char *line)
{
    char *cp;
    cp = line;
    tknum = 0;
    
    while (*cp != 0){
        while (*cp == ' ') *cp++ = 0;        
        if (*cp != 0)
            tok[tknum++] = cp;         
        while (*cp != ' ' && *cp != 0) cp++;                  
        if (*cp != 0)   
            *cp = 0;                   
        else 
                break; 
        cp++;
    }
    tok[tknum] = 0;
}

int split_up(char *cmdline, char *cmd1, char *cmd2)
{
    int i = 0, n = 0;
    char *cp; //, *c1, *c2;

    cp = cmdline; //splittemp1;

    while (*cp != '|' && *cp != '\r')
    {
        //mputcCustom(outtty, *cp);
        cmd1[i++] = *cp;
        cp++;
    }
    cmd1[i] = 0;

    if (*cp == '\r')  // reached the end of cmds
        return 0;

    cp++; // miss the pipe...
    cp++; // ...and space

    while(*cp != '\r')
    {
        cmd2[n++] = *cp;
        cp++;
    }
    cmd2[n] = 0;

    numpipes--;
}

int mypipe(char *cmd) //char *cmd1, char *cmd2)
{
    split_up(cmd, cmd1, cmd2);
    printfCustom(outtty, "split_up -> cmd1:%s cmd2:%s\n", cmd1, cmd2);
    
    int pd[2], pid, r = 0;

    pipe(pd);   // creates a PIPE; pd[0] for READ, pd[1] for WRITE
    pid = fork();  // fork a child to share a pipe

    if (pid)  // parent: as pipe READER
    {
        close(pd[1]);    // close pipe WRITE end
        dup2(pd[0], 0);  // redirect stdin to pipe READ end
        if (numpipes) mypipe(cmd2);
        else exec(cmd2);
    }
    else      // child: as pipe WRITER
    {
        close(pd[0]);    // close pipe READ end
        dup2(pd[1], 1);  // redirect stout to pipe WRITE end

        //tokenize_line(cmd1);

        exec(cmd1);
    }
}

void parseArgs()
{
    int n;
    for (n = 1; n < tknum; n++)
    {
        if (strcmp(tok[n], "<") == 0)
        {
            printsCustom(outtty, "Redirect Input < infile\n");  // take inputs from infile
            close(STDIN);
            printfCustom(outtty, "tok[n+1]=%s tok[n]=%s\n", tok[n+1], tok[n]); 
            fd = open(tok[n+1], O_RDONLY);  // open filename for READ, which
                                                // will replace fd 0
            
            //dup2(fd, STDIN);    // system call to close file descriptor IN (0)
        }
        else if (strcmp(tok[n], ">") == 0)
        {
            printsCustom(outtty, "Redirect Output > outfile\n");  // send outputs to outfile
            fd = open(tok[n+1], O_WRONLY|O_CREAT); // open filename for WRITE, which
                                                            // will replace fd 0
            //saved_stdout = dup(OUT);
            dup2(fd, STDOUT);    // system call to close file descriptor OUT (1)
            //close(fd);
        }
        else if (strcmp(tok[n], ">>") == 0)
        {
            printsCustom(outtty, "Redirect Output >> outfile APPEND\n");  // APPEND outputs to outfile
            fd = open(tok[n+1], O_WRONLY|O_CREAT|O_APPEND); // open filename for WRITE, which
                                                            // will replace fd 0
            //saved_stdout = dup(OUT);
            dup2(fd, STDOUT);    // system call to close file descriptor OUT (1)
            //close(fd);
        }
        else if (strcmp(tok[n], "|") == 0)
        {
            //printsCustom(outtty, "has | let the pipin beginnnn\n");
            numpipes++;
            printfCustom(outtty, "numpipes:%d\n", numpipes);
        }
    }
}

main(int argc, char *argv[])
{
    int r, pid, status;
    char cmdline[128], noIOcmd[128];

    // get the current tty
    gettty(tty);
    
    //open the current tty for read and write
    intty = STDIN; //open(tty, O_RDONLY);
    outtty = STDOUT; //open(tty, O_WRONLY);

    while (1)
    {
        memset(cmdline, 0, 128);
        numpipes = 0;
        printsCustom(outtty, "sh_n: #"); getsCustom(intty, outtty, cmdline);
        //prints("cmd: "); prints(cmdline); prints("\n");
        if (strcmp(cmdline, "\0") == 0 || cmdline[0] == ' ') 
            continue;
        // copy cmdline into nIOcmd so that can be passed to non-IO commands, like "cat filename"
        strcpy(noIOcmd, cmdline);
        //tokenize dat shit! in tok[32] and num of tokens in tknum
        tokenize_line(cmdline);

        if (strcmp(cmdline, "logout") == 0) 
            exit(1);
        else if (strcmp(cmdline, "exit") == 0)
            exit(1);
        
        if (strcmp(cmdline, "cd") == 0 && tknum == 1)
        {
            if (chdir("/") < 0)    printsCustom(outtty, "chdir unsuccessful\n");
            continue;
        }
        // need to deal with arguments!
        else if (strcmp(cmdline, "cd") == 0 && tknum == 2)
        {
            if (chdir(tok[1]) < 0)    printsCustom(outtty, "chdir unsuccessful\n");
            continue;
        }
        else if (strcmp(cmdline, "cd") == 0 && tknum > 2)
        {
            printsCustom(outtty, "too many args\n");
            continue;
        }
        
        //if (strcmp(cmdline, "\n")) continue;
        printfCustom(outtty, "\nTHIS IS %d  MY PARENT=%d\n", getpid(), getppid());

        printsCustom(outtty, "********nofal program exec********\n");
        parseArgs();
        // add piping function....
        if (numpipes > 0)
        {
            printsCustom(outtty, "mypipe\n");
            mypipe(noIOcmd);
        }
         // if not pipes run them here
        pid = fork();
        if (pid == 0)
        {
            r = exec(noIOcmd);
            if (r < 0)    printsCustom(outtty, "exec error - cmd not found\n");

            exit(1);
        }
        else
        {
            printfCustom(outtty, "PARENT %d WAITS FOR CHILD %d TO DIE\n", getpid(), pid);
            pid = wait(&status);
            printfCustom(outtty, "DEAD CHILD=%d, HOW=%04x\n", pid, status);
        }

    }
    //close(saved_stdout);
    close(intty);
    close(outtty);
}