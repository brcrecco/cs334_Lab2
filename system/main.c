/*  main.c  - main */

#include <xinu.h>
#include "lab2.h"
/*DO NOT DEFINE DEPENDENT VARIABLES HERE*/
int task2consumed = 0; // task 2 consumed message count
int task2produced = 0; // task 2 produced message count

int task3consumed1 = 0; // task 3 consumed messages
int task3consumed2 = 0;
int task3consumed3 = 0;

int task3produced1 = 0; // task 3 produced messages
int task3produced2 = 0;
int task3produced3 = 0;

int allocated1 = 0; // task 3 allocated messages
int allocated2 = 0;
int allocated3 = 0;


process Task2Test(), Task2Producer(), Task2Consumer(), Task3Producer(), Task3Consumer(), Task3Test(),
		Task3Allocator(), EqualArrayTest(), UnequalArrayTest(), UnequalArrayTestConsumer(), UnequalArrayTestProducer(),
		SyserrTestSend(), RcvMsgWaitTest(), RcvMsgWaitLongTest(), RcvMsgWaitLongTestConsumer(), 
		RcvMsgWaitLongTestProducer(), RcvMsgWaitIncompleteTest(), RcvMsgWaitDelayTest(), 
		RcvMsgWaitDelayProducer(), RcvMsgWaitDelayConsumer();

process	main(void)
{	
	resume(create(RcvMsgWaitDelayTest, 8192, 50, "RcvMsgWaitDelayTest", 0));
	return OK;
}

process Task2Test(void) { /* Create and run task2consumer and task2prodcuer, with appropriate input */

	pid32 consumer = create(Task2Consumer, 8192, 50, "Task2Consumer", 0);
	pid32 producer = create(Task2Producer, 8192, 50, "Task2Producer", 1, consumer);
	resume(producer);
	resume(consumer);
	
	while(1) { /* Print out number of sent and received messages of producer and consumer, respectively */
		kprintf("Producer created: %d items and Consumer consumed: %d items\n", task2produced, task2consumed);
	}

	return OK;
}


process Task2Producer(pid32 consumer) { /* Send one message at a time to consumer */
	umsg32 msg = 42;
	while(1) {
		uint32 sent_msgs = SendMsg(consumer, &msg, 1);
		if(sent_msgs != SYSERR) {
			task2produced += sent_msgs;
		}	
		sleepms(1); /* After message sent, relinquish CPU */
	}

	return OK;
}


process Task2Consumer(void) { /* Receive one message from its own mailbox at a time */
	int32 numMsgs = 1;
	umsg32 msg[numMsgs];
	while(1) {
		syscall rcv = RcvMsg(msg, numMsgs);
		if(rcv == OK) {
			task2consumed += numMsgs;
		}
		sleepms(1); /* After message received, relinquish CPU */
	}

	return OK;
}

process Task3Test(void) { /* Create and resume 3 producers, 1 allocator, and 3 consumers */

	umsg32 msg1[1]; /* Message array to be sent by producer1 */
	umsg32 msg2[1]; /* Message array to be sent by producer2 */
	umsg32 msg3[1]; /* Message array to be sent by producer3 */

	msg1[0] = 1; /* Producer1 sends message of 1 */
	msg2[0] = 2; /* Producer2 sends message of 2 */
	msg3[0] = 3; /* Producer3 sends message of 3 */

	pid32 consumer1 = create(Task3Consumer, 8192, 20, "Task3Consumer1", 0);
	pid32 consumer2 = create(Task3Consumer, 8192, 20, "Task3Consumer2", 0);
	pid32 consumer3 = create(Task3Consumer, 8192, 20, "Task3Consumer3", 0);
	pid32 allocator = create(Task3Allocator, 8192, 40, "Task3Allocator", 3, consumer1, consumer2, consumer3);
	pid32 producer1 = create(Task3Producer, 8192, 20, "ONE", 2, allocator, msg1);
	pid32 producer2 = create(Task3Producer, 8192, 20, "TWO", 2, allocator, msg2);
	pid32 producer3 = create(Task3Producer, 8192, 20, "THREE", 2, allocator, msg3);

	pri16 p1 = resume(producer1);
	pri16 p2 = resume(producer2);
	pri16 p3 = resume(producer3);
	pri16 a = resume(allocator);
	pri16 c1 = resume(consumer1);
	pri16 c2 = resume(consumer2);
	pri16 c3 = resume(consumer3);

	
	while(1) { /* Print number of messsages sent by each producer, recieved by allocator, and received by consumers */
		kprintf("Producer 1 created: %d items and Consumer 1 consumed: %d items\n", task3produced1, task3consumed1);
		kprintf("Producer 2 created: %d items and Consumer 2 consumed: %d items\n", task3produced2, task3consumed2);
		kprintf("Producer 3 created: %d items and Consumer 3 consumed: %d items\n", task3produced3, task3consumed3);
		kprintf("Allocator node has: %d items from 1 %d items from 2 %d items from 3\n\n", allocated1, allocated2, allocated3);

		sleepms(1); /* Relinqiush CPU */
	}
	
	return OK;

}

