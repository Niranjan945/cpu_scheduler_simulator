#include "scheduler.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

void Scheduler::addGanttEntry(int pid, int start, int end) {
    if (start == end) {
        return;
    }
    if (!gantt_chart.empty()) {
        GanttEntry& last = gantt_chart.back();
        if (last.pid == pid && last.end_time == start) {
            last.end_time = end;
            return;
        }
    }
    gantt_chart.push_back({pid, start, end});
    context_switches++;
}

const std::vector<GanttEntry>& Scheduler::getGanttChart() const {
    return gantt_chart;
}

Metrics Scheduler::computeMetrics(const std::vector<Process>& processes) const {
    Metrics m{};
    int n = static_cast<int>(processes.size());
    if (n == 0) {
        return m;
    }

    double total_waiting = 0.0;
    double total_turnaround = 0.0;
    double total_response = 0.0;
    int earliest_arrival = processes[0].arrival_time;
    int latest_completion = processes[0].completion_time;
    int total_burst = 0;

    for (int i = 0; i < n; i++) {
        const Process& p = processes[i];
        total_waiting += p.waiting_time;
        total_turnaround += p.turnaround_time;
        total_response += p.response_time;
        total_burst += p.burst_time;
        earliest_arrival = std::min(earliest_arrival, p.arrival_time);
        latest_completion = std::max(latest_completion, p.completion_time);
    }

    m.avg_waiting_time = total_waiting / n;
    m.avg_turnaround_time = total_turnaround / n;
    m.avg_response_time = total_response / n;
    m.context_switches = context_switches;

    int total_time = latest_completion - earliest_arrival;
    if (total_time > 0) {
        m.cpu_utilization = (static_cast<double>(total_burst) / total_time) * 100.0;
        m.throughput = static_cast<double>(n) / total_time;
    } else {
        m.cpu_utilization = 0.0;
        m.throughput = 0.0;
    }

    return m;
}

void Scheduler::printGanttChart() const {
    std::cout << "\nGantt Chart (" << getName() << ")\n";

    std::cout << " ";
    for (const auto& entry : gantt_chart) {
        int width = std::max(4, (entry.end_time - entry.start_time) * 2);
        std::string label = "P" + std::to_string(entry.pid);
        int pad_total = width - static_cast<int>(label.size());
        int pad_left = pad_total / 2;
        int pad_right = pad_total - pad_left;
        std::cout << "|" << std::string(pad_left, ' ') << label << std::string(pad_right, ' ');
    }
    std::cout << "|\n";

    std::cout << gantt_chart.empty() ? "0" : std::to_string(gantt_chart.front().start_time);
    for (const auto& entry : gantt_chart) {
        int width = std::max(4, (entry.end_time - entry.start_time) * 2);
        std::string label = std::to_string(entry.end_time);
        std::cout << std::string(width - static_cast<int>(label.size()) + 1, ' ') << label;
    }
    std::cout << "\n";
}

void Scheduler::printMetricsTable(const std::vector<Process>& processes) const {
    std::cout << "\n" << std::left
               << std::setw(6) << "PID"
               << std::setw(10) << "Arrival"
               << std::setw(8) << "Burst"
               << std::setw(10) << "Priority"
               << std::setw(12) << "Completion"
               << std::setw(12) << "Turnaround"
               << std::setw(10) << "Waiting"
               << std::setw(10) << "Response" << "\n";

    std::cout << std::string(78, '-') << "\n";

    std::vector<Process> sorted = processes;
    std::sort(sorted.begin(), sorted.end(), [](const Process& a, const Process& b) {
        return a.pid < b.pid;
    });

    for (const auto& p : sorted) {
        std::cout << std::left
                   << std::setw(6) << p.pid
                   << std::setw(10) << p.arrival_time
                   << std::setw(8) << p.burst_time
                   << std::setw(10) << p.priority
                   << std::setw(12) << p.completion_time
                   << std::setw(12) << p.turnaround_time
                   << std::setw(10) << p.waiting_time
                   << std::setw(10) << p.response_time << "\n";
    }

    Metrics m = computeMetrics(processes);
    std::cout << std::string(78, '-') << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average Waiting Time    : " << m.avg_waiting_time << "\n";
    std::cout << "Average Turnaround Time : " << m.avg_turnaround_time << "\n";
    std::cout << "Average Response Time   : " << m.avg_response_time << "\n";
    std::cout << "CPU Utilization         : " << m.cpu_utilization << "%\n";
    std::cout << "Throughput              : " << m.throughput << " processes/unit time\n";
    std::cout << "Context Switches        : " << m.context_switches << "\n";
}
