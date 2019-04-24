#include "ucode.c"

#define MORBLKSIZE 512

char q, mline[128];


int getmorline(int fd, char *line) //, char *buf)
{
    int n, i=0;
    char c;
    
    while (c != '\n')
    {
        //getc();
        n = read(fd, &c, 1);
        if (n == 0) 
        {
            line[i] = 0;
            return EOF;
        }
        //mputc(c);
        line[i++] = c;
    }
    line[i] = 0;
}

int main(int argc, char *argv[])
{    
    int i, n, fd, intty, outtty, nbytes, offset;
    char tty[32];
    prints("*******************************\n");
    prints("***********more Nofal***********\n");
    prints("*******************************\n");

    // get the current tty
    gettty(tty);

    // open the current tty for read and write
    intty = open(tty, O_RDONLY);
    outtty = open(tty, O_WRONLY);

    if (argv[1] != 0)
    {
        fd = open(argv[1], O_RDONLY);
        if (fd < 0) return 0;
    }

    char q;
    int r = 0, nline = 15;

    while (q != 'q' && r != EOF)
    {
        for(i = 0; i < nline; i++)
        {
            r = getmorline(fd, mline);
            if (r == EOF)
                break;
            prints(mline);
        }
        q = getc();
        if (q == '\r')
            nline = 1;
        else
            nline = 15;
    }
    close(fd);
    close(intty);
    close(outtty);
}