process Task3Producer(pid32 allocator, umsg32* msg) {

	while(1) {
		uint32 sent_msgs = SendMsg(allocator, msg, 1); /* Sends message */
		if(sent_msgs != SYSERR) {
			switch(*msg) { /* Increase counter for message sent by appropriate producer */
				case 1: task3produced1 += sent_msgs;
						break;
				case 2: task3produced2 += sent_msgs;
						break;
				case 3: task3produced3 += sent_msgs;
						break;
				default:
						break;
			}
		}
		sleepms(1); /* Relinquish CPU */
	}

	return OK;
}

process Task3Consumer(void) {

	umsg32 msg[1];

	while(1) {
		syscall rcv = RcvMsg(msg, 1); /* Receive 1 message from mailbox */

		if(rcv == OK) {
			switch(msg[0]) {	/* Increase counter of message received for appropriate consumer */
				case 1: task3consumed1++;
						break;
				case 2: task3consumed2++;
						break;
				case 3: task3consumed3++;
						break;
				default: 
						break;
			}
		}
		sleepms(1); /* Relinquish CPU */
	}

	return OK;
}

process Task3Allocator(pid32 consumer1, pid32 consumer2, pid32 consumer3) {

	umsg32 msg[5];	// storage for receiving 5 messages at a time

	while(1) {
		syscall rect = RcvMsg(msg, 5); /* Receive 5 messages at a time from mailbox */
		for(int i = 0; i < 5; i++){

			switch(msg[i]) { /* Increase counter for message received from appropriate producer */
				case 1: allocated1++;
						break;
				case 2: allocated2++;
						break;
				case 3: allocated3++;
						break;
				default: 
						break;
			}
		}

		for(int i = 0; i < 5; i++) {
			switch(msg[i]){ /* Send message to appropriate consumer */
				case 1: SendMsg(consumer1, &msg[i], 1);
						break;

				case 2: SendMsg(consumer2, &msg[i], 1);
						break;

				case 3: SendMsg(consumer3, &msg[i], 1);
						break;

				default: 
						break;


			}
		}
		sleepms(10); /* Relinquish CPU */
	}
	return OK;
}

process EqualArrayTest(void) {
	int n = 30;	// number of messages sent
	umsg32 msg[n]; 	// sent messages
	umsg32 rec_msg[n]; //received messages

	while(1){	// do forever
		for(int i = 0; i < n; i++) {	// store messages in array
			msg[i] = 40 + i;
		}

		SendMsg(currpid, msg, n);	// send the messages

		RcvMsg(rec_msg, n);		// receive the messages

		for(int i = 0; i< n; i++) {	// check to see if the received messages are the sent messages
			if(rec_msg[i] != msg[i]) {
				kprintf("Fail\n");	// if not, print fail and return syserr
				return SYSERR;
			}
		}	// otherwise, it passes
		kprintf("Pass\n");
	}
	return OK;
}

process UnequalArrayTest(void){ /* Create and resume producer and consumer */
	pid32 consumer = create(UnequalArrayTestConsumer, 8192, 20, "UnequalArraytestConsumer", 1, 9);
	resume(consumer);
	resume(create(UnequalArrayTestProducer, 8192, 20, "UnequalArrayTestProducer", 2, 30, consumer));
}

int iMsg; /* Local variable for message to be sent */
process UnequalArrayTestProducer(int n, pid32 pid) {

	umsg32 msg[n]; // array holding msgs to send
	iMsg = 0;

	while(1) {

		for(int i = 0; i < n; i++) {	// insert the messages into the array
			msg[i] = iMsg;	// set the message
			iMsg++;		// increment message value by 1
		}
		SendMsg(pid, msg, n);	// send the message
	}

	return OK;
}

process UnequalArrayTestConsumer(int m) {

	umsg32 rec_msg[m]; // array holding msgs to be received


	while(1) {
		RcvMsg(rec_msg, m);	// receive messages

		for(int i = 0; i < (m-1); i++) {	// check to see the successive messages are one more than the previous message (as they should be)
			if(rec_msg[i] != (rec_msg[i + 1] - 1)) {
				kprintf("Fail\n");	// if not, print fail and return syserr
				return SYSERR;
			}
		}
		kprintf("Pass\n");	// otherwise, it passes
	}
	return OK;
}

