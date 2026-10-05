#include "../include/pcb.h"

// 1. Add 'int prio' here (Remember: no '= 0' here, that stays in the header!)
Process::Process(int id, int arrival, int burst, int prio) {
     pid = id;
     arrival_time = arrival;
     burst_time = burst;
     priority = prio;  // <--- Save the new priority!

     reset();  // Call our own reset function to clean up the rest of the chart automatically
}

void Process::reset() {
    remaining_time = burst_time;  // At the very beginning, remaining time is the full burst time
    completion_time = 0;
    turnaround_time = 0;
    waiting_time = 0;
    started = false;
}