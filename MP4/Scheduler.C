

#include "thread.H"
#include "Scheduler.H"
#include "console.H"
#include "utils.H" 
#include "simple_timer.H"
#include "machine.H"

extern int RRschedule;

Scheduler::Scheduler(){
	q = readyQ();
}

void Scheduler::yield(){

	Thread* thr = q.dequeue ();	
	Thread::dispatch_to(thr);	
}


void Scheduler::resume(Thread* _thread){

	q.enqueue(_thread);
}

void Scheduler::add(Thread* _thread){

	q.enqueue(_thread);
}

void Scheduler::terminate(Thread* _thread){
	q.remove (_thread);
}

void Scheduler::handle_RRinterrupt (REGS *_r){

  	RRschedule = 1;
	resume (Thread::CurrentThread());
	yield ();
}

void readyQ::enqueue(Thread* thrd){
	
	if (head == tail)
		
		queue[head] = thrd;
	else
		queue[tail] = thrd;
	++tail;
}


Thread* readyQ::dequeue (){
	Thread *thr;
	int i;

	if (head == tail){return NULL;}
	else{

	thr = queue[head];
	for (i = head; i < tail; i++)
		queue[i-1] = queue[i];

	queue[tail-1] = 0;
	--tail;
	return thr; 
	}
	
}

#if 1

void readyQ::remove(Thread *thrd)
{
	int i;

	for (i = head; i < tail; i++)
	if ((queue[i]->Thread::ThreadId()) == thrd->Thread::ThreadId())

		break;
	else	

		return;
	
	queue[i] = 0;

	for (int j = i+1; j < tail; j++)
		queue[j-1] = queue[j];

	queue[tail-1] = 0;
	--tail;
}
	#endif
