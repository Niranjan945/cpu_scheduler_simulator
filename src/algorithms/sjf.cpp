#include "algorithms/sjf.h"
#include <limits>

std::string SJF::getName() const {
    return "Shortest Job First (SJF, non-preemptive)";
}

void SJF::schedule(std::vector<Process>& processes) {
    gantt_chart.clear();
    context_switches = 0;

    int n = static_cast<int>(processes.size());
    std::vector<bool> done(n, false);
    int completed = 0;
    int current_time = 0;

    while (completed < n) {
        int best_idx = -1;
        int best_burst = std::numeric_limits<int>::max();

        for (int i = 0; i < n; i++) {
            if (done[i] || processes[i].arrival_time > current_time) {
                continue;
            }
            if (processes[i].burst_time < best_burst ||
                (processes[i].burst_time == best_burst &&
                 processes[i].arrival_time < processes[best_idx].arrival_time)) {
                best_burst = processes[i].burst_time;
                best_idx = i;
            }
        }

        if (best_idx == -1) {
            // Nobody has arrived yet; jump to the next arrival.
            int next_arrival = std::numeric_limits<int>::max();
            for (int i = 0; i < n; i++) {
                if (!done[i]) {
                    next_arrival = std::min(next_arrival, processes[i].arrival_time);
                }
            }
            current_time = next_arrival;
            continue;
        }

        Process& p = processes[best_idx];
        p.response_time = current_time - p.arrival_time;
        p.started = true;
        int start = current_time;
        current_time += p.burst_time;
        p.completion_time = current_time;
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
        p.remaining_time = 0;

        addGanttEntry(p.pid, start, current_time);
        done[best_idx] = true;
        completed++;
    }
}