process SyserrTestSend(void){ /* Ensure SendMsg can catch error */
	int n = 20;
	umsg32 msgs[n];

	while(1){
		uint32 stat = SendMsg(NPROC+1, msgs, n); /* Call SendMsg with badpid */

		if(stat == (uint32)SYSERR){ /* If it returns SYSERR, pass */
			kprintf("PASS\n");
		}
		else{
			kprintf("FAIL\n"); /* Else, fail */
			return SYSERR;
		}
	}

}

process RcvMsgWaitTest(void) { /* Ensure RcvMsgWait returns after time expired */
	int32 maxwait = 10;
	int n = 5;
	umsg32 msgs[n];
													/* == NEVER SEND MESSAGES */
	uint32 rec_msgs = RcvMsgWait(maxwait, msgs, n); /* Wait on messages to be sent*/

	if(rec_msgs == 0) { /* If no messages received, pass */
		kprintf("Pass\n");
	} else {
		kprintf("Fail\n"); /* Else, fail */
	}

	return OK;
}

process RcvMsgWaitLongTest() { /* Create and resume producer and consumer */ 
	pid32 consumer = create(RcvMsgWaitLongTestConsumer, 8192, 50, "RcvMsgWaitConsumer", 1, 5);
	resume(consumer);
	resume(create(RcvMsgWaitLongTestProducer, 8192, 60, "RcvMsgWaitProducer", 2, consumer, 5));
}

process RcvMsgWaitLongTestConsumer(int n) { /* Ensures RcvMsgWait will recieve n messages */
	int32 maxwait = 1000000000; /* Ridiculously large wait time */
	umsg32 msgs[n];

	int rec_msg;
	rec_msg = RcvMsgWait(maxwait, msgs, n); /* Wait on messages */

	if(rec_msg == n){ /* If all n msgs received, pass */
		kprintf("Pass\n"); 
	} else { 		  /* Else, fail */
		kprintf("Fail\n");
	}

	for(int i = 0; i < rec_msg; i++) { /* For manual check, ensure appropriate messages sent */
		kprintf("%u, ", msgs[i]);
	}

	return OK;
}

process RcvMsgWaitLongTestProducer(pid32 pid, int n) {
	umsg32 msgs[n];

	for(int i = 0; i < n; i++) { /* Produce messages to be sent */
		msgs[i] = i;
	}

	SendMsg(pid, msgs, n); /* Send messages */ 

	return OK;
}

process RcvMsgWaitIncompleteTest(void) { /* Test an appropraite return value for insufficent messages received*/
	int n = 3; /* Number of messages to be sent*/
	int m = 5; /* Number of messages desired */
	int32 maxwait = 10; /* Short maxwait */
	
	umsg32 snd_msgs[n]; /* Array of messages to be sent */
	umsg32 rcv_msgs[m]; /* Array of messages to be received */

	for(int i = 0; i < n; i++) {
		snd_msgs[i] = i; /* Place messages in array */
	}

	SendMsg(currpid, snd_msgs, n); /* Send messages to self */

	uint32 received = RcvMsgWait(maxwait, rcv_msgs, m); /* Receive all messages in mailbox */

	if(received == n) { /* If, return value equal to n*/
		kprintf("Pass\n"); /* PASS */
	} else {
		kprintf("Fail\n"); /* Else, FAIL */
	}
	return OK;
}

process RcvMsgWaitDelayConsumer(void) {
	int n = 5; /* Number of messages desired */
	int m = 2; /* Number of messages to send to self */
	umsg32 rec_msgs[n]; /* Array of messages to receive */
	umsg32 snd_msgs[m]; /* Array of messsages to send to self */

	for(int i = 0; i < m; i++) {
		snd_msgs[i] = i; /* Insert messages into array */
	}

	SendMsg(currpid, snd_msgs, m); /* Send messages to self */
	resume(create(RcvMsgWaitDelayProducer, 8192, 20, "RcvMsgWaitDelayProducer", 2, currpid, n)); /* Create producer */
	uint32 received = RcvMsgWait(1000000, rec_msgs, n);	/* Wait on n messages */ 				/* with a smaller priority */

	if(received == n) { /* If all messages received */
		kprintf("Pass\n"); /* PASS */ 
	} else {
		kprintf("Fail\n"); /* Else, FAIL */
	}
	return OK;
}


process RcvMsgWaitDelayProducer(pid32 pid, int32 n) {
	umsg32 snd_msgs[n]; /* Array of messages to be sent */
	
	for(int i = 0; i < n; i++) {
		snd_msgs[i] = i; /* Insert messages into array */
	}

	SendMsg(pid, snd_msgs, n); /* Send messages */

	return OK;
}

process RcvMsgWaitDelayTest(void) { /* Test for if RcvMsgWait will wait if insufficient number of msgs currently in mailbox*/
	resume(create(RcvMsgWaitDelayConsumer, 8192, 50, "RcvMsgWaitDelayConsumer", 0)); /* Only create consumer */
	return OK;
}