#include "../include/fcfs.h"

void FCFS::schedule(std::vector<Process>& processes) {
    // This is our CPU's master clock. It starts at second 0.
    int current_time = 0; 

    // Look at each process one by one in the order they were handed to us
    for (auto& p : processes) {
        
        // Scenario: The CPU finished early, and the next process hasn't arrived yet.
        // We have to sit idle and fast-forward the clock to when it finally arrives.
        if (current_time < p.arrival_time) {
            current_time = p.arrival_time;
        }

        // 1. Record when the process actually started
        int start_time = current_time;
        
        // 2. Fill out the final metrics on the patient's chart (PCB)
        p.waiting_time = current_time - p.arrival_time;
        p.turnaround_time = p.waiting_time + p.burst_time;
        
        // 3. Fast-forward the CPU clock by the time it took to do the work
        current_time += p.burst_time;
        
        p.completion_time = current_time;

        // 4. Write this entire chunk of time onto the Manager's notepad
        add_gantt_entry(p.pid, start_time, current_time);
    }
}