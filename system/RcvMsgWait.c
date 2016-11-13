#include <xinu.h>

uint32 RcvMsgWait(int32 maxwait, umsg32* msgs, uint32 msg_count)
{
  intmask mask;

  mask = disable();

  struct	procent	*prptr = &proctab[currpid];		/* Ptr to process table entry	*/

  if(msg_count < 0) { 
	restore(mask);
	return SYSERR;
  }

  /*if((sizeof(msgs)/sizeof(umsg32)) < msg_count) {
	  restore(mask);
	  return SYSERR;
    }*/

  if(maxwait < 0) {
  	restore(mask);
  	return SYSERR;
  }

  int rec_msgs = 0;

  if(semcount(prptr->recsem) >= msg_count) { // Case 1: mailbox contains at least msg_count messages
  	rec_msgs = RcvMsg(msgs, msg_count);  // Receive all and return number of messages received
  	restore(mask);
  	return rec_msgs;
  }

  int msgs_in_mailbox = semcount(prptr->recsem);
  
  for(int i = 0; i < msgs_in_mailbox; i++) {
  	wait(prptr->recsem);
  	msgs[i] = prptr->mailbox[prptr->mboxhead];	/* store received messages in msgs */
	  prptr->mboxhead = (prptr->mboxhead + 1) % NMSG;	/* update the head */
	  signal(prptr->sendsem); 	/* signal new opening in mailbox */
  }

  for(int i = msgs_in_mailbox; i < msg_count; i++) {
  	if (insertd(currpid,sleepq,maxwait) == SYSERR) { /* insert current process into sleep list*/
			restore(mask); /* if there is an error restore mask and return SYSERR */
			return SYSERR;
		}
		prptr->prstate = PR_RECTIM;
		resched();
    if(prptr->prstate == PR_RECVMSG){ /* a msg is waiting to be received */
		  wait(prptr->recsem);  
  	  msgs[i] = prptr->mailbox[prptr->mboxhead];	/* store received messages in msgs */
		  prptr->mboxhead = (prptr->mboxhead + 1) % NMSG;	/* update the head */
		  signal(prptr->sendsem); 	/* signal new opening in mailbox */
      rec_msgs++;   /* indicate an increase in the number of messages received */
      /* TODO: change maxwait value to indicate remaining time to wait */
    } else {
      break;  /* Time expired, break out of loop*/
    }
  }

  return msgs;
}
