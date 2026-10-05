#ifndef PROCESS_H
#define PROCESS_H

struct Process {
    // 1. Permanent Inputs
    int pid;
    int arrival_time;
    int burst_time;
    int priority; // <--- 1. NEW COLUMN

    // 2. Dynamic Trackers
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    bool started;

    // 3. Update the constructor declaration with a default value
    Process(int id, int arrival, int burst, int prio = 0);
    void reset();
};

#endif // PROCESS_H