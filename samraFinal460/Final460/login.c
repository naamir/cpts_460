/****************** Algorithm of login *******************/
// login.c : Upon entry, argv[0]=login, argv[1]=/dev/ttyX
#include "ucode.c"

int in, out, err;
char username[128],password[128];
char buf[1024];

main(int argc, char *argv[])
{
    // (1). close file descriptors 0,1 inherited from INIT.
    int sizeOfFile = 0;
    char *token [100], *line[100];
    int fd =0;
    int n =0;
    int i =0;
    close(0);
    close(1);
    // (2). open argv[1] 3 times as in(0), out(1), err(2).
    in = open(argv[1], O_RDONLY);  
    out = open(argv[1], O_WRONLY);  
    err = open(argv[1], O_RDWR);   

    // (3). settty(argv[1]); // set tty username string in PROC.tty
    fixtty(argv[1]);
    // (4). open /etc/passwd file for READ;
    fd = open("/etc/passwd", O_RDONLY);
    while(1){
    // (5).
    // printf("login:");
    // gets(username);
    // printf("password:"); gets(password);
    

    printf("login:");
    gets(username);

    printf("password:");
    gets(password);

    n = read(fd, buf, 1024);
    if (n <= 0)
    {
      printf("Empty file --- No passwords\n");
    }
    printf(n);
    sizeOfFile = tokenize(line[i], token , ':');
    // for each line in /etc/passwd file do{
    // tokenize user account line;
    
    // userusername:password:gid:uid:fullusername:HOMEDIR:program
    // e.g.    root:xxxxxxx:1000:0:superuser:/root:sh
    for (i=0; i<sizeOfFile; i++)
    {
        tokenize(line[i], token , ':');


    // (6).
    // if (user has a valid account)
    
      if (strcmp(username, token[0]) == 0 && strcmp(password, token[1]) == 0)
      {
        /* 7. change uid, gid to user's uid, gid // chuid()
              change cwd to user's home DIR // chdir()
              close opened /ect/passwd file // close()*/
        chuid(atoi(token[2]), atoi(token[3]));
        chdir(token[5]);
        close(fd);

        // 8. exec to program in user account // exec()
        printf("SMLOGIN: Welcome! %s\n", token[0]);
        printf("SMLOGIN: cd to HOME=%s change uid to %d\n", token[5], atoi(token[2]));
        exec(token[6]);
        return 1;
    }
   }
    prints("login failed, try again\n");
    memset(username, 0, 128);
    memset(password, 0, 128);
  }
}