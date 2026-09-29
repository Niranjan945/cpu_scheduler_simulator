#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <string>
#include <vector>

struct Segment {
    std::string name;
    int base;
    int limit; // size of the segment in bytes/words
};

// Classic base+limit segmentation: a logical address is (segment_id, offset).
// Translation succeeds only if 0 <= offset < limit, otherwise it's a
// protection fault (segmentation fault).
class SegmentTable {
public:
    void addSegment(const std::string& name, int base, int limit);

    // Returns true and writes the physical address on success.
    bool translate(int segment_id, int offset, int& physical_address) const;

    int segmentCount() const;
    void printTable() const;

private:
    std::vector<Segment> segments;
};

#endif
