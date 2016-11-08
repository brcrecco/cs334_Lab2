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

	if(semcount(sendsem) > 0) {
		wait(sendsem);
		mailbox[tail] = msg;
		tail = (tail +1) % NMSG;
		signal(recsem);
	} else {
		return SYSERR;
	}
	return OK;
}