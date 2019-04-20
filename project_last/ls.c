//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/stat.h>
//#include <time.h>
//#include <sys/types.h>
//#include <dirent.h>

//#include "utype.h"
#include "ucode.c"

char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";

int ls_file(char *fname)
{
	struct stat fstat, *sp;
	int r, i;
    char sbuf[4096];
	char ftime[64];
	sp = &fstat;
	
	if ( (r = stat(fname, &fstat)) < 0){
		printf("can't stat %s\n", fname);
		exit(1);
	}
    printf("mode hex %x \n",sp->st_mode); // link count
	if ((sp->st_mode & 0xF000) == 0x8000) // if (S_ISREG())
		prints("-");  // printf("%c","-");
	if ((sp->st_mode & 0xF000) == 0x4000) // if (S_ISDIR())
		prints("d");  //printf("%c","d");
	if ((sp->st_mode & 0xF000) == 0xA000) // if (S_ISLNK())
		prints('l');  //printf("%c",'l');
	
	for (i=8; i >= 0; i--){
		if (sp->st_mode & (1 << i)) // print r|w|x
			mputc(t1[i]);  // printf("%c", t1[i]);
		else
			mputc(t2[i]); // or print -  //printf("%c", t2[i]);
	}
	printf("%d ",sp->st_nlink); // link count
	printf("%d ",sp->st_gid); // gid
	printf("%d ",sp->st_uid); // uid
	printf("%d ",sp->st_size); // file size

	// print time
	// strcpy(ftime, ctime(&sp->st_ctime)); // print time in calendar form // (time_t)ctime(&sp->st_ctime)
	// ftime[strlen(ftime)-1] = 0; // kill \n at end
	// printf("%s ",ftime);

	// print name
	printf("%s", fname); // print file basename // basename(fname)
	// print -> linkname if symbolic file
	if ((sp->st_mode & 0xF000)== 0xA000){  // if (S_ISLNK())
		//use readlink() to read linkname
        r = readlink(fname, sbuf);
		printf(" -> %s<p>", sbuf); // print linked name
	}
	printf("\n");
}

/* int ls_dir(char *dname)
{
    char name[256];    // EXT2 filename: 1-255 chars
    DIR *dp;
    struct dirent *ep;
    // open DIR to read names
    dp = opendir(dname);  // opendir() syscall
    while(ep = readdir(dp)) { // readdir() syscall
        strcpy(name, ep->d_name);
        if (!strcmp(name, ".") || !strcmp(name, ".."))
            continue;   // skip over . and .. entries
        strcpy(name, dname);
        strcat(name, "/");
        strcat(name, ep->d_name);
        ls_file(name);
    }
} */

int main(int argc, char *argv[])
{
    prints("********************************\n");
    prints("***********ls Nofal*************\n");
    prints("********************************\n");
    struct stat mystat, *sp;
    int r, ino;
    DIR *d;
    char *s;
    char filename[1024], cwd[1024];
    s = argv[1];   // ls [filename]
    if (argc == 1)   // no parameter: ls CWD
        s = "./";
    sp = &mystat;
    if ((r = stat(s, sp)) < 0) {  // stat() syscall
        prints("stat failed in ls\n"); 
        exit(1);
    }
    strcpy(filename, s);
    if (s[0] != '/') {   // filename is relative to CWD
        getcwd(cwd);   // get CWD path
        strcpy(filename, cwd);
        strcat(filename, "/");
        strcat(filename, s);   // construct $CWD/filename
    }
    //if ((sp->st_mode & 0xF000) == 0x8000) // if (S_ISREG())
        //ls_dir(filename);   // list DIR
    //else
    //ino = getino(pathname);
	//mip = iget(dev, ino);
    ls_file(filename);  // list single file
}

