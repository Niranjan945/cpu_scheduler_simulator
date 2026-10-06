#include <iostream>
#include <vector>
#include <string>
#include "include/pcb.h"
#include "include/schedulers.h"

void print_results(Manager* scheduler, const std::vector<Process>& processes, std::string algo_name) {
    std::cout << "\n--- " << algo_name << " Results ---\n";
    std::cout << "PID\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n";
    std::cout << "-----------------------------------------------------------\n";
    
    for (const auto& p : processes) {
        std::cout << p.pid << "\t" 
                  << p.arrival_time << "\t" 
                  << p.burst_time << "\t" 
                  << p.completion_time << "\t\t" 
                  << p.waiting_time << "\t" 
                  << p.turnaround_time << "\n";
    }

    scheduler->print_gantt_chart();
}

int main() {
    std::cout << "=================================\n";
    std::cout << "   OS CPU SCHEDULING SIMULATOR   \n";
    std::cout << "=================================\n";
    
    int choice = 0;
    while (true) {
       std::cout << "\n--- Select Scheduling Algorithm ---\n";
        std::cout << "1. First Come First Serve (FCFS)\n";
        std::cout << "2. Round Robin (RR)\n";
        std::cout << "3. Shortest Job First (SJF - Non-Preemptive)\n";
        std::cout << "4. Shortest Remaining Time First (SRTF - Preemptive SJF)\n";
        std::cout << "5. Priority Scheduling (Preemptive)\n";
        std::cout << "6. Multilevel Feedback Queue (MLFQ)\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (choice == 7) {
            std::cout << "Exiting Simulator. Goodbye!\n";
            break;
        }

        if (choice < 1 || choice > 7) {
            std::cout << "Invalid choice. Please enter 1-7.\n";
            continue; 
        }

        // Now that they picked a valid algorithm, ask for the processes
        int n;
        std::cout << "Enter the number of processes: ";
        std::cin >> n;

        std::vector<Process> current_run;
        for (int i = 0; i < n; i++) {
            int arrival, burst, prio;
            std::cout << "Process " << (i + 1) << " Arrival Time: ";
            std::cin >> arrival;
            std::cout << "Process " << (i + 1) << " Burst Time: ";
            std::cin >> burst;
            std::cout << "Process " << (i + 1) << " Priority (Lower # = Higher Priority): ";
            std::cin >> prio;
            // Pass the priority to the constructor!
            current_run.push_back(Process(i + 1, arrival, burst, prio)); 
        }

        switch (choice) {
            case 1: {
                FCFS scheduler;
                scheduler.schedule(current_run);
                print_results(&scheduler, current_run, "FCFS");
                break;
            }
            case 2: {
                int tq;
                std::cout << "Enter Time Quantum for Round Robin: ";
                std::cin >> tq;
                RoundRobin scheduler(tq); 
                scheduler.schedule(current_run);
                print_results(&scheduler, current_run, "Round Robin");
                break;
            }
            case 3: {
                SJF scheduler;
                scheduler.schedule(current_run);
                print_results(&scheduler, current_run, "SJF (Non-Preemptive)");
                break;
            }
            case 4: {
                SJF_P scheduler;
                scheduler.schedule(current_run);
                print_results(&scheduler, current_run, "SRTF (Preemptive SJF)");
                break;
            }
            case 5: {
                PSA_P scheduler;
                scheduler.schedule(current_run);
                print_results(&scheduler, current_run, "Priority Scheduling (Preemptive)");
                break;
            }
            case 6: {
             MLFQ scheduler;
             scheduler.schedule(current_run);
             print_results(&scheduler, current_run, "Multilevel Feedback Queue (Q1=2, Q2=4, Q3=FCFS)");
             break;
            }
        }
    }

    return 0;
}