#ifndef FCFS_H
#define FCFS_H

#include "scheduler.h"

// First Come First Served: non-preemptive, purely arrival-order based.
class FCFS : public Scheduler {
public:
    std::string getName() const override;
    void schedule(std::vector<Process>& processes) override;
};

#endif
