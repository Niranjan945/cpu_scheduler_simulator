#ifndef TLB_H
#define TLB_H

#include <list>
#include <utility>

// Fully associative TLB with LRU eviction, implemented as a small ordered
// list (front = most recently used). Realistic hardware TLBs are tiny
// (typically 32-1024 entries), so a linear scan here is representative.
class TLB {
public:
    explicit TLB(int capacity);

    // Returns true on a hit and writes the mapped frame into frame_number.
    bool lookup(int page_number, int& frame_number);
    void insert(int page_number, int frame_number);
    void invalidate(int page_number);

    int getHits() const;
    int getMisses() const;
    double getHitRatio() const;
    void printStats() const;

private:
    int capacity;
    std::list<std::pair<int, int>> entries; // (page_number, frame_number)
    int hits = 0;
    int misses = 0;
};

#endif
