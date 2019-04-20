#ifndef UTYPE
#define UTYPE

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;

int parseArg(char *line);
int ubody(char *name);

int ugetline(char *line);
int uprints(char *s);
void urpx(u32 x);
int uprintx(u32 x);
void urpu(u32 x);
int uprintu(u32 x);
int uprinti(int x);
int uprintf(char *fmt,...);


int kprintf(char *, ...);
int strlen(char *s);
int strcmp(char *s1, char *s2);
int strcpy(char *dest, char *src);
int kstrcpy(char *dest, char *src);
int atoi(char *s);
int geti();

void mputs(char *s);
// int ubody(char *name);

// int ukfork();
// int usleep();
// int uwakeup();
// int uwait();
// int uexit();
// int umenu();

// int ups();
// int uchname();
// int uswitch();
// int ugetc();
// int uputc(char c);
// int getPA();

int syscall();
int getcsr();

void token(char *line);
void showarg(int argc, char *argv[ ]);
void main0(char *s);

int getc();
int getline(char *s);
int gets(char *s);
int getuid();
int getpid();
int getppid();
int getpri();
int chpri(int value);
int chuid(int uid, int gid);
int fork();
int exec(char *cmd_line);
int wait(int *status);



#endif