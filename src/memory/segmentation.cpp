#include "memory/segmentation.h"
#include <iostream>
#include <iomanip>

void SegmentTable::addSegment(const std::string& name, int base, int limit) {
    segments.push_back({name, base, limit});
}

bool SegmentTable::translate(int segment_id, int offset, int& physical_address) const {
    if (segment_id < 0 || segment_id >= static_cast<int>(segments.size())) {
        return false;
    }
    const Segment& seg = segments[segment_id];
    if (offset < 0 || offset >= seg.limit) {
        return false; // out of bounds -> segmentation fault
    }
    physical_address = seg.base + offset;
    return true;
}

int SegmentTable::segmentCount() const {
    return static_cast<int>(segments.size());
}

void SegmentTable::printTable() const {
    std::cout << "\nSegment Table\n";
    std::cout << std::left << std::setw(6) << "ID" << std::setw(12) << "Name"
               << std::setw(10) << "Base" << std::setw(10) << "Limit" << "\n";
    for (size_t i = 0; i < segments.size(); i++) {
        std::cout << std::left << std::setw(6) << i << std::setw(12) << segments[i].name
                   << std::setw(10) << segments[i].base << std::setw(10) << segments[i].limit << "\n";
    }
}
