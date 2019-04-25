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
        //mputc(c);
        line[i++] = c;
    }
    line[i] = 0;
    return n;
}

int getcCustom(int pd)
{
    int c, n;
    n = read(pd, &c, 1);

    /********************************************************************* 
     getc from KBD will NOT get 0 byte but reading file (after redirect 0 
    to file) may get 0 byte ==> MUST return 2-byte -1 to differentiate.
    **********************************************************************/

    if (n==0 || c==4 || c==0 ) return EOF;  
                                    
    return (c&0x7F);
}
/* 
int strncmp(char *s1, char *s2, int n)
{
    while((*s1 && *s2) && (*s1==*s2) && n){
        s1++; s2++; n--;
    }
    if ((*s1==0) && (*s2==0))
        return 0;
    return(*s1 - *s2);
} */

int patterncheck(char *pat, int patlen, char *string, int strl, int intty)
{
    int i, n;
    //patlen = strlen(pat);
    //char *temp;

    //temp = (char *)malloc(patlen * sizeof(char));
/*     for (i = 0; i < strl; i++)
    {
        //strncmp(pat, string, patlen);
        for (n = 0; n < patlen; n++)
        {
            if (pat[n] == string[i])

        }
    }  */
    while (strncmp(pat, string, patlen) != 0)
    {
        getcCustom(intty);
        string++;
    }

}

int main(int argc, char *argv[])
{
    int i, r=0, n, fd, intty, outtty;
    char buf[128], tty[32];

    prints("********************************\n");
    prints("***********grep Nofal***********\n");
    prints("********************************\n");

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
            int i=0;
            char temp[64];
            char *cp;
            r = getmorline(mline);
            //prints("test1\n");
            printf("strncmp args: %d, %s\n", strlen(argv[1]), argv[1]); //, mline);
            prints(mline);
            
            //if (patterncheck(argv[1], strlen(argv[1]), mline, strlen(mline), intty) == 0)
            //    prints("found pattern!!!\n");
            strncpy(temp, mline, strlen(argv[1]));
            while (strncmp(argv[1], temp, strlen(argv[1])) != 0)
            {
                prints("test");
                getcCustom(intty);
                mline[i] = mline[i+1];
                strcpy(temp, mline);
                i++;
            }
            
        }
    }   
}