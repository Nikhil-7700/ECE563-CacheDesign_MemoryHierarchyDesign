# Cache and Memory Hierarchy Design Simulator

This repository implements a configurable cache and memory hierarchy simulator. The simulator supports flexible configurations of L1/L2 caches, including block size, associativity, and optional stream buffer-based prefetching.

## Project Summary

This project explores the design trade-offs in memory hierarchy configurations by simulating a wide range of cache sizes, associativities, and block sizes. It calculates performance metrics such as miss rates and Average Access Time (AAT), enabling architectural insights on optimal configurations under various workloads.

- **Course**: ECE 563 – Microprocessor Architecture
- **Project**: Cache Design and Memory Hierarchy Simulator
- **Author**: Padmanabha Nikhil Bhimavarapu
- **Language**: C++
- **Evaluation Metrics**: Miss rate, AAT, Area, Stream Buffer effectiveness

## Features

- **Fully configurable** L1/L2 caches:
  - Cache size
  - Set associativity
  - Block size
- **Write Policy**: Write-back + Write-allocate (WBWA)
- **Replacement Policy**: Least Recently Used (LRU)
- **Stream Buffer Prefetching** (for ECE 563 only)
  - Configurable N (number of stream buffers) and M (blocks per buffer)
- Supports **multiple memory hierarchy levels** and **traces**
- Outputs key metrics:
  - Read/write accesses and misses
  - Writebacks
  - Prefetch statistics
  - Final cache/stream buffer contents
  - Total memory traffic and AAT

## Running the Simulator

The simulator must be executed with **8 command-line arguments**:

```bash
./sim <BLOCKSIZE> <L1_SIZE> <L1_ASSOC> <L2_SIZE> <L2_ASSOC> <PREF_N> <PREF_M> <trace_file>



