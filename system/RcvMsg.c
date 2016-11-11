#include <xinu.h>

syscall RcvMsg(umsg32* msgs, uint32 msg_count)
{
	intmask mask;

	mask = disable();
  
	struct	procent	*prptr = &proctab[currpid];		/* Ptr to process table entry	*/

/*	umsg32*     mailbox = prptr->mailbox;
	sid32		sendsem  = prptr->sendsem;
	sid32		recsem	= prptr->recsem;
	int16 		head = prptr->mboxhead;*/

	if(msg_count < 0) { 
		restore(mask);
		return SYSERR;
	}

	/*if(sizeof(msgs) < msg_count) {
		restore(mask);
		return SYSERR;
	}*/

	for(int i = 0; i < msg_count; i++ ) { 
		wait(prptr->recsem); /* wait on msgs to be received */
	}

	for(int i = 0; i < msg_count; i++) { 	/* enough messages available to be received */
		//kprintf("!%u!\n", prptr->mailbox[prptr->mboxhead]);
		msgs[i] = prptr->mailbox[prptr->mboxhead];	/* store received messages in msgs */
		prptr->mboxhead = (prptr->mboxhead + 1) % NMSG;	/* update the head */
		signal(prptr->sendsem); 	/* signal new opening in mailbox */
	}

	restore(mask);
	return OK;
}
