/* ex5.c - main, prod2, cons2 */

#include <xinu.h>

process prod2(sid32, sid32), cons2(sid32, sid32);

int32 n = 0; /* Global variable n has initial value zero */ 


/*-------------------------------------------------------------------
 * main - Producer and consume processes synchronized with semaphores
 * ------------------------------------------------------------------
 */

process main(void)
{

  sid32 produced, consumed;

  consumed = semcreate(0);
  produced = semcreate(1);
  
  resume(create(cons2, 1024, 20, "cons", 2, consumed, produced));
  resume(create(prod2, 1024, 20, "prod", 2, consumed, produced));
  return OK;
}

/*-----------------------------------------------------------------
 * produce - Increment n 2000 times, waiting for it to be consumed
 * ----------------------------------------------------------------
 */

process prod2(
              sid32 consumed,
              sid32 produced
              ) 
{
  int32 i;

  for(i=1; i<=2000; i++) {
    wait(consumed);
    n++;
    signal(produced);
  }
  return OK;
}

/*-----------------------------------------------------------------
 * consume - Print n 2000 times and exit
 * ----------------------------------------------------------------
 */

process cons2(
              sid32 consumed,
              sid32 produced) 
{
  int32 i;

  for(i=1; i<=2000; i++) {
    wait(produced);
    printf("The value of n is %d \n", n);
    signal(consumed);
  }
  return OK;
}
