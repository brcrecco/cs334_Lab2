/* ex1.c - main */

#include <xinu.h>

/*--------------------------------------
 * main - Write "hi" on the console
 * --------------------------------------
 */

process main(void)
{
  putc(CONSOLE, 'h');
  putc(CONSOLE, 'i');
  putc(CONSOLE, '\n');

  return OK;
}

  
