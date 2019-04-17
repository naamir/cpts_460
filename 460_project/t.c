/********************************************************************
Copyright 2010-2017 K.C. Wang, <kwang@eecs.wsu.edu>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#include "type.h"
#include "string.c"
#define VA(x) (0x80000000 + (u32)x)

char *tab = "0123456789ABCDEF";
int BASE;
int color;

#include "uart.c"
#include "kbd.c"
#include "timer.c"
#include "vid.c"
#include "exceptions.c"
#include "queue.c"
#include "kernel.c"
#include "fork.c"
#include "fork-exec.c"
char *status[ ] = {"FREE", "READY", "SLEEP", "ZOMBIE", "BLOCK"};
#include "tree.c"
#include "svc.c"

#include "type.h"
#include "sdc.c"
#include "yourload.c"


int initfs()
{
	int i, n;
	MINODE *mip;
	PROC *p;

	printf("initfs()\n");

	for (i = 0; i < NMINODE; i++)
	{
		mip = &minode[i];
		// set all entries to 0;
		mip->refCount = 0;
		mip->dirty = 0;
		mip->mounted = 0;
		mip->mptr = 0;
	}

	for (i = 0; i < NPROC; i++)
	{
		p = &proc[i];
		p->pid = i;
		p->uid = i;
		p->cwd = 0;

		// init File Descriptors
		//getchar();
		for (n = 0; n < NFD; n++)
		{
			oftp[n].refCount = 0;
			oftp[n].mode = 0;
			oftp[n].mptr = 0;
			oftp[n].offset = 0;

			p->fd[n] = &oftp[n]; // note that all fds are now pointing
							  // to same location - don't know if this is necessary
		}
	}
}

// load root INODE and set root pointer to it
int mount_root()
{
	char buf[BLKSIZE];
	//SUPER *sp;
	//GD *gp;
	//MINODE mip;

	printf("mount_root()\n");
	/*
	 (1). read super block in block #1;
	 verify it is an EXT2 FS;
	 record nblocks, ninodes as globals;
       */
	/* get super block of the rootdev */
	get_block(dev, 1, buf);
	sp = (SUPER *)buf;
	/* check magic number */
	if (sp->s_magic != SUPER_MAGIC)
	{
		printf("super magic=%x : %s is not an EXT2 file system\n",
					 sp->s_magic, disk);
		exit(0);
	}
	nblocks = sp->s_blocks_count;
	ninodes = sp->s_inodes_count;
	/*
	 (2). get GD0 in Block #2:
	 record bmap, imap, inodes_start as globals
       */
	get_block(dev, 2, buf);
	gp = (GD *)buf;
	bmap = gp->bg_block_bitmap;
	imap = gp->bg_inode_bitmap;
	iblk = gp->bg_inode_table;
	/*
	 (3). root = iget(dev, 2);       // get #2 INODE into minode[ ]
	 printf("mounted root OK\n");
	*/

	root = iget(dev, 2);
	printf("root: dev=%d, ino=%d\n", root->dev, root->ino);
	printf("root refCount = %d\n", root->refCount);

	printf("creating P0 as running process\n");
	
	proc[0].status = READY;
	proc[0].cwd = iget(dev, 2);
	running = &proc[0];
	// set proc[1]'s cwd to root also
	proc[1].cwd = iget(dev, 2);

	printf("mounted root OK\n");
}

void reset()
{
	int i;
	for (i=0; i<256; i++)
		pathname[i] = 0;

	for (i=0; i<64; i++)
		name[i] = 0;
	
	for (i=0; i<32; i++)
		cmd[i] = 0;
}

void copy_vectors(void) {
    extern u32 vectors_start;
    extern u32 vectors_end;
    u32 *vectors_src = &vectors_start;
    u32 *vectors_dst = (u32 *)0;
    while(vectors_src < &vectors_end)
       *vectors_dst++ = *vectors_src++;
}

int mkPtable()
{
    int i;
    u32 *ut = (u32 *)0x4000;  // at 16KB
    u32 entry = 0 | 0x412;    // 0x412; AP=01 (K R|W; U NO) domain=0 CB=00
    for (i=0; i<4096; i++)
        ut[i] = 0;
    for (i=0; i<258; i++){
        ut[i] = entry;
        entry += 0x100000;
    }
}

int kprintf(char *fmt, ...);
void timer0_handler();

void IRQ_handler()
{
    int vicstatus, sicstatus;
    int ustatus, kstatus;

    // read VIC status register to find out which interrupt
    vicstatus = VIC_STATUS;
    sicstatus = SIC_STATUS;  

    if (vicstatus & 0x0010){
         timer0_handler();
    }
    if (vicstatus & 0x1000){
         uart_handler(&uart[0]);
    }
    if (vicstatus & 0x2000){
         uart_handler(&uart[1]);
    }
    if (vicstatus & (1<<31)){
      if (sicstatus & (1<<3)){
          kbd_handler();
       }
       // SDC interrupt at 22 on SIC
       if (sicstatus & (1<<22)){
          sdc_handler();
       }
    }
}

int main()
{ 
    int i,a; 
    char line[128]; 
    UART *up;

    color = RED;
    row = col = 0; 
    BASE = 10;

    fbuf_init();
    kprintf("                     Welcome to WANIX in Arm\n");
    kprintf("LCD display initialized : fbuf = %x\n", fb);
    color = CYAN;
    kbd_init();

    /* enable UART IRQ */
    VIC_INTENABLE |= (1<<4);     // timer0,1 at 4 
    VIC_INTENABLE |= (1<<12);    // UART0 at 12
    VIC_INTENABLE |= (1<<13);    // UART1 at 13
    VIC_INTENABLE |= (1<<31);    // SIC to VIC's IRQ31

    /* enable UART0 RXIM interrupt */
    UART0_IMSC = 1<<4;
    /* enable UART1 RXIM interrupt */
    UART1_IMSC = 1<<4;
    /* enable KBD and SDC IRQ */
    SIC_ENSET    |= (1<<3);   // KBD int=3 on SIC
    SIC_ENSET    |= (1<<22);  // SDC int=22 on SIC

    unlock();

    timer_init();
    timer_start(0);
    uart_init();
    up = &uart[0];
    ufprintf(up, "test UART\n");
    sdc_init();
    kernel_init();

    kfork("u1");
    //kfork("u2");
    //kfork("u3");
    //kfork("u4");

    unlock();
    color = WHITE;
    kprintf("P0 switch to P1\n");

    while(1){
        unlock();
        if (readyQueue)
            tswitch();
    }
}
