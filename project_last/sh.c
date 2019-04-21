#include "ucode.c"

int tknum;
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

main(int argc, char *argv[])
{
    int r, pid, status;
    char cmdline[128];
    while (1)
    {
        memset(cmdline, 0, 128);
        prints("sh: #"); gets(cmdline);

        if (strcmp(cmdline, "\0") == 0 || cmdline[0] == ' ') 
            continue;
        //tokenize dat shit!
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
        pid = fork();
        if (pid == 0)
        {
            r = exec(cmdline);
            if (r < 0)    prints("some exec error\n");

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