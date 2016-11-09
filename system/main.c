/*  main.c  - main */

#include <xinu.h>
#include "lab2.h"
/*DO NOT DEFINE DEPENDENT VARIABLES HERE*/
int consumed = 0;
int produced = 0;

process Task2Test(), Task2Producer(), Task2Consumer();

process	main(void)
{
	resume(create(Task2Test, 8192, 50, "Task2Test", 0));
	return OK;
}

process Task2Test(void) {

	pid32 consumer = create(Task2Consumer, 8192, 50, "Task2Consumer", 0);
	pid32 producer = create(Task2Producer, 8192, 50, "Task2Producer", 1, consumer);
	resume(producer);
	resume(consumer);
	
	while(1) {
		//sleepms(1);
		kprintf("Producer created: %d items and Consumer consumed: %d items\n", produced, consumed);
	}

	return OK;
}


process Task2Producer(pid32 consumer) {
	umsg32* msg = 42;
	while(1) {
		uint32 sent_msgs = SendMsg(consumer, msg, 1);
		if(sent_msgs != SYSERR) {
			produced += sent_msgs;
		}	
		sleepms(1);
	}

	return OK;
}


process Task2Consumer(void) {
	int32 numMsgs = 1;
	umsg32* msg[numMsgs];
	while(1) {
		syscall rcv = RcvMsg(msg, numMsgs);
		if(rcv == OK) {
			consumed += numMsgs;
		}
		sleepms(1);
	}

	return OK;
}
