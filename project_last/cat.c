#include "ucode.c"

int main(int argc, char *argv[])
{
    char buf[BLKSIZE];
    int n, fd;

    prints("*******************************\n");
    prints("***********cat Nofal***********\n");
    prints("*******************************\n");

    //strcat(ar)
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) return 0;
    //pfd();
    while(n = read(fd, buf, BLKSIZE))
    {
        buf[n] = 0;             // as a null terminated string
        prints(buf);  // <=== THIS works but not good
        //spit out chars from mybuf[ ] but handle \n properly;
    }     
    close(fd);
}