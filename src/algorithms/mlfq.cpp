#include "algorithms/mlfq.h"
#include <deque>
#include <algorithm>

MLFQ::MLFQ(std::vector<int> quanta_, int aging_threshold_)
    : quanta(std::move(quanta_)), aging_threshold(aging_threshold_) {}

std::string MLFQ::getName() const {
    return "Multi-Level Feedback Queue (MLFQ, " + std::to_string(quanta.size()) + " levels)";
}

void MLFQ::schedule(std::vector<Process>& processes) {
    gantt_chart.clear();
    context_switches = 0;

    int n = static_cast<int>(processes.size());
    int num_levels = static_cast<int>(quanta.size());

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

    std::vector<std::deque<int>> queues(num_levels);
    std::vector<int> level(n, 0);
    std::vector<int> enqueue_time(n, 0);

    size_t next_arrival_ptr = 0;
    int current_time = 0;
    int completed = 0;

    auto admitArrivals = [&](int up_to_time) {
        while (next_arrival_ptr < arrival_order.size() &&
               processes[arrival_order[next_arrival_ptr]].arrival_time <= up_to_time) {
            int idx = arrival_order[next_arrival_ptr];
            queues[0].push_back(idx);
            level[idx] = 0;
            enqueue_time[idx] = processes[idx].arrival_time;
            next_arrival_ptr++;
        }
    };

    admitArrivals(0);
    if (next_arrival_ptr < arrival_order.size() &&
        std::all_of(queues.begin(), queues.end(), [](const std::deque<int>& q) { return q.empty(); })) {
        current_time = processes[arrival_order[0]].arrival_time;
        admitArrivals(current_time);
    }

    while (completed < n) {
        admitArrivals(current_time);

        // Aging pass: anyone stuck too long in a lower queue jumps back to the top.
        for (int lvl = 1; lvl < num_levels; lvl++) {
            std::deque<int> survivors;
            while (!queues[lvl].empty()) {
                int idx = queues[lvl].front();
                queues[lvl].pop_front();
                if (current_time - enqueue_time[idx] >= aging_threshold) {
                    level[idx] = 0;
                    enqueue_time[idx] = current_time;
                    queues[0].push_back(idx);
                } else {
                    survivors.push_back(idx);
                }
            }
            queues[lvl] = survivors;
        }

        int chosen_level = -1;
        for (int lvl = 0; lvl < num_levels; lvl++) {
            if (!queues[lvl].empty()) {
                chosen_level = lvl;
                break;
            }
        }

        if (chosen_level == -1) {
            if (next_arrival_ptr < arrival_order.size()) {
                current_time = processes[arrival_order[next_arrival_ptr]].arrival_time;
                continue;
            }
            break;
        }

        int idx = queues[chosen_level].front();
        queues[chosen_level].pop_front();
        Process& p = processes[idx];

        if (!p.started) {
            p.response_time = current_time - p.arrival_time;
            p.started = true;
        }

        int slice = std::min(quanta[chosen_level], p.remaining_time);
        int start = current_time;
        current_time += slice;
        p.remaining_time -= slice;
        addGanttEntry(p.pid, start, current_time);

        admitArrivals(current_time);

        if (p.remaining_time > 0) {
            int new_level = std::min(chosen_level + 1, num_levels - 1);
            level[idx] = new_level;
            enqueue_time[idx] = current_time;
            queues[new_level].push_back(idx);
        } else {
            p.completion_time = current_time;
            p.turnaround_time = p.completion_time - p.arrival_time;
            p.waiting_time = p.turnaround_time - p.burst_time;
            completed++;
        }
    }
}
