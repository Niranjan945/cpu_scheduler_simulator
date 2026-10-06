#ifndef MLFQ_H
#define MLFQ_H

#include "manager.h"
#include <vector>

class MLFQ : public Manager {
public:
    void schedule(std::vector<Process>& processes) override;
};

#endif // MLFQ_H