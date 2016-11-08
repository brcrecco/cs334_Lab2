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
	uint32		msg_needed = msg_count;

	if(msg_count < 0) {
		restore(mask);
		return SYSERR;
	}

	while(semcount(recsem) < msg_needed) {
		wait(recsem);
		msg_needed--;
	}

	umsg32 received_msgs = mailbox[msg_count];

	for(int i = 0; i < msg_count; i++) {
		wait(recsem);
		//received_msgs[i] = mailbox[head];
		head = (head + 1) % NMSG;	
		signal(sendsem);
	}

	msgs = &received_msgs;
	restore(mask);
	return OK;
}
