#ifndef FCFS_H
#define FCFS_H

#include "./manager.h"

class FCFS : public Manager {
public:
    // We are overriding the blank '= 0' function from the Manager
    // and promising to provide actual FCFS sorting logic.
    void schedule(std::vector<Process>& processes) override;
};

#endif // FCFS_H