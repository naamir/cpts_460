/***************** tree.c file **************
Implementing the tree structure fot the processes
****************************************/
//#include "type.h"

int insertChild(PROC *rproc, PROC *fc)  // rproc->running process , fc->forked child
{
   PROC *p = rproc;
   PROC *c = p->child;

   if (p->child != NULL){

      while(c->sibling != NULL)
         c = c->sibling;
      c->sibling = fc;
      c->sibling->parent = c;
   }
   else {
      p->child = fc;
      p->child->parent = p;
   }
}

int removeChild(PROC *rproc)
{
   // rproc is being removed (becoming a ZOMBIE) hence being removed as the child
   PROC *p = rproc;
   if (p->parent->child == p)
      p->parent->child = NULL;
   else if (p->parent->sibling == p)
      p->parent->sibling = NULL;
   return 0;
}

int printChildren(char *name, PROC *p) 
{
  printf("%s = ", name);

  if (p->child) {
     printf("[%d %d]->", p->pid, p->priority);
     p = p->child;

     while(p->sibling){
         printf("[%d %d]->", p->pid, p->priority);
         p = p->sibling;
      }
  }
  printf("NULL\n");
}