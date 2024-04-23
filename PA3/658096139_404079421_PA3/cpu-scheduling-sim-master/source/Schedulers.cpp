#include "../headers/Schedulers.h"
#include <climits>
#include <cmath>
#include <iostream>
#include <fstream>
Scheduler::Scheduler() {
    blocked_queue = new DList<PCB>();
    next_pcb_index = -1;
    ready_queue = NULL;
    output_file.open("process_lifecycle.txt", std::ios::out);
}

// This is the constructor for the non-RR algorithm below,

Scheduler::Scheduler(DList<PCB> *rq, CPU *cp, int alg){
    ready_queue = rq;
    cpu = cp;
    blocked_queue = new DList<PCB>();
    dispatcher = NULL;
    next_pcb_index = -1;
    algorithm = alg;
    output_file.open("process_lifecycle.txt", std::ios::out);
}

// This is the constructor for the RR algorithm below,

Scheduler::Scheduler(DList<PCB> *rq, CPU *cp, int alg, int tq){
    ready_queue = rq;
    cpu = cp;
    timeq = timer = tq;
    dispatcher = NULL;
    next_pcb_index = -1;
    algorithm = alg;
    blocked_queue = new DList<PCB>();
    output_file.open("process_lifecycle.txt", std::ios::out);
    
}

// Here dispatcher is set after construction since they mutually include each other (can be set here only once)


void Scheduler::setdispatcher(Dispatcher *disp) {
    if(dispatcher == NULL) dispatcher = disp;
}

// This is used by the dispatcher to check which process in the queue to grab and which not.

int Scheduler::getnext() {
    return next_pcb_index;
}

// The code below acts as a switch for the algorithms
void Scheduler::execute() {
    if (timer > 0) timer -= 0.5;
    if (ready_queue->size()) {
       
                // Continuing the scheduling of the process
                
                switch (algorithm) {
                    case 0:
                        fcfs();
                        break;
                    case 1:
                        srtf();
                        break;
                    case 2:
                        rr();
                        break;
                    case 3:
                        pp();
                        break;
                    case 4:
                        pr();
                        break;
                    default:
                        break;
                }
       }
 }

// Here in the code, it waits for cpu to go idle and then notifies the dispatcher to load next in the queue.

void Scheduler::fcfs() {
    next_pcb_index = 0;
    if (!cpu->isidle()) {
            moveFromRunningToReady(cpu->getpcb()->pid);
    }
    if(cpu->isidle()){
    moveFromReadyToRunning(next_pcb_index);
    dispatcher->interrupt();
    }
}

// SRTF algorithm : shortest remaining time first below,

void Scheduler::srtf() {
    float short_time;
    int short_index = -1;

    // This code shows if cpu is idle, it initializes shortest time to head of queue.
    
    if(!cpu->isidle()) short_time = cpu->getpcb()->time_left;
    else {
        short_time = ready_queue->gethead()->time_left;
        short_index = 0;
        
    }
    if (!cpu->isidle()) {
            moveFromRunningToReady(cpu->getpcb()->pid);
        }
        
        
    //now we search through the queue for actual shortest time in code below:
    
    for(int index = 0; index < ready_queue->size(); ++index){
        if(ready_queue->getindex(index)->time_left < short_time){ //Here less than ensures FCFS is used for tie.
            short_index = index;
            short_time = ready_queue->getindex(index)->time_left;
        }
    }

    // Here in the code, -1 means nothing to schedule, and this only happens if cpu is already working on shortest time.
    if(short_index >= 0) {
        next_pcb_index = short_index;
        moveFromReadyToRunning(next_pcb_index);
        dispatcher->interrupt();
    }
}

//round robin algorithm below, it simply uses timer and interrupts dispatcher when timer is up and schedules next in the queue.

