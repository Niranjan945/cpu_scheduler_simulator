#include "algorithms/fcfs.h"
#include <algorithm>

std::string FCFS::getName() const {
    return "First Come First Served (FCFS)";
}

void FCFS::schedule(std::vector<Process>& processes) {
    gantt_chart.clear();
    context_switches = 0;

    std::vector<int> order(processes.size());
    for (size_t i = 0; i < order.size(); i++) {
        order[i] = static_cast<int>(i);
    }
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        if (processes[a].arrival_time != processes[b].arrival_time) {
            return processes[a].arrival_time < processes[b].arrival_time;
        }
        return processes[a].pid < processes[b].pid;
    });

    int current_time = 0;
    for (int idx : order) {
        Process& p = processes[idx];
        if (current_time < p.arrival_time) {
            current_time = p.arrival_time;
        }

        p.response_time = current_time - p.arrival_time;
        p.started = true;
        int start = current_time;
        current_time += p.burst_time;
        p.completion_time = current_time;
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
        p.remaining_time = 0;

        addGanttEntry(p.pid, start, current_time);
    }
}
