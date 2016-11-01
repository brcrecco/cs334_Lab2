/* ex3.c - main, sndch */

#include <xinu.h>

process sndch(char);

/*---------------------------------------------
 * main - Example of creating processes in Xinu
 * --------------------------------------------
 */

process main(void)
{
  resume(create(sndch, 1024, 20, "send A", 1, 'A'));
  resume(create(sndch, 1024, 20, "send B", 1, 'B'));
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
