#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include <string>
#include <vector>

// Strategy pattern for page replacement, mirrors the Scheduler hierarchy.
// frame_table[i] holds the page number currently occupying physical frame i,
// or -1 if the frame is free.
class ReplacementAlgorithm {
public:
    virtual ~ReplacementAlgorithm() = default;

    virtual std::string getName() const = 0;

    // Called once a page is loaded into a frame, so bookkeeping (FIFO order,
    // LRU recency) can be updated. current_time is the reference index.
    virtual void onLoad(int frame_index, int page_number, int current_time) = 0;

    // Called on every access (hit or fault) so LRU can refresh recency.
    virtual void onAccess(int frame_index, int page_number, int current_time) = 0;

    // Chooses which occupied frame to evict when memory is full.
    // `reference_string` and `current_index` are only needed by Optimal,
    // which must look ahead at future references.
    virtual int selectVictim(const std::vector<int>& frame_table,
                              const std::vector<int>& reference_string,
                              int current_index) = 0;
};

class FIFOReplacement : public ReplacementAlgorithm {
public:
    std::string getName() const override;
    void onLoad(int frame_index, int page_number, int current_time) override;
    void onAccess(int frame_index, int page_number, int current_time) override;
    int selectVictim(const std::vector<int>& frame_table,
                      const std::vector<int>& reference_string,
                      int current_index) override;

private:
    std::vector<int> load_order; // frame indices in the order they were filled
};

class LRUReplacement : public ReplacementAlgorithm {
public:
    explicit LRUReplacement(int num_frames);
    std::string getName() const override;
    void onLoad(int frame_index, int page_number, int current_time) override;
    void onAccess(int frame_index, int page_number, int current_time) override;
    int selectVictim(const std::vector<int>& frame_table,
                      const std::vector<int>& reference_string,
                      int current_index) override;

private:
    std::vector<int> last_used_time; // indexed by frame
};

class OptimalReplacement : public ReplacementAlgorithm {
public:
    std::string getName() const override;
    void onLoad(int frame_index, int page_number, int current_time) override;
    void onAccess(int frame_index, int page_number, int current_time) override;
    int selectVictim(const std::vector<int>& frame_table,
                      const std::vector<int>& reference_string,
                      int current_index) override;
};

#endif
