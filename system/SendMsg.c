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
		status msg_status = insertm(pid, msgs + i); /* attempt to insert msg into mailbox */
  		if(msg_status == SYSERR) { /* if cannot, break out */
  			break;
		}
	sent_msgs++; /* successfully inserted into mailbox, increment sent_msgs */
  }

  restore(mask);
  return sent_msgs;
}
