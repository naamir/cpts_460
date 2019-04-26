#include "ucode.c"
int intty, outtty;
char tty[32];

int main(int argc, char *argv[])
{
    //close(STDIN);
    //close(STDOUT);
    // get the current tty
    //gettty(tty);
    
    //open the current tty for read and write
    intty = STDIN; //open(tty, O_RDONLY);
    //printf("intty:%d\n", intty);
    outtty = STDOUT; //open(tty, O_WRONLY);
    //printf("outtty:%d\n", outtty);

    char buf[BLKSIZE];
    int n, fd;

    prints("*******************************\n");
    prints("***********cat Nofal***********\n");
    prints("*******************************\n");

    if (argc == 1)
    {
        int l;
        char q;
        prints("no file provided - reading from stdin\n");
        getline(buf);
        while (buf[0] != 'q')
        {
            l = gets(buf);
            buf[l] = 0;
            strcat(buf, "\n");
            prints(buf);
        }
    }
    else
    {
        fd = open(argv[1], O_RDONLY);
        if (fd < 0) 
            return -1;
        
        while(n = read(fd, buf, BLKSIZE))
        {
            buf[n] = 0;  // as a null terminated string
            prints(buf);
        }     
        close(fd);
    }
    //close(intty);
    //close(outtty);
}