#include "../include/mlfq.h"
#include <queue>

void MLFQ::schedule(std::vector<Process>& processes) {
    int current_time = 0;
    int completed = 0;
    int n = processes.size();

    // The Three Tiers of Queues
    std::queue<int> q1; // High Priority (RR, TQ = 2)
    std::queue<int> q2; // Medium Priority (RR, TQ = 4)
    std::queue<int> q3; // Low Priority (FCFS)

    int tq1 = 2;
    int tq2 = 4;
    int current_tq_used = 0;

    int active_idx = -1;
    int active_queue = 0;

    // --- The Manager's Gantt Chart Memory ---
    int last_pid = -1;
    int block_start_time = 0;

    // Track who has already arrived in the system
    std::vector<bool> in_system(n, false);

    while (completed < n) {
        // 1. Check the lobby for new arrivals
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && !in_system[i] && processes[i].remaining_time > 0) {
                q1.push(i); // Everyone starts at the VIP level!
                in_system[i] = true;
            }
        }

        // 2. Preemption Check (If Q1 gets a new arrival, pause Q2/Q3!)
        if (active_idx != -1) {
            if (active_queue == 2 && !q1.empty()) {
                q2.push(active_idx); // Put back in Q2
                active_idx = -1;
            } else if (active_queue == 3 && (!q1.empty() || !q2.empty())) {
                q3.push(active_idx); // Put back in Q3
                active_idx = -1;
            }
        }

        // 3. Pick the next process to run (Strict Priority)
        if (active_idx == -1) {
            if (!q1.empty()) {
                active_idx = q1.front();
                q1.pop();
                active_queue = 1;
                current_tq_used = 0;
            } else if (!q2.empty()) {
                active_idx = q2.front();
                q2.pop();
                active_queue = 2;
                current_tq_used = 0;
            } else if (!q3.empty()) {
                active_idx = q3.front();
                q3.pop();
                active_queue = 3;
                current_tq_used = 0;
            }
        }

        // 4. Execution Logic
        if (active_idx == -1) {
            // CPU is Idle
            if (last_pid != -1) {
                add_gantt_entry(last_pid, block_start_time, current_time);
                last_pid = -1;
            }
            current_time++;
            continue;
        }

        // Context Switch Check
        if (last_pid != processes[active_idx].pid) {
            if (last_pid != -1) {
                add_gantt_entry(last_pid, block_start_time, current_time);
            }
            block_start_time = current_time;
            last_pid = processes[active_idx].pid;
        }

        // The 1-Second Drip
        processes[active_idx].remaining_time -= 1;
        current_time += 1;
        current_tq_used += 1;

        // 5. Evaluate process after the drip
        if (processes[active_idx].remaining_time == 0) {
            completed++;
            processes[active_idx].completion_time = current_time;
            processes[active_idx].turnaround_time = processes[active_idx].completion_time - processes[active_idx].arrival_time;
            processes[active_idx].waiting_time = processes[active_idx].turnaround_time - processes[active_idx].burst_time;
            active_idx = -1; // CPU is free
        } else {
            // Check for demotion
            if (active_queue == 1 && current_tq_used == tq1) {
                q2.push(active_idx); // Too slow! Demoted to Q2.
                active_idx = -1;
            } else if (active_queue == 2 && current_tq_used == tq2) {
                q3.push(active_idx); // Still too slow! Demoted to Economy Q3.
                active_idx = -1;
            }
            // If it's in Q3, it just keeps running (FCFS) until it finishes or is preempted.
        }
    }

    // Save the final streak
    if (last_pid != -1) {
        add_gantt_entry(last_pid, block_start_time, current_time);
    }
}