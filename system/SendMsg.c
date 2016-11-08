#include<xinu.h>

uint32 SendMsg(pid32 pid, umsg32* msgs, uint32 msg_count)
{
	intmask mask;

	mask = disable();

	if(msg_count < 0 || isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	uint32 sent_msgs = 0;

	for(int i = 0; i < msg_count; i++) {
		status msg_status = insertm(pid, msgs + i);
  		if(msg_status == SYSERR) {
  			break;
		}
	sent_msgs++;
  }

  restore(mask);
  return sent_msgs;
}
