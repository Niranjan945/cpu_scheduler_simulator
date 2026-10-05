#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include "./manager.h"

class RoundRobin : public Manager {
    protected:
        int time_quantum;
    public:
        RoundRobin(int tq) : time_quantum(tq) {}
        void schedule(std::vector<Process>& processes) override;
};

#endif // ROUND_ROBIN_H