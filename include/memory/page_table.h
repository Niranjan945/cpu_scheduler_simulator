#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <vector>

struct PageTableEntry {
    int frame_number = -1;
    bool valid = false;
    bool referenced = false;
    bool dirty = false;
};

// Simple single-level page table: one entry per virtual page number.
class PageTable {
public:
    explicit PageTable(int num_pages);

    void mapPage(int page_number, int frame_number);
    void invalidatePage(int page_number);
    bool isValid(int page_number) const;
    int getFrame(int page_number) const;
    void markReferenced(int page_number);
    void printTable() const;

private:
    std::vector<PageTableEntry> entries;
};

#endif
