//SJF with preemtion work according to burst time , but mainly checks for remaining brst time of the process.
// If a process with smaller remaining time arrives, it preempts the current process and executes the new one.
#include "../include/sjf_p.h"
#include <climits> 

void SJF_P::schedule(std::vector<Process>& processes){
 
    int current_time = 0;
    int completed = 0;
    int n = processes.size();
    
    // --- The Manager's Gantt Chart Memory ---
    int last_pid = -1; 
    int block_start_time = 0; 
    // ----------------------------------------

    while (completed < n) {
        int idx = -1;
        int min_remaining = INT_MAX;

        // 1. Scan the lobby for the smallest remaining_time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0 && processes[i].remaining_time < min_remaining) {
                min_remaining = processes[i].remaining_time;
                idx = i;
            }
        }
        
        // We will write the Execution logic here next!
        if(idx == -1){
            // NEW: If the CPU just became idle, save the last process's streak immediately!
            if (last_pid != -1) {
                add_gantt_entry(last_pid, block_start_time, current_time);
                last_pid = -1; // Reset memory to "Idle"
            }
            
            current_time += 1;
            continue;
        }
        else {
            // 1. Context Switch Check
            if (last_pid != processes[idx].pid) {
                // If someone was running before, save their streak to the notepad
                if (last_pid != -1) {
                    add_gantt_entry(last_pid, block_start_time, current_time);
                }
                // Update memory for the new process
                block_start_time = current_time;
                last_pid = processes[idx].pid;
            }

            // 2. The 1-Second Drip
            processes[idx].remaining_time -= 1;
            current_time += 1;

            // 3. Did it finish?
            if (processes[idx].remaining_time == 0) {
                completed++;
                processes[idx].completion_time = current_time;
                processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
                processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            }
        }
    }

    // Save the very last process's streak to the notepad
    if (last_pid != -1) {
        add_gantt_entry(last_pid, block_start_time, current_time);
    }
}
