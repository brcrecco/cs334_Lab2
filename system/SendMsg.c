#include<xinu.h>

status insertm();

uint32 SendMsg(pid32 pid, umsg32* msgs, uint32 msg_count)
{
	intmask mask;

	mask = disable();

	if(msg_count < 0 || isbadpid(pid)) {
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

  if(proctab[pid].prstate == PR_RECTIME) { //if pid is waiting on msgs
  	unsleep(pid);	// awaken sleeping process
	ready(pid);		// make it ready
  }

  restore(mask);
  return sent_msgs;
}
