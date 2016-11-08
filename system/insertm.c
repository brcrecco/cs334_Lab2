/* insertm.c - insert a given message into a process' mailbox */

#include<xinu.h>

status insertm(
	pid32 pid,
	umsg32 msg
	)
{

	struct	procent	*prptr = &proctab[pid];		// Ptr to process table entry	

	umsg32		*mailbox = prptr->mailbox;
	sid32		sendsem  = prptr->sendsem;
	sid32		recsem	= prptr->recsem;
	int16 		tail = prptr->mboxtail;

	if(semcount(sendsem) > 0) {		/* if there is an empty slot in mailbox to send a msg to */
		wait(sendsem); 	/* decrement number of available spaces */
		mailbox[tail] = msg;	/* place message at tail */
		tail = (tail +1) % NMSG; 	/* update the tail */
		signal(recsem);	/* signal recsem to indicate new receivable message */
	} else {
		return SYSERR; /* no empty spaces, return SYSERR */
	}
	return OK;
}