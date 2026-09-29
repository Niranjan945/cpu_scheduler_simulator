#ifndef PRIORITY_H
#define PRIORITY_H

#include "scheduler.h"

// Priority scheduling, non-preemptive. Lower priority number = runs first.
class PriorityScheduler : public Scheduler {
public:
    std::string getName() const override;
    void schedule(std::vector<Process>& processes) override;
};

#endif
