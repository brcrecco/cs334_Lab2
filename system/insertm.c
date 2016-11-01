/* insertm.c - insert a given message into a process' mailbox */

#include<xinu.h>

status insertm(
	pid32 pid,
	umsg32 msg
	)
{
	umsg32[]	mailbox = proctab[pid].mailbox;
	int16 	 	head = proctab[pid].mboxhead;
	int16 		tail = proctab[pid].mboxtail;

	if(mailbox[tail] != NULL ) {
		return SYSERR;
	}

	mailbox[tail] = msg;
	tail = (tail +1) % NMSG;

	return OK;
}