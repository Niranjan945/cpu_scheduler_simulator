#include "memory/page_table.h"
#include <iostream>
#include <iomanip>

PageTable::PageTable(int num_pages) : entries(num_pages) {}

void PageTable::mapPage(int page_number, int frame_number) {
    entries[page_number].frame_number = frame_number;
    entries[page_number].valid = true;
    entries[page_number].referenced = true;
}

void PageTable::invalidatePage(int page_number) {
    entries[page_number].valid = false;
    entries[page_number].frame_number = -1;
}

bool PageTable::isValid(int page_number) const {
    return entries[page_number].valid;
}

int PageTable::getFrame(int page_number) const {
    return entries[page_number].frame_number;
}

void PageTable::markReferenced(int page_number) {
    entries[page_number].referenced = true;
}

void PageTable::printTable() const {
    std::cout << "\nPage Table\n";
    std::cout << std::left << std::setw(10) << "Page" << std::setw(10) << "Valid" << std::setw(10) << "Frame" << "\n";
    for (size_t i = 0; i < entries.size(); i++) {
        if (!entries[i].valid && entries[i].frame_number == -1 && !entries[i].referenced) {
            continue; // never touched, skip for a cleaner listing
        }
        std::cout << std::left << std::setw(10) << i
                   << std::setw(10) << (entries[i].valid ? "yes" : "no")
                   << std::setw(10) << entries[i].frame_number << "\n";
    }
}
