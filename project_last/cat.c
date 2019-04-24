#include "ucode.c"

int main(int argc, char *argv[])
{
    char buf[BLKSIZE];
    int n, fd;

    prints("*******************************\n");
    prints("***********cat Nofal***********\n");
    prints("*******************************\n");

    if (argv[1] == 0)
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
}