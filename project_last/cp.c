#include "ucode.c"

int main(int argc, char *argv[])
{
    prints("***********cp Nofal*************\n");
    int fd, gd, n, r;
    char buf[BLKSIZE];
    printf("arg1 %s\n", argv[1]); printf("arg2 %s\n", argv[2]);
    fd = open(argv[1], O_RDONLY);
    gd = open(argv[2], O_RDWR|O_CREAT);

    while( n = read(fd, buf, BLKSIZE) ) {
        prints("***********cp Nofal*************\n");
        printf("n %d\n", n);
        r = write(gd, buf, n);  // notice the n in write()

        if (r<0) prints("write failed");
        //t = getc();
        //mputc(t);
    }

    close(fd); 
    close(gd);
}