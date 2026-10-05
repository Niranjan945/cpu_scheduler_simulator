#include "../include/rr.h"
#include <queue>

void RoundRobin::schedule(std::vector<Process>& processes){
     int current_time=0;
     int completed_processes=0;

     std::queue<int>ready_queue;
     std::vector<bool> in_queue(processes.size(), false);

     int n=processes.size();
     while(completed_processes<n){

        //1. Add all processes that have arrived to the ready queue
        for(int i=0;i<n;i++){
            // Check if the process has arrived and is not already in the queue
            if(processes[i].arrival_time<=current_time && processes[i].remaining_time>0 && !in_queue[i]){
                ready_queue.push(i);
                in_queue[i]=true;
            }
        }
        
        //2. If the ready queue is empty , increment the current time and continue to the next iteration as cpu sits idle.
        if(ready_queue.empty()){
            current_time+=1;
            continue;
        }
        //3. if the ready queue is not empty, pop the front process from the queue and execute it for a time quantum or until it finishes, whichever comes first.
        else{
            int top_index=ready_queue.front();
            ready_queue.pop();

            // Execute the process for a time quantum or until it finishes
            int execution_time=std::min(time_quantum,processes[top_index].remaining_time);
            processes[top_index].remaining_time-=execution_time;
            current_time+=execution_time;

            //store to gantt chart
            add_gantt_entry(processes[top_index].pid,current_time-execution_time,current_time);


            for(int i=0;i<n;i++){
            // Check if the process has arrived and is not already in the queue
            if(processes[i].arrival_time<=current_time && processes[i].remaining_time>0 && !in_queue[i]){
                ready_queue.push(i);
                in_queue[i]=true;
            }
             }

            // If the process is not finished, add it back to the ready queue
            if(processes[top_index].remaining_time>0){
                ready_queue.push(top_index);
                in_queue[top_index]=true;
            }
            else{
                completed_processes++;
                processes[top_index].completion_time=current_time;
                processes[top_index].turnaround_time=processes[top_index].completion_time-processes[top_index].arrival_time;
                processes[top_index].waiting_time=processes[top_index].turnaround_time-processes[top_index].burst_time;
                in_queue[top_index]=false;
            }
     }

}
}