#include "ucode.c"

char q, mline[128];

int getmorline(char *line) //, char *buf)
{
    int n, i=0;
    char c;
    
    while (c != '\n' && c != '\r' && c != EOF)
    {
        //getc();
        n = read(0, &c, 1);
        if (n == 0)
        {
            line[i] = 0;
            return EOF;
        }
        mputc(c);
        line[i++] = c;
    }
    line[i] = 0;
    return i;
}

int main(int argc, char *argv[])
{
    int r = 0, n, fd, intty, outtty, nmatches = 0, patlen;
    char buf[128], tty[32];

    prints("********************************\n");
    prints("***********grep Nofal***********\n");
    prints("********************************\n");

    patlen = strlen(argv[1]);
    // get the current tty
    gettty(tty);
    
    // open the current tty for read and write
    intty = open(tty, O_RDONLY);
    outtty = open(tty, O_WRONLY);

    if (argc > 2)  // too many args
    {
        prints("too many args\n");
        return 0;
    }
    else if (argc == 1)
    {
        prints("enter a pattern\n");
        return 0;
    }
    else
    {
        while (r != EOF)
        {
            int i=0, n;
            char temp[64];
            char *cp, *cq;

            r = getmorline(mline);
            //printf("r:%d", r);
            // have no idea why we have to print a blank char to have grep work when getting
            // data from a pipe, if this isn't there it becomes a broken pipe...
            mputc(' ');
            if (r < 0) break;
            //prints(mline);
            cp = mline;
            cq = argv[1];

            while (*cp != '\n')
            {

                //printf("cp:%c, cq:%c  ", *cp, *cq);
                if (*cp == *cq)
                {
                    char *tcp;
                    tcp = cp;
                    //getcCustom(intty);
                    
                    for (n=0; n < patlen; n++)
                    {
                        temp[n] = *tcp;
                        tcp++;
                    }
                    temp[patlen] = 0;
                    //printf("temp %s\n", temp);
                }

                if (strcmp(temp, argv[1]) == 0)
                {
                    //prints("found pattern!!!\n");
                    memset(&temp, 0, patlen);
                    nmatches++;
                    //i = 0;
                }
                
                cp++;
            }
            if (nmatches > 0)
            {
                nmatches = 0;
                prints(mline);
            }
            
        }
        //mputcCustom(outtty, '\r');//getcCustom(intty);
    }
    close(intty);
    close(outtty);
    //getcCustom(intty);
}