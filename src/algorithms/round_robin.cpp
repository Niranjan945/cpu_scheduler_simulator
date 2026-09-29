#include "algorithms/round_robin.h"
#include <queue>
#include <algorithm>

RoundRobin::RoundRobin(int quantum_) : quantum(quantum_) {}

std::string RoundRobin::getName() const {
    return "Round Robin (quantum = " + std::to_string(quantum) + ")";
}

void RoundRobin::schedule(std::vector<Process>& processes) {
    gantt_chart.clear();
    context_switches = 0;

    int n = static_cast<int>(processes.size());

    std::vector<int> arrival_order(n);
    for (int i = 0; i < n; i++) {
        arrival_order[i] = i;
    }
    std::sort(arrival_order.begin(), arrival_order.end(), [&](int a, int b) {
        if (processes[a].arrival_time != processes[b].arrival_time) {
            return processes[a].arrival_time < processes[b].arrival_time;
        }
        return processes[a].pid < processes[b].pid;
    });

    std::vector<bool> in_queue(n, false);
    std::queue<int> ready_queue;
    int current_time = 0;
    int completed = 0;
    size_t next_arrival_ptr = 0;

    auto admitArrivals = [&](int up_to_time) {
        while (next_arrival_ptr < arrival_order.size() &&
               processes[arrival_order[next_arrival_ptr]].arrival_time <= up_to_time) {
            int idx = arrival_order[next_arrival_ptr];
            ready_queue.push(idx);
            in_queue[idx] = true;
            next_arrival_ptr++;
        }
    };

    admitArrivals(0);
    if (ready_queue.empty() && next_arrival_ptr < arrival_order.size()) {
        current_time = processes[arrival_order[0]].arrival_time;
        admitArrivals(current_time);
    }

    while (completed < n) {
        if (ready_queue.empty()) {
            if (next_arrival_ptr < arrival_order.size()) {
                current_time = processes[arrival_order[next_arrival_ptr]].arrival_time;
                admitArrivals(current_time);
            }
            continue;
        }

        int idx = ready_queue.front();
        ready_queue.pop();
        in_queue[idx] = false;
        Process& p = processes[idx];

        if (!p.started) {
            p.response_time = current_time - p.arrival_time;
            p.started = true;
        }

        int run_time = std::min(quantum, p.remaining_time);
        int start = current_time;
        current_time += run_time;
        p.remaining_time -= run_time;
        addGanttEntry(p.pid, start, current_time);

        // New arrivals that showed up during this slice join the queue
        // before the process being preempted rejoins it (standard convention).
        admitArrivals(current_time);

        if (p.remaining_time > 0) {
            ready_queue.push(idx);
            in_queue[idx] = true;
        } else {
            p.completion_time = current_time;
            p.turnaround_time = p.completion_time - p.arrival_time;
            p.waiting_time = p.turnaround_time - p.burst_time;
            completed++;
        }
    }
}
