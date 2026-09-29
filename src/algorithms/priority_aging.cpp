#include "algorithms/priority_aging.h"
#include <limits>

PriorityAging::PriorityAging(int aging_threshold_) : aging_threshold(aging_threshold_) {}

std::string PriorityAging::getName() const {
    return "Priority Scheduling with Aging (preemptive)";
}

void PriorityAging::schedule(std::vector<Process>& processes) {
    gantt_chart.clear();
    context_switches = 0;

    int n = static_cast<int>(processes.size());
    std::vector<int> dynamic_priority(n);
    std::vector<int> wait_ticks(n, 0);
    for (int i = 0; i < n; i++) {
        dynamic_priority[i] = processes[i].priority;
    }

    int completed = 0;
    int current_time = 0;
    int max_finish = 0;
    for (const auto& p : processes) {
        max_finish = std::max(max_finish, p.arrival_time + p.burst_time);
    }
    int time_limit = max_finish + 1;

    while (completed < n && current_time <= time_limit) {
        int running_idx = -1;
        int best_priority = std::numeric_limits<int>::max();

        for (int i = 0; i < n; i++) {
            Process& p = processes[i];
            if (p.arrival_time <= current_time && p.remaining_time > 0) {
                if (dynamic_priority[i] < best_priority) {
                    best_priority = dynamic_priority[i];
                    running_idx = i;
                }
            }
        }

        if (running_idx == -1) {
            current_time++;
            continue;
        }

        // Everyone else who is ready but didn't get picked ages by one tick.
        for (int i = 0; i < n; i++) {
            Process& p = processes[i];
            if (i == running_idx || p.arrival_time > current_time || p.remaining_time == 0) {
                continue;
            }
            wait_ticks[i]++;
            if (wait_ticks[i] >= aging_threshold && dynamic_priority[i] > 0) {
                dynamic_priority[i]--;
                wait_ticks[i] = 0;
            }
        }

        Process& p = processes[running_idx];
        if (!p.started) {
            p.response_time = current_time - p.arrival_time;
            p.started = true;
        }

        addGanttEntry(p.pid, current_time, current_time + 1);
        p.remaining_time--;
        wait_ticks[running_idx] = 0;
        current_time++;

        if (p.remaining_time == 0) {
            p.completion_time = current_time;
            p.turnaround_time = p.completion_time - p.arrival_time;
            p.waiting_time = p.turnaround_time - p.burst_time;
            completed++;
        }
    }
}
