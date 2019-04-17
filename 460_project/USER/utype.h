#ifndef UTYPE
#define UTYPE

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;

#define R         0
#define W         1
#define RW        2
#define APPEND    3

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

int ubody(char *name);
int ufork();
int ukfork();
int uexec();
int usleep();
int uwakeup();
int uwait();
int uexit();
int umenu();
int getpid();
int getppid();
int ugetpid();
int ugetppid();
int ups();
int uchname();
int uswitch();
int ugetc();
int uputc(char c);
int getPA();

int syscall();
int getcsr();

#endif