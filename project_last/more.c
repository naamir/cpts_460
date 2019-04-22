#include "ucode.c"

#define MORBLKSIZE 512

int main(int argc, char *argv[])
{
    char q, buf[MORBLKSIZE];
    int n, fd;

    prints("*******************************\n");
    prints("***********more Nofal***********\n");
    prints("*******************************\n");

    fd = open(argv[1], O_RDONLY);
    if (fd < 0) return 0;
    //pfd();
    while(n = read(fd, buf, MORBLKSIZE))
    {
        buf[n] = 0;             // as a null terminated string
        prints(buf);  // <=== THIS works but not good
        //spit out chars from mybuf[ ] but handle \n properly;
        q = getc();
        if (q == 'q') break;
    }     
    close(fd);
}