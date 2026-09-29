#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <limits>
#include <iomanip>
#include <algorithm>

#include "process.h"
#include "scheduler.h"
#include "algorithms/fcfs.h"
#include "algorithms/sjf.h"
#include "algorithms/srtf.h"
#include "algorithms/priority.h"
#include "algorithms/priority_aging.h"
#include "algorithms/round_robin.h"
#include "algorithms/mlfq.h"

#include "memory/mmu.h"
#include "memory/replacement.h"
#include "memory/segmentation.h"

// ---------- input helpers ----------

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }
        std::cout << "Please enter a valid integer.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::vector<Process> loadProcessesFromCSV(const std::string& path) {
    std::vector<Process> processes;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Could not open " << path << "\n";
        return processes;
    }

    std::string line;
    bool first_line = true;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (first_line) {
            // Skip header row if it doesn't start with a digit.
            first_line = false;
            if (!isdigit(static_cast<unsigned char>(line[0]))) continue;
        }

        std::stringstream ss(line);
        std::string field;
        std::vector<int> values;
        while (std::getline(ss, field, ',')) {
            values.push_back(std::stoi(field));
        }
        if (values.size() < 3) continue;

        int pid = values[0];
        int arrival = values[1];
        int burst = values[2];
        int priority = values.size() > 3 ? values[3] : 0;
        int memory = values.size() > 4 ? values[4] : 0;
        processes.emplace_back(pid, arrival, burst, priority, memory);
    }
    return processes;
}

std::vector<Process> inputProcessesManually() {
    std::vector<Process> processes;
    int n = readInt("Number of processes: ");
    for (int i = 0; i < n; i++) {
        std::cout << "\nProcess P" << (i + 1) << "\n";
        int arrival = readInt("  Arrival time : ");
        int burst = readInt("  Burst time   : ");
        int priority = readInt("  Priority (lower = more important) : ");
        int memory = readInt("  Memory size (KB) : ");
        processes.emplace_back(i + 1, arrival, burst, priority, memory);
    }
    return processes;
}

std::vector<Process> getProcessInput() {
    std::cout << "\n1. Enter processes manually\n2. Load from data/processes_sample.csv\n";
    int choice = readInt("Choice: ");
    if (choice == 2) {
        std::vector<Process> processes = loadProcessesFromCSV("data/processes_sample.csv");
        if (processes.empty()) {
            std::cout << "CSV load failed or empty, falling back to manual entry.\n";
            return inputProcessesManually();
        }
        std::cout << "Loaded " << processes.size() << " processes.\n";
        return processes;
    }
    return inputProcessesManually();
}

void resetAll(std::vector<Process>& processes) {
    for (auto& p : processes) {
        p.reset();
    }
}

// ---------- scheduling menu ----------

std::unique_ptr<Scheduler> buildScheduler(int choice) {
    switch (choice) {
        case 1: return std::make_unique<FCFS>();
        case 2: return std::make_unique<SJF>();
        case 3: return std::make_unique<SRTF>();
        case 4: return std::make_unique<PriorityScheduler>();
        case 5: return std::make_unique<PriorityAging>(5);
        case 6: {
            int quantum = readInt("Time quantum: ");
            return std::make_unique<RoundRobin>(quantum);
        }
        case 7: return std::make_unique<MLFQ>(std::vector<int>{4, 8, 16}, 20);
        default: return nullptr;
    }
}

void runSingleAlgorithm() {
    std::vector<Process> processes = getProcessInput();
    if (processes.empty()) return;

    std::cout << "\nAlgorithms:\n"
               << " 1. FCFS\n 2. SJF (non-preemptive)\n 3. SRTF (preemptive)\n"
               << " 4. Priority (non-preemptive)\n 5. Priority with aging (preemptive)\n"
               << " 6. Round Robin\n 7. MLFQ\n";
    int choice = readInt("Choice: ");

    resetAll(processes);
    auto scheduler = buildScheduler(choice);
    if (!scheduler) {
        std::cout << "Invalid choice.\n";
        return;
    }

    scheduler->schedule(processes);
    scheduler->printGanttChart();
    scheduler->printMetricsTable(processes);
}

