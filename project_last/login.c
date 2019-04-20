#include "ucode.c"

#define IN      0
#define OUT     1
#define BLKSIZE 1024

int in, out, err, tknum;
char lname[128], password[128];
char *tok[32];

void tokenize(char *line)
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
    int i, n, fd, in, out;   // file descriptors for terminal I/O
    char buf[BLKSIZE];
    char *myline[32];

    close(IN);
    close(OUT);

    in = open(argv[1], O_RDONLY);   // file descriptor 0
    out = open(argv[1], O_WRONLY);   // for disply to console

    fixtty(argv[1]);
    fd = open("/etc/passwd", O_RDONLY);
    while (n = read(fd, buf, BLKSIZE)) {
        buf[n] = 0;
        //prints(buf);
    }
    eatpath(buf, myline);  // function converts what we've read into lines
    for (i=0; i < n; i++){
        if (myline[i]){
            prints(myline[i]); prints("  ");
        }
    }
    while (1)
    {
        prints("login_nofal:"); gets(name);
        prints("password_nofal:"); gets(password);
    }
    
}