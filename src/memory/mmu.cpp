#include "memory/mmu.h"
#include <iostream>
#include <iomanip>

MMU::MMU(int num_pages, int num_frames, std::unique_ptr<ReplacementAlgorithm> algo, int tlb_capacity)
    : page_table(num_pages),
      tlb(tlb_capacity),
      replacement(std::move(algo)),
      frame_table(num_frames, -1) {}

int MMU::findFreeFrame() const {
    for (size_t i = 0; i < frame_table.size(); i++) {
        if (frame_table[i] == -1) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void MMU::simulate(const std::vector<int>& reference_string) {
    std::cout << "\nSimulating " << reference_string.size() << " memory references using "
               << replacement->getName() << " replacement, " << frame_table.size()
               << " physical frames.\n\n";

    std::cout << std::left << std::setw(8) << "Step" << std::setw(8) << "Page"
               << std::setw(10) << "TLB" << std::setw(14) << "Page Table" << "Frames\n";

    for (size_t i = 0; i < reference_string.size(); i++) {
        int page = reference_string[i];
        stats.total_references++;

        int frame = -1;
        bool tlb_hit = tlb.lookup(page, frame);

        std::string tlb_result;
        std::string pt_result;

        if (tlb_hit) {
            stats.tlb_hits++;
            stats.page_hits++;
            tlb_result = "HIT";
            pt_result = "-";
            page_table.markReferenced(page);
            replacement->onAccess(frame, page, static_cast<int>(i));
        } else {
            stats.tlb_misses++;
            tlb_result = "MISS";

            if (page_table.isValid(page)) {
                stats.page_hits++;
                frame = page_table.getFrame(page);
                pt_result = "HIT";
                replacement->onAccess(frame, page, static_cast<int>(i));
            } else {
                stats.page_faults++;
                pt_result = "FAULT";

                frame = findFreeFrame();
                if (frame == -1) {
                    int victim_frame = replacement->selectVictim(frame_table, reference_string, static_cast<int>(i));
                    int victim_page = frame_table[victim_frame];
                    page_table.invalidatePage(victim_page);
                    tlb.invalidate(victim_page);
                    frame = victim_frame;
                }

                frame_table[frame] = page;
                page_table.mapPage(page, frame);
                replacement->onLoad(frame, page, static_cast<int>(i));
            }

            tlb.insert(page, frame);
        }

        std::cout << std::left << std::setw(8) << i << std::setw(8) << page
                   << std::setw(10) << tlb_result << std::setw(14) << pt_result << "[";
        for (size_t f = 0; f < frame_table.size(); f++) {
            std::cout << (frame_table[f] == -1 ? std::string("_") : std::to_string(frame_table[f]));
            if (f + 1 < frame_table.size()) std::cout << " ";
        }
        std::cout << "]\n";
    }

    printSummary();
}

void MMU::printSummary() const {
    std::cout << "\nMemory Access Summary\n";
    std::cout << "Total references : " << stats.total_references << "\n";
    tlb.printStats();
    std::cout << "Page faults      : " << stats.page_faults
               << " (fault ratio " << std::fixed << std::setprecision(2)
               << (stats.total_references ? (100.0 * stats.page_faults / stats.total_references) : 0.0)
               << "%)\n";
    std::cout << "Page hits        : " << stats.page_hits << "\n";
}
