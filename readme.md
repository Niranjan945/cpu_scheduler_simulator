# CPU Scheduler & Memory Management Simulator

A C++17 command-line simulator covering CPU scheduling algorithms and core
virtual-memory concepts (paging with TLB + replacement policies, and
segmentation). Built as a resume/interview project — no external
dependencies, no ncurses, just the standard library and a Makefile.

## Build & run

```
make          # builds ./scheduler
./scheduler   # or: make run
make clean    # removes obj/ and the binary
```

Requires g++ with C++17 support (tested with GCC on Linux).

## What's inside

### Scheduling algorithms (`include/algorithms`, `src/algorithms`)

| Algorithm | Type | File |
|---|---|---|
| FCFS | non-preemptive | `fcfs.*` |
| SJF | non-preemptive | `sjf.*` |
| SRTF | preemptive | `srtf.*` |
| Priority | non-preemptive | `priority.*` |
| Priority with aging | preemptive | `priority_aging.*` |
| Round Robin | preemptive, quantum-based | `round_robin.*` |
| MLFQ | preemptive, 3 feedback queues + aging | `mlfq.*` |

Every algorithm implements the abstract `Scheduler` base class
(`schedule()` + `getName()`) — a Strategy pattern, so `main.cpp` never
needs to know how a given algorithm works internally. The base class also
owns the shared bookkeeping: Gantt chart construction, average
waiting/turnaround/response time, CPU utilization, throughput and context
switch counting, so each algorithm file only contains the actual
scheduling logic.

Run any single algorithm, or use **"Compare all algorithms"** from the
main menu to run the same process set through every algorithm and print
a side-by-side table of average waiting/turnaround/response time — the
kind of comparison an interviewer will usually ask you to reason about.

### Memory management (`include/memory`, `src/memory`)

- **`PageTable`** — single-level page table (valid bit, frame number).
- **`TLB`** — small fully-associative TLB with LRU eviction, tracks hit/miss
  ratio independently from the page table.
- **`ReplacementAlgorithm`** — Strategy pattern again: `FIFOReplacement`,
  `LRUReplacement`, `OptimalReplacement` (Belady's algorithm, looks ahead
  in the reference string).
- **`MMU`** — drives a full reference string through TLB lookup → page
  table lookup → page fault → replacement, printing a step-by-step trace
  (which frame each page lands in) and a final summary (fault ratio, TLB
  hit ratio).
- **`SegmentTable`** — classic base+limit segmentation; translates
  `(segment_id, offset)` to a physical address or reports a segmentation
  fault on an out-of-bounds offset.

### Process input

Processes can be entered manually, or loaded from `data/processes_sample.csv`
(format: `pid,arrival_time,burst_time,priority,memory_size`).

## Design notes for interview discussion

- **Strategy pattern** used twice: `Scheduler` for CPU algorithms,
  `ReplacementAlgorithm` for page replacement. Both let `main.cpp` add a
  new algorithm without touching existing code (Open/Closed Principle).
- **Preemptive algorithms (SRTF, Priority+Aging) are simulated tick-by-tick**;
  non-preemptive ones (FCFS, SJF, Priority) jump directly from one
  process's start to its completion, since nothing can interrupt them —
  this is a deliberate efficiency/clarity tradeoff worth mentioning.
- **Round Robin and MLFQ** use an explicit ready queue and re-admit new
  arrivals before requeueing the process that was just preempted, which is
  the standard textbook convention (matters for tie-breaking).
- **Aging** is implemented in two places (Priority scheduling and MLFQ) to
  bound worst-case starvation — a common follow-up interview question on
  vanilla priority scheduling.
- **TLB is modeled as a separate cache from the page table**, so you can
  see TLB miss + page table hit (page in memory but not cached in TLB)
  versus TLB miss + page fault (page not in memory at all) as distinct
  outcomes, matching real hardware behavior.

## Possible extensions

- Multi-core scheduling (load balancing across N CPUs)
- Deadlock detection (resource allocation graph)
- Disk scheduling algorithms (FCFS/SSTF/SCAN for I/O)
- Persisting simulation runs to a file for later comparison/plotting
