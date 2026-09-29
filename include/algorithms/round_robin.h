#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include "scheduler.h"

// Time-quantum based preemptive scheduling using a circular ready queue.
class RoundRobin : public Scheduler {
public:
    explicit RoundRobin(int quantum);

    std::string getName() const override;
    void schedule(std::vector<Process>& processes) override;

private:
    int quantum;
};

#endif
