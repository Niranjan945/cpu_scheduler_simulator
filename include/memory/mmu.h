#ifndef MMU_H
#define MMU_H

#include <vector>
#include <memory>
#include "memory/page_table.h"
#include "memory/tlb.h"
#include "memory/replacement.h"

struct MemoryStats {
    int total_references = 0;
    int tlb_hits = 0;
    int tlb_misses = 0;
    int page_faults = 0;
    int page_hits = 0;
};

// Drives a full virtual-memory access simulation over a page reference
// string: TLB lookup -> page table lookup -> page fault + replacement.
class MMU {
public:
    MMU(int num_pages, int num_frames, std::unique_ptr<ReplacementAlgorithm> algo, int tlb_capacity = 4);

    // Runs the whole reference string and prints a step-by-step trace.
    void simulate(const std::vector<int>& reference_string);

    void printSummary() const;

private:
    PageTable page_table;
    TLB tlb;
    std::unique_ptr<ReplacementAlgorithm> replacement;
    std::vector<int> frame_table; // frame_table[f] = page number in frame f, or -1
    MemoryStats stats;

    int findFreeFrame() const;
};

#endif
