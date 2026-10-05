//SJF with preemtion
#ifndef SJF_P_h
#define SJF_P_h

#include "./manager.h"

class SJF_P : public Manager{
    public:
    void schedule(std::vector<Process>& processes) override;
};

#endif // SJF_P_h