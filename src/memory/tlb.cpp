#include "memory/tlb.h"
#include <iostream>

TLB::TLB(int capacity_) : capacity(capacity_) {}

bool TLB::lookup(int page_number, int& frame_number) {
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        if (it->first == page_number) {
            frame_number = it->second;
            entries.splice(entries.begin(), entries, it); // move to front (MRU)
            hits++;
            return true;
        }
    }
    misses++;
    return false;
}

void TLB::insert(int page_number, int frame_number) {
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        if (it->first == page_number) {
            entries.erase(it);
            break;
        }
    }
    entries.push_front({page_number, frame_number});
    if (static_cast<int>(entries.size()) > capacity) {
        entries.pop_back(); // evict least recently used
    }
}

void TLB::invalidate(int page_number) {
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        if (it->first == page_number) {
            entries.erase(it);
            return;
        }
    }
}

int TLB::getHits() const { return hits; }
int TLB::getMisses() const { return misses; }

double TLB::getHitRatio() const {
    int total = hits + misses;
    if (total == 0) return 0.0;
    return (static_cast<double>(hits) / total) * 100.0;
}

void TLB::printStats() const {
    std::cout << "TLB hits: " << hits << ", TLB misses: " << misses
               << ", hit ratio: " << getHitRatio() << "%\n";
}
