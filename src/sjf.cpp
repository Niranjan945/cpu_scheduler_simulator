//Logic for SJF Without preemption
// Works according to : "smallest burst time first" principle
#include "../include/sjf.h"
#include <iostream>
#include <climits>

void SJF::schedule(std::vector<Process>& processes) {
    int current_time = 0;
    int completed=0;
    int n=processes.size();
    while(completed < n ){
        int idx=-1;
        int min_burst=INT_MAX;

        for(int i=0;i<n;i++){
            if(processes[i].arrival_time<=current_time && processes[i].remaining_time > 0 && processes[i].burst_time<min_burst){
                    min_burst=processes[i].burst_time;
                    idx=i;
            }  
        }

        if(idx==-1){
            current_time+=1;
            continue;
        }
        else{
            current_time+=processes[idx].burst_time;
            processes[idx].remaining_time = 0;
            processes[idx].completion_time = current_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            add_gantt_entry(processes[idx].pid, current_time - processes[idx].burst_time, current_time);
            completed++;
        }
    }
}