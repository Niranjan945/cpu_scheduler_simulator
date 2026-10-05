//shortest job first scheduling algorithm without preemption
#ifndef SJF_h
#define SJF_h

#include "./manager.h"

class SJF: public Manager{
    public:
    void schedule(std::vector<Process>& processes) override;
};

#endif // SJF_h