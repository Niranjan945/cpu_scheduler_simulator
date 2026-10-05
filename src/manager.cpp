#include "../include/manager.h"
#include <iostream>

void Manager::add_gantt_entry(int id, int start, int end) {
    gantt_chart.push_back({id, start, end});
}

void Manager::print_gantt_chart() {
    std::cout << "\n=== GANTT CHART TIMELINE ===\n";
    
    if (gantt_chart.empty()) return;

    // --- NEW: Pre-process the chart to find idle gaps ---
    std::vector<GanttEntry> display_chart;
    int last_time = 0; // The CPU always starts at time 0

    for (const auto& entry : gantt_chart) {
        if (entry.start_time > last_time) {
            // There is a gap! Insert an idle block (ID -1 means idle)
            display_chart.push_back({-1, last_time, entry.start_time});
        }
        display_chart.push_back(entry);
        last_time = entry.end_time;
    }
    // ----------------------------------------------------

    // 1. Print the top border
    for (size_t i = 0; i < display_chart.size(); i++) {
        std::cout << "--------";
    }
    std::cout << "-\n";

    // 2. Print Process IDs or Dashes for idle time
    for (const auto& entry : display_chart) {
        if (entry.id == -1) {
            std::cout << "|  --\t";  // Dashes for idle CPU
        } else {
            std::cout << "|  P" << entry.id << "\t";
        }
    }
    std::cout << "|\n";

    // 3. Print the bottom border
    for (size_t i = 0; i < display_chart.size(); i++) {
        std::cout << "--------";
    }
    std::cout << "-\n";

    // 4. Print the exact time markers perfectly aligned
    std::cout << display_chart[0].start_time << "\t";
    for (const auto& entry : display_chart) {
        std::cout << entry.end_time << "\t";
    }
    std::cout << "\n\n";
}