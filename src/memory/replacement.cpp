#include "memory/replacement.h"
#include <algorithm>
#include <limits>

// ---------------- FIFO ----------------

std::string FIFOReplacement::getName() const {
    return "FIFO";
}

void FIFOReplacement::onLoad(int frame_index, int /*page_number*/, int /*current_time*/) {
    // A frame being reused (after an eviction) must lose its old queue slot.
    load_order.erase(std::remove(load_order.begin(), load_order.end(), frame_index), load_order.end());
    load_order.push_back(frame_index);
}

void FIFOReplacement::onAccess(int /*frame_index*/, int /*page_number*/, int /*current_time*/) {
    // FIFO ignores accesses after the initial load; age is purely load order.
}

int FIFOReplacement::selectVictim(const std::vector<int>& /*frame_table*/,
                                   const std::vector<int>& /*reference_string*/,
                                   int /*current_index*/) {
    int victim = load_order.front();
    load_order.erase(load_order.begin());
    return victim;
}

// ---------------- LRU ----------------

LRUReplacement::LRUReplacement(int num_frames) : last_used_time(num_frames, -1) {}

std::string LRUReplacement::getName() const {
    return "LRU";
}

void LRUReplacement::onLoad(int frame_index, int /*page_number*/, int current_time) {
    last_used_time[frame_index] = current_time;
}

void LRUReplacement::onAccess(int frame_index, int /*page_number*/, int current_time) {
    last_used_time[frame_index] = current_time;
}

int LRUReplacement::selectVictim(const std::vector<int>& frame_table,
                                  const std::vector<int>& /*reference_string*/,
                                  int /*current_index*/) {
    int victim = -1;
    int oldest = std::numeric_limits<int>::max();
    for (size_t i = 0; i < frame_table.size(); i++) {
        if (frame_table[i] == -1) continue;
        if (last_used_time[i] < oldest) {
            oldest = last_used_time[i];
            victim = static_cast<int>(i);
        }
    }
    return victim;
}

// ---------------- Optimal (Belady's algorithm) ----------------

std::string OptimalReplacement::getName() const {
    return "Optimal (Belady)";
}

void OptimalReplacement::onLoad(int, int, int) {}
void OptimalReplacement::onAccess(int, int, int) {}

int OptimalReplacement::selectVictim(const std::vector<int>& frame_table,
                                      const std::vector<int>& reference_string,
                                      int current_index) {
    int victim = -1;
    int farthest_use = -1;

    for (size_t i = 0; i < frame_table.size(); i++) {
        int page = frame_table[i];
        if (page == -1) continue;

        int next_use = std::numeric_limits<int>::max();
        for (size_t j = current_index + 1; j < reference_string.size(); j++) {
            if (reference_string[j] == page) {
                next_use = static_cast<int>(j);
                break;
            }
        }

        if (next_use > farthest_use) {
            farthest_use = next_use;
            victim = static_cast<int>(i);
        }
    }
    return victim;
}
