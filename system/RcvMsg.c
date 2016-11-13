#include <xinu.h>

syscall RcvMsg(umsg32* msgs, uint32 msg_count)
{
	intmask mask;

	mask = disable();
  
	struct	procent	*prptr = &proctab[currpid];		/* Ptr to process table entry	*/

	//kprintf("+%u+\n", sizeof(msgs)/sizeof(msgs[0]));

	/*if((sizeof(msgs)/sizeof(umsg32)) < msg_count) {
		restore(mask);
		return SYSERR;
	}*/

	for(int i = 0; i < msg_count; i++ ) { 
		wait(prptr->recsem); /* wait on msgs to be received */
	}

	for(int i = 0; i < msg_count; i++) { 	/* enough messages available to be received */
		msgs[i] = prptr->mailbox[prptr->mboxhead];	/* store received messages in msgs */
		prptr->mboxhead = (prptr->mboxhead + 1) % NMSG;	/* update the head */
		signal(prptr->sendsem); 	/* signal new opening in mailbox */
	}

	restore(mask);
	return OK;
}
