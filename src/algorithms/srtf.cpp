#include "algorithms/srtf.h"
#include <limits>

std::string SRTF::getName() const {
    return "Shortest Remaining Time First (SRTF, preemptive)";
}

void SRTF::schedule(std::vector<Process>& processes) {
    gantt_chart.clear();
    context_switches = 0;

    int n = static_cast<int>(processes.size());
    int completed = 0;
    int current_time = 0;

    int max_finish = 0;
    for (const auto& p : processes) {
        max_finish = std::max(max_finish, p.arrival_time + p.burst_time);
    }
    // Safety cap in case of malformed input, so the loop can never spin forever.
    int time_limit = max_finish + 1;

    while (completed < n && current_time <= time_limit) {
        int running_idx = -1;
        int shortest = std::numeric_limits<int>::max();

        for (int i = 0; i < n; i++) {
            Process& p = processes[i];
            if (p.arrival_time <= current_time && p.remaining_time > 0) {
                if (p.remaining_time < shortest) {
                    shortest = p.remaining_time;
                    running_idx = i;
                }
            }
        }

        if (running_idx == -1) {
            current_time++;
            continue;
        }

        Process& p = processes[running_idx];
        if (!p.started) {
            p.response_time = current_time - p.arrival_time;
            p.started = true;
        }

        addGanttEntry(p.pid, current_time, current_time + 1);
        p.remaining_time--;
        current_time++;

        if (p.remaining_time == 0) {
            p.completion_time = current_time;
            p.turnaround_time = p.completion_time - p.arrival_time;
            p.waiting_time = p.turnaround_time - p.burst_time;
            completed++;
        }
    }
}
