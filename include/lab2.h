/* Function Prototypes for the new system calls */

extern uint32 SendMsg(pid32 pid, umsg32* msgs, uint32 msg_count);
extern syscall RcvMsg(umsg32* msgs, uint32 msg_count);
extern uint32 RcvMsgWait(int32 maxwait, umsg32* msgs, uint32 msg_count);
