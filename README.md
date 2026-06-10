# Low Latency Order Matching Engine

A multithreaded low-latency **Order Matching Engine** built in **Modern C++**, inspired by trading exchange infrastructure used in High Frequency Trading (HFT) systems.

The engine supports **price-time priority matching**, concurrent order ingestion, memory pooling, and stress-tested execution under high-volume order flow.

---

## Features

### Core Exchange Features

* Limit Orders
* Immediate-Or-Cancel (IOC) Orders
* Buy / Sell Matching
* Partial Fills
* Price-Time Priority (FIFO)
* Order Cancellation
* Order Modification

### Low Latency / HFT Features

* Multithreaded Order Processing
* Thread-safe Concurrent Queue
* Memory Pool Allocator
* Asynchronous Trade Logging
* Stress Testing
* Throughput Benchmarking

---

## System Architecture

```text
Client Orders
      ↓
Producer Threads
      ↓
Thread Safe Queue
      ↓
Matching Engine Thread
      ↓
Trade Execution
      ↓
Trade Logger
```

---

## Order Book Design

The engine maintains separate order books for **buy** and **sell** orders.

### Buy Book (Descending Price)

Higher bid price gets priority.

```cpp
std::map<int,
         std::deque<Order*>,
         std::greater<int>>
```

### Sell Book (Ascending Price)

Lower ask price gets priority.

```cpp
std::map<int,
         std::deque<Order*>>
```

### Why `deque`?

For **price-time priority (FIFO)**.

Orders at the same price level are executed in the order they arrive.

---

## Matching Rules

### Price-Time Priority

1. Better price executes first
2. Same price → earlier order executes first

Example:

BUY:

* Buy 10 @ 105
* Buy 5 @ 105

Incoming SELL 12 @ 105

Execution:

* First order fills completely
* Second order partially fills

---

## Order Types

### Limit Order

Executes only at specified price or better.

Example:

```text
BUY LIMIT 100 @ 105
```

---

### IOC (Immediate Or Cancel)

Executes immediately.

Remaining quantity is canceled.

Example:

```text
BUY IOC 100 @ 105
```

---

## Project Structure

```text
order-matching-engine/
│── src/
│   ├── main.cpp
│   ├── Order.h
│   ├── Trade.h
│   ├── ThreadSafeQueue.h
│   ├── MemoryPool.h
│   ├── MatchingEngine.h
│   ├── MatchingEngine.cpp
│   ├── Logger.h
│   ├── Benchmark.h
│
│── tests/
│   ├── stress_test.cpp
│
│── Makefile
│── README.md
```

---

## How to Run

### Build Engine

```bash
make
```

### Run Engine

```bash
./engine
```

### Run Stress Test

```bash
make test
make stress
```

---

## Benchmark Results

Example benchmark:

```text
Threads: 8
Orders: 80000
Execution Time: 2056 ms
Throughput: 38910 orders/sec
```

---

## Optimization Techniques

### Memory Pooling

Avoids expensive heap allocations.

Instead of:

```cpp
new/delete
```

Preallocated objects are reused.

---

### Concurrent Queue

Thread-safe producer-consumer architecture for efficient order ingestion.

---

### Lock Synchronization

Mutex-based synchronization prevents race conditions during concurrent order processing.

---

## Future Improvements

* Lock-Free Data Structures
* Order Persistence
* Market Orders
* Multi-Symbol Parallel Matching
* Latency Optimization
* Risk Management Layer
* FIX Protocol Integration

---

## Tech Stack

* C++17
* STL
* Multithreading
* Mutex Synchronization
* Concurrent Programming
* Low Latency Systems Design

---

## Motivation

This project was built to explore **low-latency systems engineering**, concurrent programming, and exchange matching systems used in **High Frequency Trading (HFT)** environments.
