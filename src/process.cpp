#include "process.h"

Process::Process(int pid_, int arrival_, int burst_, int priority_, int memory_)
    : pid(pid_),
      arrival_time(arrival_),
      burst_time(burst_),
      priority(priority_),
      memory_size(memory_) {
    reset();
}

void Process::reset() {
    remaining_time = burst_time;
    completion_time = 0;
    turnaround_time = 0;
    waiting_time = 0;
    response_time = -1;
    started = false;
}
