#include<xinu.h>

status insertm();

uint32 SendMsg(pid32 pid, umsg32* msgs, uint32 msg_count)
{
	intmask mask;

	mask = disable();

	if(isbadpid(pid)){
		restore(mask);
		return SYSERR;
	}

	uint32 sent_msgs = 0;	/* number of successfully sent messages */

	for(int i = 0; i < msg_count; i++) {	
		
  		if(insertm(pid, msgs[i]) == SYSERR) { /* attempt to insert msg into mailbox */
  			break; /* if it cannot, return syserr */
		}
		sent_msgs++; /* successfully inserted into mailbox, increment sent_msgs */
  	}

  if(proctab[pid].prstate == PR_RECTIM) { /* if pid waiting on msgs, update number of msgs needed */

  	proctab[pid].msgs_needed -= sent_msgs; /* decrement number of messages needed to return */
  	
  	if(proctab[pid].msgs_needed <= 0){  /* if number of msgs needed is met, unsleep and make ready */
  		unsleep(pid);	
		ready(pid);		
	}
  }
  
  restore(mask);
  return sent_msgs;
}
