#include<xinu.h>

uint32 SendMsg(pid32 pid, umsg32* msgs, uint32 msg_count)
{

  uint32 sent_msgs = 0;

  for(int i = 0; i < msg_count; i++) {
  	status msg_status = insertm(pid, msg + i);
  	if(status == SYSERR) {
  		break;
	}
	sent_msgs++;
  }
  return sent_msgs;
}
