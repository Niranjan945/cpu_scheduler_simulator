#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>
#include <vector>
#include "process.h"

// One contiguous slice of CPU time given to a process on the Gantt chart.
struct GanttEntry {
    int pid;
    int start_time;
    int end_time;
};

struct Metrics {
    double avg_waiting_time;
    double avg_turnaround_time;
    double avg_response_time;
    double cpu_utilization;
    double throughput;
    int context_switches;
};

// Base class for every scheduling algorithm (Strategy pattern).
// Each concrete algorithm implements schedule(), filling in the runtime
// fields of every Process and recording CPU slices via addGanttEntry().
class Scheduler {
public:
    virtual ~Scheduler() = default;

    virtual std::string getName() const = 0;
    virtual void schedule(std::vector<Process>& processes) = 0;

    const std::vector<GanttEntry>& getGanttChart() const;
    Metrics computeMetrics(const std::vector<Process>& processes) const;

    void printGanttChart() const;
    void printMetricsTable(const std::vector<Process>& processes) const;

protected:
    std::vector<GanttEntry> gantt_chart;
    int context_switches = 0;

    // Appends a CPU slice. If it directly continues the previous slice for
    // the same pid, the two slices are merged instead of duplicated.
    void addGanttEntry(int pid, int start, int end);
};

#endif
