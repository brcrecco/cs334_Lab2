#include <xinu.h>

uint32 RcvMsgWait(int32 maxwait, umsg32* msgs, uint32 msg_count)
{
  intmask mask;

  mask = disable();

  struct	procent	*prptr = &proctab[currpid];		/* Ptr to process table entry	*/

  if(maxwait < 0) {
  	restore(mask);
  	return SYSERR;
  }

  if(semcount(prptr->recsem) >= msg_count) { // Mailbox contains at least msg_count messages
  	RcvMsg(msgs, msg_count);  // Receive all and return number of messages received
  	restore(mask);
  	return msg_count;
  }

  prptr->msgs_needed = (msg_count - semcount(prptr->recsem)); /* Number of additional msgs needed in mailbox to return */

  if (insertd(currpid,sleepq,maxwait) == SYSERR) { /* Insert waiting process onto sleep list*/
      restore(mask);
      return SYSERR;
    }
  prptr->prstate = PR_RECTIM; /* Update process state */
  resched(); /* Relinquish CPU time */

  int min;

  if(semcount(prptr->recsem) < msg_count) { /* Determine the min of msgs in mailbox, and msg_count */
    min = semcount(prptr->recsem);          /* Set min to determined value */
  } else {
    min = msg_count;
  }

  RcvMsg(msgs, min); /* Retrieve all available or all needed messages (whichever is less) */

  restore(mask);
  return min;
}
