#include "ucode.c"

int in, out, err, tknum;//, tknum2;
int usergid, useruid;
char username[128], password[128];
char user[32], homedir[32], prog[32];

char *tok[32];
char *myline[32];

void tokenize_line(char *line)
{
    char *cp;
    cp = line;
    tknum = 0;
    
    while (*cp != 0){
        while (*cp == ' ') *cp++ = 0;        
        if (*cp != 0)
            tok[tknum++] = cp;         
        while (*cp != ':' && *cp != 0) cp++;                  
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
    int i, t, num, n, fd, in, out, err, psw_flag;   // file descriptors for terminal I/O
    char ent_username[128], ent_password[128], buf[BLKSIZE];

    close(STDIN);
    close(STDOUT);

    in = open(argv[1], O_RDONLY);   // file descriptor 0
    out = open(argv[1], O_WRONLY);   // for disply to console
    err = open(argv[1], O_RDWR);   // for errors

    fixtty(argv[1]);
    fd = open("/etc/passwd", O_RDONLY);
    /* while (n = read(fd, buf, BLKSIZE)) {
        buf[n] = 0;
        //prints(buf);
    } */
    
    prints("NOFs LOGIN "); prints("tty: "); prints(argv[1]); prints("\n");
    while (1)
    {
        n = read(fd, buf, BLKSIZE);
        buf[n] = 0;
        num = eatpath(buf, myline);
        psw_flag = 0;
        
        prints("login_n:"); gets(ent_username);
        prints("password_n:"); gets(ent_password);
        for (i=0; i < num; i++)
        {
            //prints(myline[i]); prints("  l\n");
            //getc();
            tokenize_line(myline[i]);
            strcpy(username, tok[0]);
            //prints(username); prints("  username\n");
            if (strcmp(username, ent_username) != 0)                 
                continue; // continue here if username isn't correct, no point in going further
            
            for(t=1; t<tknum; t++) // start loop from 1 because we've already looked at the username, i.e. case 0
            {
                //prints(tok[t]); prints("  t\n");
                switch (t)
                {
                    /* case 0:
                        strcpy(username, tok[t]);
                        prints(username); prints("  username\n");
                        if (strcmp(username, ent_username) == 0) 
                        break; */
                    case 1:
                        strcpy(password, tok[t]);
                        //prints(password); prints("  password\n");
                        break;
                    case 2:
                        usergid = atoi(tok[t]);
                        //printi(usergid); prints("  usergid\n");
                        break;
                    case 3:
                        useruid = atoi(tok[t]);
                        //printi(useruid); prints("  useruid\n");
                        break;
                    case 4:
                        strcpy(user, tok[t]);
                        //prints(user); prints("  user\n");                        
                        break;
                    case 5:
                        strcpy(homedir, tok[t]);
                        //prints(homedir); prints("  homedir\n");
                        break;
                    case 6:
                        strcpy(prog, tok[t]);
                        //prints(prog); prints("  prog\n");
                        break;
                    default:
                        //prints("default");
                        break;
                }
            }

            if (strcmp(password, ent_password) == 0)
            {
                chuid(useruid, usergid);
                chdir(homedir);
                close(fd);
                exec(prog);
            }
            else
            {
                psw_flag = 1;
                prints("incorrect password!\n");
                memset(ent_username, 0, 128);
                memset(ent_password, 0, 128);
                break;
            }
        }
        
        if (!psw_flag) 
        {
            memset(ent_username, 0, 128);
            memset(ent_password, 0, 128);
            prints("username not found!\n");
        }
    }
    prints("login failed, try again\n");
}
    
