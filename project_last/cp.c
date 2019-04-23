#include "ucode.c"

int main(int argc, char *argv[])
{
    prints("***********cp Nofal*************\n");
    int fd, gd, n, r;
    char buf[BLKSIZE];
    
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        printf("couldn't open %s\n", argv[1]);
        return -1;
    }
    gd = open(argv[2], O_RDWR);
    if (gd < 0)
    {
        printf("couldn't open %s\n", argv[2]);
        prints("creating new file\n");
        creat(argv[2]);
        gd = open(argv[2], O_RDWR);
    }

    while( n = read(fd, buf, BLKSIZE) ) {
        r = write(gd, buf, n);  // notice the n in write()
        if (r < 0) prints("write failed");
    }

    close(fd);
    close(gd);
}