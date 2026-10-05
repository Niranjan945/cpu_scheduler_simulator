#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include "./pcb.h"

struct GanttEntry {
    int id;
    int start_time;
    int end_time;
};

class Manager{
    protected:
        std::vector<GanttEntry> gantt_chart;
    public:
        
        void print_gantt_chart();
        void add_gantt_entry(int id, int start_time, int end_time);
        virtual void schedule(std::vector<Process>& processes) = 0;
};  


#endif // MANAGER_H