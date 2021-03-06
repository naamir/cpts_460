#define NPIPE 10
#define PSIZE  8

typedef struct pipe{
  char buf[8];
  int head, tail;
  int data, room;
}PIPE;

PIPE pipe;

int show_pipe()
{
  PIPE *p = &pipe;
  int i;
  printf("----------------------------------------\n");
  printf("room=%d data=%d buf=", p->room, p->data);
  for (i=0; i<p->data; i++)
    kputc(p->buf[p->tail+i]);
  printf("\n");
  printf("----------------------------------------\n");
}
int kpipe()
{
  int i;
  PIPE *p = &pipe;
  p->head = p->tail = 0;
  p->data = 0; p->room = PSIZE;
}

int read_pipe(PIPE *p, char *buf, int n)
{
  int ret;
  char c;
  
  if (n<=0)
    return 0;
  show_pipe();

  while(n){
    printf("reader %d reading pipe\n", running->pid);
    ret = 0;
    while(p->data){
        *buf = p->buf[p->tail++];
        p->tail %= PSIZE;
        buf++;  ret++; 
        p->data--; p->room++; n--;
        if (n <= 0)
            break;
    }
    show_pipe();
    if (p->data == 0) {
      printf("reader has read everything! scram\n");
      return 0;
    }
    if (ret){ /* has read something */
       kwakeup(&p->room);
       return ret;
    }
    // pipe has no data
    //printf("reader %d sleep for data\n", running->pid);
    //kwakeup(&p->room);
    //ksleep(&p->data);
    //continue;
  }
}

int write_pipe(PIPE *p, char *buf, int n)
{
  char c;
  int ret = 0; 
  show_pipe();
  while (n){
    printf("proc %d writing to pipe\n", running->pid);
    while (p->room){
      p->buf[p->head++] = *buf; 
      p->head  %= PSIZE;
      buf++;  ret++; 
      p->data++; p->room--; n--;
      if (n<=0){
        show_pipe();
        kwakeup(&p->data);
        return ret;
      }
    }
    show_pipe();
    printf("writer %d sleep for room\n", running->pid);
    kwakeup(&p->data);
    ksleep(&p->room);
  }
}
 
int pipe_reader()
{
  char line[128];
  int nbytes, n, iline;
  PIPE *p = &pipe;
  printf("proc %d as pipe reader\n", running->pid);

  while(1){
    printf("input nbytes to read : " );
    //scanf("%d", &nbytes);
    //kgets(line);
    //line[strlen(line)] = 0;
    iline = geti();
    printf("nbts:%d\n", iline);
    if (iline == 0) {
      kwakeup(&p->room);
      kexit(&running);
    }

    nbytes = iline;
    
    n = read_pipe(p, line, nbytes);
    line[n] = 0;
    printf("Read n=%d bytes : line=%s\n", n, line);
  }
}

int checkReaders(PROC *p)
{
  int procCount = 0;
  printQ(readyQueue);
  while (p) {
    procCount++;
    p = p->next;
  }
  if (procCount <= 1) {
    printf("we got ourselves a BROKEN PIPE!\n");
    kexit(running);
  }

}

int pipe_writer()
{
  char line[128];
  int nbytes, n;
  PIPE *p = &pipe;

  //if (!running->child)
  checkReaders(readyQueue);
  printQ(readyQueue);
  printf("proc %d as pipe writer\n", running->pid);

  while(1){
    printf("input a string to write or ENTER to exit: " );

    kgets(line);
    line[strlen(line)] = 0;
    printf("line:%s\n", line);
    if (line[0] == 0)
       kexit(&running);
    if (strcmp(line, "") == 0)
       continue;

    nbytes = strlen(line);
    printf("nbytes=%d buf=%s\n", nbytes, line);
    n = write_pipe(p, line, nbytes);

    checkReaders(readyQueue);

    printf("wrote n=%d bytes\n", n);
  }
}


