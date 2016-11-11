/* insertm.c - insert a given message into a process' mailbox */

#include<xinu.h>

status insertm(
	pid32 pid,
	umsg32 msg
	)
{

	struct	procent	*prptr = &proctab[pid];		// Ptr to process table entry	

	if(semcount(prptr->sendsem) > 0) {		/* if there is an empty slot in mailbox to send a msg to */
		wait(prptr->sendsem); 	/* decrement number of available spaces */
		prptr->mailbox[prptr->mboxtail] = msg;	/* place message at tail */
		prptr->mboxtail = (prptr->mboxtail + 1) % NMSG; 	/* update the tail */
		signal(prptr->recsem);	/* signal recsem to indicate new receivable message */
	} else {
		return SYSERR; /* no empty spaces, return SYSERR */
	}
	return OK;
}