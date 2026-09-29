#ifndef PRIORITY_AGING_H
#define PRIORITY_AGING_H

#include "scheduler.h"

// Preemptive priority scheduling with aging: every AGING_THRESHOLD ticks a
// process spends waiting in the ready state, its effective priority is
// improved by 1. This bounds worst-case starvation of low priority jobs.
class PriorityAging : public Scheduler {
public:
    explicit PriorityAging(int aging_threshold = 5);

    std::string getName() const override;
    void schedule(std::vector<Process>& processes) override;

private:
    int aging_threshold;
};

#endif