void compareAllAlgorithms() {
    std::vector<Process> base_processes = getProcessInput();
    if (base_processes.empty()) return;

    int quantum = readInt("Time quantum to use for Round Robin: ");

    std::vector<std::unique_ptr<Scheduler>> schedulers;
    schedulers.push_back(std::make_unique<FCFS>());
    schedulers.push_back(std::make_unique<SJF>());
    schedulers.push_back(std::make_unique<SRTF>());
    schedulers.push_back(std::make_unique<PriorityScheduler>());
    schedulers.push_back(std::make_unique<PriorityAging>(5));
    schedulers.push_back(std::make_unique<RoundRobin>(quantum));
    schedulers.push_back(std::make_unique<MLFQ>(std::vector<int>{4, 8, 16}, 20));

    std::cout << "\n" << std::left
               << std::setw(52) << "Algorithm"
               << std::setw(14) << "Avg Wait"
               << std::setw(16) << "Avg Turnaround"
               << std::setw(14) << "Avg Response" << "\n";
    std::cout << std::string(96, '-') << "\n";

    for (auto& scheduler : schedulers) {
        std::vector<Process> processes = base_processes;
        resetAll(processes);
        scheduler->schedule(processes);
        Metrics m = scheduler->computeMetrics(processes);
        std::cout << std::left << std::fixed << std::setprecision(2)
                   << std::setw(52) << scheduler->getName()
                   << std::setw(14) << m.avg_waiting_time
                   << std::setw(14) << m.avg_turnaround_time
                   << std::setw(14) << m.avg_response_time << "\n";
    }
}

// ---------- memory menu ----------

std::vector<int> readReferenceString() {
    std::cout << "\n1. Enter reference string manually\n2. Generate a sample reference string\n";
    int choice = readInt("Choice: ");
    std::vector<int> refs;
    if (choice == 2) {
        refs = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
        std::cout << "Using sample reference string: ";
        for (int r : refs) std::cout << r << " ";
        std::cout << "\n";
        return refs;
    }
    int n = readInt("How many page references? ");
    std::cout << "Enter " << n << " page numbers separated by spaces: ";
    for (int i = 0; i < n; i++) {
        int p;
        std::cin >> p;
        refs.push_back(p);
    }
    return refs;
}

void pagingDemo() {
    int num_frames = readInt("Number of physical frames: ");
    std::vector<int> refs = readReferenceString();

    int max_page = 0;
    for (int r : refs) max_page = std::max(max_page, r);

    std::cout << "\nReplacement algorithm:\n 1. FIFO\n 2. LRU\n 3. Optimal\n";
    int choice = readInt("Choice: ");

    std::unique_ptr<ReplacementAlgorithm> algo;
    if (choice == 1) algo = std::make_unique<FIFOReplacement>();
    else if (choice == 2) algo = std::make_unique<LRUReplacement>(num_frames);
    else algo = std::make_unique<OptimalReplacement>();

    MMU mmu(max_page + 1, num_frames, std::move(algo));
    mmu.simulate(refs);
}

void segmentationDemo() {
    SegmentTable table;
    table.addSegment("code", 1000, 400);
    table.addSegment("data", 2000, 800);
    table.addSegment("stack", 5000, 1000);
    table.addSegment("heap", 8000, 2000);
    table.printTable();

    std::cout << "\nEnter (segment_id, offset) pairs to translate. Enter -1 to stop.\n";
    while (true) {
        int seg_id = readInt("Segment id (-1 to stop): ");
        if (seg_id == -1) break;
        int offset = readInt("Offset: ");

        int physical;
        if (table.translate(seg_id, offset, physical)) {
            std::cout << "  -> physical address = " << physical << "\n";
        } else {
            std::cout << "  -> SEGMENTATION FAULT (invalid segment or offset out of bounds)\n";
        }
    }
}

void memoryMenu() {
    std::cout << "\n1. Paging (page table + TLB + replacement algorithm)\n"
               << "2. Segmentation (base/limit translation)\n"
               << "3. Back\n";
    int choice = readInt("Choice: ");
    if (choice == 1) pagingDemo();
    else if (choice == 2) segmentationDemo();
}

// ---------- top level ----------

int main() {
    std::cout << "=====================================\n";
    std::cout << "   CPU Scheduler & Memory Simulator\n";
    std::cout << "=====================================\n";

    while (true) {
        std::cout << "\nMain Menu\n"
                   << " 1. Run a single scheduling algorithm\n"
                   << " 2. Compare all scheduling algorithms\n"
                   << " 3. Memory management demos\n"
                   << " 4. Exit\n";
        int choice = readInt("Choice: ");

        if (choice == 1) runSingleAlgorithm();
        else if (choice == 2) compareAllAlgorithms();
        else if (choice == 3) memoryMenu();
        else if (choice == 4) break;
        else std::cout << "Invalid choice.\n";
    }

    std::cout << "Goodbye.\n";
    return 0;
}
