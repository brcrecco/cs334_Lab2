/* ex3a.c - main, sndch */

#include <xinu.h>

process sndch(char);

/*---------------------------------------------------------
 * main - Example of creating and killing processes in Xinu
 * --------------------------------------------------------
 */

process main(void)
{
  pid32 pidSndA, pidSndB;
  
  pidSndA = create(sndch, 1024, 20, "send A", 1, 'A');
  pidSndB = create(sndch, 1024, 20, "send B", 1, 'B');
  printf("Starting send A with pid %d.\n", pidSndA);
  printf("Starting send B with pid %d.\n", pidSndB);
  resume(pidSndA);
  resume(pidSndB);
  sleepms(1000); /* sleep for one second */
  kill(pidSndA);
  sleepms(1000); /* sleep for one second */
  kill(pidSndB);
  return OK;
}

process sndch(
              char ch  /* The character to emit continuously */
              )
{
  while(1) {
    putc(CONSOLE, ch);
  }
  return OK;
}
