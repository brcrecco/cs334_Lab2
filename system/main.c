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
		Task3Allocator();

process	main(void)
{
	resume(create(Task3Test, 8192, 50, "Task3Test", 0));
	return OK;
}

process Task2Test(void) {

	pid32 consumer = create(Task2Consumer, 8192, 50, "Task2Consumer", 0);
	pid32 producer = create(Task2Producer, 8192, 50, "Task2Producer", 1, consumer);
	resume(producer);
	resume(consumer);
	
	while(1) {
		kprintf("Producer created: %d items and Consumer consumed: %d items\n", task2produced, task2consumed);
	}

	return OK;
}


process Task2Producer(pid32 consumer) {
	umsg32 msg = 42;
	while(1) {
		uint32 sent_msgs = SendMsg(consumer, &msg, 1);
		if(sent_msgs != SYSERR) {
			task2produced += sent_msgs;
		}	
		sleepms(1);
	}

	return OK;
}


process Task2Consumer(void) {
	int32 numMsgs = 1;
	umsg32 msg[numMsgs];
	while(1) {
		syscall rcv = RcvMsg(msg, numMsgs);
		if(rcv == OK) {
			task2consumed += numMsgs;
		}
		sleepms(1);
	}

	return OK;
}

process Task3Test(void) {
/*
	umsg32 one = 1;
	umsg32 two = 2;
	umsg32 three = 3;
*/
	umsg32 msg1[1];
	umsg32 msg2[1];
	umsg32 msg3[1];

	msg1[0] = 1;
	msg2[0] = 2;
	msg3[0] = 3;

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

	
	while(1) {
		kprintf("Producer 1 created: %d items and Consumer 1 consumed: %d items\n", task3produced1, task3consumed1);
		kprintf("Producer 2 created: %d items and Consumer 2 consumed: %d items\n", task3produced2, task3consumed2);
		kprintf("Producer 3 created: %d items and Consumer 3 consumed: %d items\n", task3produced3, task3consumed3);
		kprintf("Allocator node has: %d items from 1 %d items from 2 %d items from 3\n", allocated1, allocated2, allocated3);

		sleepms(1);
	}
	
	return OK;

}

process Task3Producer(pid32 allocator, umsg32* msg) {

	while(1) {
		uint32 sent_msgs = SendMsg(allocator, msg, 1);
		if(sent_msgs != SYSERR) {
			switch(*msg) {
				case 1: task3produced1 += sent_msgs;
						break;
				case 2: task3produced2 += sent_msgs;
						break;
				case 3: task3produced3 += sent_msgs;
						break;
				default: //kprintf("not producing\n");
						break;
			}
		}
		sleepms(1);
	}

	return OK;
}

process Task3Consumer(void) {

	umsg32 msg[1];

	while(1) {
		syscall rcv = RcvMsg(msg, 1);
		//kprintf("[%d] \n", *msg);
		if(rcv == OK) {
			switch(msg[0]) {	// gets the first value
				case 1: task3consumed1++;
						break;
				case 2: task3consumed2++;
						break;
				case 3: task3consumed3++;
						break;
				default: //kprintf("not consuming\n"); 
						break;
			}
		}
		sleepms(1);
	}

	return OK;
}

process Task3Allocator(pid32 consumer1, pid32 consumer2, pid32 consumer3) {

	umsg32 msg[5];	// storage for receiving 5 messages at a time

	while(1) {
		syscall rect = RcvMsg(msg, 5);
		for(int i = 0; i < 5; i++){
			//kprintf("{%u}\n", msg[i]);
			switch(msg[i]) {
				case 1: allocated1++;
						break;
				case 2: allocated2++;
						break;
				case 3: allocated3++;
						break;
				default: //kprintf("REC\n"); 
						break;
			}
		}

		for(int i = 0; i < 5; i++) {
			switch(msg[i]){
				case 1: SendMsg(consumer1, &msg[i], 1);
						allocated1--;
						break;

				case 2: SendMsg(consumer2, &msg[i], 1);
						allocated2--;
						break;

				case 3: SendMsg(consumer3, &msg[i], 1);
						allocated3--;
						break;

				default: //kprintf("SND \n"); 
						break;


			}
		}
		sleepms(10);
	}
	return OK;
}