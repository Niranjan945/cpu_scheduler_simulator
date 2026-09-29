#ifndef SRTF_H
#define SRTF_H

#include "scheduler.h"

// Shortest Remaining Time First: preemptive version of SJF.
// At every time unit the process with the smallest remaining burst runs;
// a fresh arrival with a shorter burst can preempt the running process.
class SRTF : public Scheduler {
public:
    std::string getName() const override;
    void schedule(std::vector<Process>& processes) override;
};

#endif
