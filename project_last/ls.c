#include "ucode.c"

char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";

int intty, outtty;
char tty[32];

int ls_file(char *fname)
{
	struct stat fstat, *sp;
	int r, i;
    char sbuf[4096];
	char ftime[64];
	sp = &fstat;
	
	if ( (r = stat(fname, &fstat)) < 0){
		printfCustom(outtty,  "can't stat %s\n", fname);
		exit(1);
	}
    //printf("mode hex %x \n",sp->st_mode); // link count
	if ((sp->st_mode & 0xF000) == 0x8000) // if (S_ISREG())
		printsCustom(outtty, "-");  // printf("%c","-");
	if ((sp->st_mode & 0xF000) == 0x4000) // if (S_ISDIR())
		printsCustom(outtty, "d");  //printf("%c","d");
	if ((sp->st_mode & 0xF000) == 0xA000) // if (S_ISLNK())
		printsCustom(outtty, 'l');  //printf("%c",'l');
	
	for (i=8; i >= 0; i--){
		if (sp->st_mode & (1 << i)) // print r|w|x
			mputcCustom(outtty, t1[i]);  // printf("%c", t1[i]);
		else
			mputcCustom(outtty, t2[i]); // or print -  //printf("%c", t2[i]);
	}
	printfCustom(outtty,  "%d ",sp->st_nlink); // link count
	printfCustom(outtty,  "%d ",sp->st_gid); // gid
	printfCustom(outtty,  "%d ",sp->st_uid); // uid
	printfCustom(outtty,  "%d ",sp->st_size); // file size

	// print time
	// strcpy(ftime, ctime(&sp->st_ctime)); // print time in calendar form // (time_t)ctime(&sp->st_ctime)
	// ftime[strlen(ftime)-1] = 0; // kill \n at end
	// printf("%s ",ftime);

	// print name
	printfCustom(outtty,  "%s", fname); // print file basename // basename(fname)
	// print -> linkname if symbolic file
	if ((sp->st_mode & 0xF000)== 0xA000){  // if (S_ISLNK())
		//use readlink() to read linkname
        r = readlink(fname, sbuf);
		printfCustom(outtty,  " -> %s<p>", sbuf); // print linked name
	}
	printsCustom(outtty, "\n");
}


int main(int argc, char *argv[])
{
    close(STDIN);
    close(STDOUT);
    // get the current tty
    gettty(tty);
    
    //open the current tty for read and write
    intty = STDIN; //open(tty, O_RDONLY);
    //printf("intty:%d\n", intty);
    outtty = open(tty, O_WRONLY);
    //printf("outtty:%d\n", outtty);


    printsCustom(outtty, "********************************\n");
    printsCustom(outtty, "***********ls Nofal*************\n");
    printsCustom(outtty, "********************************\n");
    struct stat mystat, *sp;
    int i, r, n, fd;
    DIR *dp;
    char *cp;
    char filename[1024], cwd[1024], buf[BLKSIZE], temp[256];

    sp = &mystat;
    
    // get the current working directory
    getcwd(cwd);
    //strcpy(filename, cwd);
    if (argc > 1)
    {
        // ls a dir
        fd = open(argv[1], O_RDONLY);
        //strcpy(filename, argv[1]);
        if (fd < 0){
            printsCustom(outtty, "can't open dir\n");
            return 0;
        }
        chdir(argv[1]);
    }
    else
    {
        // no parameter so ls the current directory
        fd = open(cwd, O_RDONLY);
        if (fd < 0){
            printsCustom(outtty, "can't open cwd\n");
            return 0;
        }
    }
    
    n = read(fd, buf, BLKSIZE);
    dp = (DIR *)buf;
    cp = buf;

    //printf("inode    rec len    name len    name\n");
    while (cp < buf + 1024)
    {        
        strncpy(filename, dp->name, dp->name_len);
        filename[dp->name_len] = 0; // removing end of str delimiter '/0', why tho?
        ls_file(filename);

        cp += dp->rec_len;
        dp = (DIR *)cp;
    }
    close(intty);
    close(outtty);
}

