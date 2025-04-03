# I/O Scheduler Simulation in C++

## 🧠 Overview

This project is a simulation of various **disk I/O scheduling algorithms**, implemented in C++ as part of an Operating Systems Lab. It models how different I/O schedulers handle a sequence of I/O requests, each with a specified arrival time and track number.

The simulation includes:
- Request queue management
- Disk head movement tracking
- Scheduler-specific request selection
- Performance metrics collection

## 📦 Included Schedulers

The following scheduling algorithms are implemented:

- **FIFO (N)**: First-In-First-Out
- **SSTF (S)**: Shortest Seek Time First
- **LOOK (L)**: Elevator algorithm (non-circular)
- **CLOOK (C)**: Circular LOOK
- **FLOOK (F)**: Modified LOOK using active/add queues

## 📁 Project Structure

```
Lab4_Submission/
├── Lab4.cpp         # Main simulator program
├── makefile         # Compilation script
├── runit.sh         # Automation script to run all schedulers
├── gradeit.sh       # Script to compare outputs with reference
├── Inputs/          # Input files (input0 ... input9)
├── refout/          # Reference output files for comparison
├── make.log         # Log from compilation
├── gradeit.log      # Output from gradeit.sh
```

---

## 🛠️ Building the Project

To compile the program, run:

```bash
make
```

This will compile `Lab4.cpp` and produce the executable (typically `Lab4` or `a.out`).

---

## 🚀 Running the Simulation

You can test all five schedulers at once using:

```bash
./runit.sh <output-directory> ./Lab4
```

### Example:

```bash
mkdir myout
./runit.sh myout ./Lab4
```

This script:
- Runs the simulation using each scheduling algorithm (N, S, L, C, F)
- Feeds it input files from `Inputs/`
- Writes corresponding output files into `<output-directory>`

Each output file is named like: `out_<input-number>_<scheduler>`

---

## ✅ Grading Your Output

To compare your results with the reference outputs:

```bash
./gradeit.sh refout myout
```

This script:
- Compares each generated output file with the corresponding reference
- Prints a grid showing pass/fail (`.` for pass, `x` for diff, `o` for missing)
- Outputs a summary with total matches

A log is saved to `myout/LOG.txt`.

---

## 📊 Sample Output (Summary)

```
     N  S  L  C  F
0    .  .  x  .  .
1    .  .  .  .  .
...
SUM  9 10 8 10 10
TOTAL: 47 out of 50
```

---

## 🧪 Sample Command for One Input

To run a single test manually:

```bash
./Lab4 -sS Inputs/input3
```

This runs SSTF (`-sS`) on `input3`.

---

## 📌 Notes

- All input files are in the `Inputs/` directory.
- You can modify/add more test cases and reference outputs.
- The code prints both per-request and summary statistics.

---

## 👨‍💻 Author

Varad Suryavanshi  
