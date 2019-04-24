#include "ucode.c"

int tknum;
int numpipes, fd;//, pipeflag;
char *tok[32];

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

/* int bepipin()
{
    int pd[2], pid, r = 0;

    pipe(pd);        // creates a PIPE; pd[0] for READ  from the pipe, 
    //                 pd[1] for WRITE to   the pipe.

    pid = fork();    // fork a child process
    // child also has the same pd[0] and pd[1]

    if (pid){        // parent as pipe WRITER
        close(pd[0]); // WRITER MUST close pd[0]
        close(1);     // close 1
        dup(pd[1]);   // replace 1 with pd[1]
        close(pd[1]); // close pd[1] since it has replaced 1
        
        for (int c = 0; c < no_paths; c++)
        {
            getCommand(c, head);
            r = execve(command);   // change image to cmd1
        }
        if (r < 0)    printf("errno=%d : %s\n", errno, strerror(errno));

        exit(1);  // exit pipe WRITER
    }
    else{            // child as pipe READER
        close(pd[1]); // READER MUST close pd[1]
        close(0);  
        dup(pd[0]);   // replace 0 with pd[0]
        close(pd[0]); // close pd[0] since it has replaced 0
        
        for (int c = 0; c < no_paths; c++)
        {
            getCommand(c, tail);
            r = execve(command, tail, env);   // change image to cmd2
        }
        if (r < 0)    printf("errno=%d : %s\n", errno, strerror(errno));

        exit(1);  // exit pipe READER
    }
} */
int bepipin(char *cmd1, char *cmd2)
{
    printf("cmd1:%s cmd2:%s\n", cmd1, cmd2);
    int pd[2], pid, r = 0;

    pipe(pd);   // creates a PIPE; pd[0] for READ, pd[1] for WRITE
    pid = fork();  // fork a child to share a pipe

    if (pid)  // parent: as pipe READER
    {
        close(pd[1]);    // close pipe WRITE end
        dup2(pd[0], 0);  // redirect stdin to pipe READ end
        exec(cmd2);
    }
    else      // child: as pipe WRITER
    {
        close(pd[0]);    // close pipe READ end
        dup2(pd[1], 1);  // redirect stout to pipe WRITE end
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
            printf("Redirect Input < infile\n");  // take inputs from infile
            fd = open(tok[n+1], O_RDONLY);  // open filename for READ, which
                                                // will replace fd 0
            dup2(fd, 0);    // system call to close file descriptor 0
        }
        else if (strcmp(tok[n], ">") == 0)
        {
            printf("Redirect Output > outfile\n");  // send outputs to outfile
            fd = open(tok[n+1], O_WRONLY|O_CREAT); // open filename for WRITE, which
                                                            // will replace fd 0
            dup2(fd, 1);    // system call to close file descriptor 1
            close(fd);
        }
        else if (strcmp(tok[n], ">>") == 0)
        {
            printf("Redirect Output >> outfile APPEND\n");  // APPEND outputs to outfile
            fd = open(tok[n+1], O_WRONLY|O_CREAT|O_APPEND); // open filename for WRITE, which
                                                            // will replace fd 0
            dup2(fd, 1);    // system call to close file descriptor 0
            close(fd);
        }
        else if (strcmp(tok[n], "|") == 0)
        {
            printf("has | let the pipin beginnnn\n");  // pipe dat shiz
            //pipeflag = 1;
            numpipes++;
            printf("numpipes:%d\n", numpipes);
        }
    }
}

main(int argc, char *argv[])
{
    int r, pid, status;
    char cmdline[128], noIOcmd[128];
    while (1)
    {
        memset(cmdline, 0, 128);
        numpipes = 0;
        prints("sh: #"); gets(cmdline);
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
            if (chdir("/") < 0)    prints("chdir unsuccessful\n");
            continue;
        }
        // need to deal with arguments!
        else if (strcmp(cmdline, "cd") == 0 && tknum == 2)
        {
            if (chdir(tok[1]) < 0)    prints("chdir unsuccessful\n");
            continue;
        }
        else if (strcmp(cmdline, "cd") == 0 && tknum > 2)
        {
            prints("too many args\n");
            continue;
        }
        
        //if (strcmp(cmdline, "\n")) continue;
        printf("\nTHIS IS %d  MY PARENT=%d\n", getpid(), getppid());

        prints("********nofal program exec********\n");
        parseArgs();
        // add piping function....
        if (numpipes > 0)
        {
            prints("bepipin\n");
            char cmd1[64], cmd2[64];
            strcpy(cmd1, tok[0]);   strcpy(cmd2, tok[3]);
            strcat(cmd1, " ");      //strcat(cmd2, " ");
            strcat(cmd1, tok[1]);   //strcat(cmd2, tok[4]);
            bepipin(cmd1, cmd2);
        }

        pid = fork();
        if (pid == 0)
        {
            
            //prints("cmd2: "); prints(noIOcmd); prints("\n");
            
            r = exec(noIOcmd);
            if (r < 0)    prints("exec error - cmd not found\n");

            exit(1);
        }
        else
        {
            printf("PARENT %d WAITS FOR CHILD %d TO DIE\n", getpid(), pid);
            pid = wait(&status);
            printf("DEAD CHILD=%d, HOW=%04x\n", pid, status);
        }

    }
    
}