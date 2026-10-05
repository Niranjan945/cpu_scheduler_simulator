
<div align="center">
  
# 🖥️ OS CPU Scheduling Simulator
  
![C++](https://img.shields.io/badge/Language-C++17-blue.svg)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen.svg)
![Platform](https://img.shields.io/badge/Platform-WSL%20%2F%20Ubuntu-orange.svg)

> *A dynamic, object-oriented C++ simulator that models how an Operating System manages processes, featuring interactive inputs and dynamically generated Gantt charts.*

</div>

---

## 🎬 Demo / Preview

*Visualizing Shortest Remaining Time First (SRTF) Preemption:*

```text
--- SRTF (Preemptive SJF) Results ---
PID     Arrival Burst   Completion      Waiting Turnaround
-----------------------------------------------------------
1       0       5       11              6       11
2       1       2       3               0       2
3       2       1       4               1       2
4       4       3       7               0       3

=== GANTT CHART TIMELINE ===
-----------------------------------------
|  P1   |  P2   |  P3   |  P4   |  P1   |
-----------------------------------------
0       1       3       4       7       11

```

---

## 📖 Table of Contents

* [Why Scheduling?](https://www.google.com/search?q=%23-why-scheduling)
* [Scheduling Algorithms](https://www.google.com/search?q=%23-scheduling-algorithms)
* [Code Architecture & OOP Design](https://www.google.com/search?q=%23-code-architecture--oop-design)
* [File Structure](https://www.google.com/search?q=%23-file-structure)
* [Installation & Setup](https://www.google.com/search?q=%23-installation--setup)
* [Usage](https://www.google.com/search?q=%23-usage)

---

## 🤔 Why Scheduling?

Imagine a busy restaurant kitchen with only one chef (the **CPU**). If 50 orders (the **processes**) come in at once, the chef needs a system to decide which meal to cook first.

| Concept | Explanation |
| --- | --- |
| **The Problem** | Without scheduling, a heavy task (like a 45-min render) blocks quick 1-second tasks. If the CPU tries to do everything simultaneously without rules, it thrashes and finishes nothing. |
| **How It Helps** | Scheduling maximizes CPU utilization, minimizes user wait times, and ensures fairness so no single program freezes the entire computer. |
| **Modern OS Logic** | Modern operating systems (Windows, Linux) use complex, hybrid algorithms (like the *Completely Fair Scheduler* or *Multilevel Feedback Queues*). However, they all rely entirely on the foundational concepts simulated in this project: time-slicing, preemption, and prioritization. |

---

## ⚙️ Scheduling Algorithms

| Algorithm | Type | Core Logic |
| --- | --- | --- |
| **FCFS** | ❌ Non-Preemptive | **First Come First Serve:** Strict queue. First to arrive is first to execute. |
| **Round Robin (RR)** | ✅ Preemptive | Every process gets a strict, equal time limit (Time Quantum) on the CPU. |
| **SJF** | ❌ Non-Preemptive | **Shortest Job First:** Executes the process with the shortest total burst time. |
| **SRTF** | ✅ Preemptive | **Shortest Remaining Time:** Kicks the current process off if a shorter job arrives. |
| **Priority** | ✅ Preemptive | Executes the highest priority task (Lower number = Higher Priority). |

---

## 🏗️ Code Architecture & OOP Design

This simulator is built using strict **Object-Oriented Programming (OOP)** principles to ensure modularity and prevent state corruption during execution.

| OOP Concept | Implementation in Simulator |
| --- | --- |
| **Inheritance** | All algorithms (`FCFS`, `RR`, etc.) inherit from a master `Manager` base class. |
| **Polymorphism** | A unified `print_results(Manager* scheduler)` function safely accepts any algorithm dynamically. |
| **Encapsulation** | Process tracking variables are securely managed within the `Process` Control Block (PCB). |

### 🔗 Execution Flow (Linking `main.cpp`)

1. **Command Center (`main.cpp`):** Acts as the entry point, collecting user inputs.
2. **Umbrella Routing (`schedulers.h`):** A single master header routes the main file to the correct algorithm classes.
3. **State Protection:** The `master_list` of processes is deep-copied before every run so the user can test FCFS and SRTF back-to-back on the exact same data without data corruption.
4. **Smart Rendering (`manager.cpp`):** The base class automatically detects CPU idle time (where no processes are in the Ready Queue) and seamlessly draws `| -- |` gaps in the Gantt chart.

---

## 📂 File Structure

```text
.
├── Makefile                # Build automation (g++ compilation)
├── include/                # Header Files (.h)
│   ├── manager.h           # Base class & Gantt entry struct
│   ├── pcb.h               # Process Control Block struct
│   ├── schedulers.h        # Master umbrella header
│   └── fcfs.h, rr.h, sjf.h, sjf_p.h, psa_p.h
├── src/                    # Implementation Files (.cpp)
│   ├── manager.cpp         # Smart Gantt chart rendering
│   ├── pcb.cpp             # Constructor & state resets
│   └── fcfs.cpp, rr.cpp, sjf.cpp, sjf_p.cpp, psa_p.cpp
├── main.cpp                # Dynamic CLI & Execution engine (Root)
└── obj/                    # Compiled object files (Generated)

```

---

## 🚀 Installation & Setup

This project uses `make` for streamlined compilation and is tailored for **WSL (Windows Subsystem for Linux) running Ubuntu**.

### 1. Install Prerequisites

Ensure you have the GNU C++ compiler and Make installed on your system:

```bash
sudo apt update
sudo apt install g++ make git

```

### 2. Clone the Repository

Pull the code directly to your local machine:

```bash
git clone [https://github.com/YourUsername/OS-simulator.git](https://github.com/YourUsername/OS-simulator.git)
cd OS-simulator

```

### 3. Compile the Code

The project utilizes a `Makefile` to link the base classes and algorithm implementations. Build the project using:

```bash
make

```

---

## 💻 Usage

Launch the interactive Command Line Interface:

```bash
make run
# OR
./simulator

```

1. **Select an Algorithm:** Choose from the interactive menu (1-5).
2. **Input Data:** Define the number of processes, and provide their Arrival Time, Burst Time, and Priority.
3. **Analyze Results:** The simulator instantly calculates Completion, Waiting, and Turnaround times, followed by a precisely mapped Gantt chart timeline.

---