void Scheduler::rr() {
    if(cpu->isidle() || timer <= 0){
     if (!cpu->isidle()) {
            moveFromRunningToReady(cpu->getpcb()->pid);
        }
        timer = timeq;
        next_pcb_index = 0;
        moveFromReadyToRunning(next_pcb_index);
        dispatcher->interrupt();
    }
}
// Preemptive Priority
void Scheduler::pp() {
    int low_prio;
    int low_index = -1;

    if (!cpu->isidle()) {
        low_prio = cpu->getpcb()->priority;
    } else {
        low_prio = ready_queue->gethead()->priority;
        low_index = 0;
    }
    if (!cpu->isidle()) {
            moveFromRunningToReady(cpu->getpcb()->pid);
    }
    for (int index = 0; index < ready_queue->size(); index++) {
        PCB* process = ready_queue->getindex(index);
        int temp_prio = process->priority;
        int time_left = process->time_left;

        if (process->io_burst_remaining > 0 && process->first_time_appear) {
            int blocking_threshold = timeq / 2;

            if (process->time_left >= blocking_threshold) {
                moveFromReadyToBlocked(process->pid);
                blocked_queue->add_end(*process);
                ready_queue->removeindex(index);
                index--;
                continue;
            }
        }

        if (temp_prio < low_prio || (temp_prio == low_prio && timer <= 0)) {
            low_prio = temp_prio;
            low_index = index;
            timer=timeq;
        }
    }

    if (low_index >= 0) {
        next_pcb_index = low_index;
        moveFromReadyToRunning(next_pcb_index);
        dispatcher->interrupt();
        timer=timeq;
    }


    for (int i = 0; i < blocked_queue->size(); i++) {
        PCB* process = blocked_queue->getindex(i);
        process->io_burst_remaining=-0.5;
        if (process->io_burst_remaining <= 0) {
            moveFromBlockedToReady(process->pid);
            ready_queue->add_end(*process);
            blocked_queue->removeindex(i);
        }else{
            i--;
        }
    }
}

void Scheduler::pr() {
        if (cpu->isidle() || timer <= 0) {
            timer = timeq;
            if (!cpu->isidle()) {
            moveFromRunningToReady(cpu->getpcb()->pid);
        }
            if (ready_queue->size() > 0) {
                int random_index = rand() % ready_queue->size();
                next_pcb_index = random_index;
                moveFromReadyToRunning(next_pcb_index);
                dispatcher->interrupt();
            }
        }
    }
void Scheduler::logProcessLifecycle(const std::string& message) {
    if (output_file.is_open()) {
        output_file << message << std::endl;
    }
}

void Scheduler::moveFromRunningToReady(int pid) {
    logProcessLifecycle("Process " + std::to_string(pid) + " moved from Running to Ready queue.");
}

void Scheduler::moveFromBlockedToReady(int pid) {
    logProcessLifecycle("Process " + std::to_string(pid) + " moved from Blocked to Ready queue.");
}

void Scheduler::moveFromReadyToRunning(int pid) {
    logProcessLifecycle("Process " + std::to_string(pid) + " moved from Ready to Running queue.");
}
void Scheduler::moveFromReadyToBlocked(int pid) {
    logProcessLifecycle("Process " + std::to_string(pid) + " moved from Ready to Blocked queue.");
}

/*
 *
 * Dispatcher Implementation
 *
 */
Dispatcher::Dispatcher(){
    cpu = NULL;
    scheduler = NULL;
    ready_queue = NULL;
    clock = NULL;
    _interrupt = false;

}

Dispatcher::Dispatcher(CPU *cp, Scheduler *sch, DList<PCB> *rq, Clock *cl) {
    cpu = cp;
    scheduler = sch;
    ready_queue = rq;
    clock = cl;
    _interrupt = false;

};

//function to handle switching out pcbs and storing back into ready queue
PCB* Dispatcher::switchcontext(int index) {
    PCB* old_pcb = cpu->pcb;
    PCB* new_pcb = new PCB(ready_queue->removeindex(scheduler->getnext()));
    cpu->pcb = new_pcb;
    return old_pcb;
}

//executed every clock cycle, only if scheduler interrupts it
void Dispatcher::execute() {
    if(_interrupt) {
        PCB* old_pcb = switchcontext(scheduler->getnext());
        if(old_pcb != NULL){ //only consider it a switch if cpu was still working on process
            old_pcb->num_context++;
            cpu->getpcb()->wait_time += .5;
            clock->step();
            ready_queue->add_end(*old_pcb);
            delete old_pcb;
        }
        _interrupt = false;
    }
}

//routine for scheudler to interrupt it
void Dispatcher::interrupt() {
    _interrupt = true;
}
