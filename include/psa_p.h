#ifndef PSA_P_H
#define PSA_P_H

#include "manager.h" // (Using standard include path)

class PSA_P : public Manager {
public:
    void schedule(std::vector<Process>& processes) override;
};

#endif /* PSA_P_H */