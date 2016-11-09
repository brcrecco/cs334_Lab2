#include <xinu.h>

syscall RcvMsg(umsg32* msgs, uint32 msg_count)
{
	intmask mask;

	mask = disable();
  
	struct	procent	*prptr = &proctab[currpid];		/* Ptr to process table entry	*/

	umsg32		*mailbox = prptr->mailbox;
	sid32		sendsem  = prptr->sendsem;
	sid32		recsem	= prptr->recsem;
	int16 		head = prptr->mboxhead;

	if(msg_count < 0) { 
		restore(mask);
		return SYSERR;
	}

	if(sizeof(*msgs) < msg_count) {
		restore(mask);
		return SYSERR;
	}

	for(int i = 0; i < msg_count; i++ ) { 
		wait(recsem); /* wait on msgs to be received */
	}

	for(int i = 0; i < msg_count; i++) { 	/* enough messages available to be received */

		msgs[i] = mailbox[head];	/* store received messages in msgs */
		head = (head + 1) % NMSG;	/* update the head */
		signal(sendsem); 	/* signal new opening in mailbox */
	}

	restore(mask);
	return OK;
}
