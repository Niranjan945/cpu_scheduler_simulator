#ifndef MLFQ_H
#define MLFQ_H

#include "scheduler.h"
#include <vector>

// Multi-Level Feedback Queue: several ready queues with increasing
// quantum/priority. New processes enter the top queue; a process that
// doesn't finish within its quantum is demoted one level. A process
// that waits too long in a lower queue is promoted back to the top
// queue (aging), which prevents starvation.
class MLFQ : public Scheduler {
public:
    explicit MLFQ(std::vector<int> quanta = {4, 8, 16}, int aging_threshold = 20);

    std::string getName() const override;
    void schedule(std::vector<Process>& processes) override;

private:
    std::vector<int> quanta;   // quanta.back() level behaves like FCFS
    int aging_threshold;
};

#endif
