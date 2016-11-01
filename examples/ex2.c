/* ex2.c - main, sndA, sndB */

#include <xinu.h>

process sndA(void), sndB(void);

/*---------------------------------------------
 * main - Example of creating processes in Xinu
 * --------------------------------------------
 */

process main(void)
{
  resume(create(sndA, 1024, 20, "Process 1", 0));
  resume(create(sndB, 1024, 20, "Process 2", 0));
  return OK;
}

process sndA(void)
{
  while(1) {
    putc(CONSOLE, 'A');
  }
  return OK;
}

process sndB(void)
{
  while(1) {
    putc(CONSOLE, 'B');
  }
  return OK;
}


      
  
