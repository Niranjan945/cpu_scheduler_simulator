#ifndef SJF_H
#define SJF_H

#include "scheduler.h"

// Shortest Job First: non-preemptive. Among arrived processes, always
// picks the one with the smallest total burst time.
class SJF : public Scheduler {
public:
    std::string getName() const override;
    void schedule(std::vector<Process>& processes) override;
};

#endif
