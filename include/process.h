#ifndef PROCESS_H
#define PROCESS_H

// Represents a single process/job for the CPU scheduling simulation.
// Static fields (pid, arrival_time, burst_time, priority, memory_size) are
// the "input" of a process and never change once created.
// Runtime fields are recomputed every time a scheduler runs, so a process
// can be reused across multiple algorithms via reset().
struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;      // lower number = higher priority
    int memory_size;   // in KB, used by the memory management demos

    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    bool started;

    Process(int pid_, int arrival_, int burst_, int priority_ = 0, int memory_ = 0);

    // Clears all runtime fields so the same process list can be fed into
    // a different scheduling algorithm without leftover state.
    void reset();
};

#endif
