# Tape Sorting Application

This project is a C++20 console application that simulates a tape storage device to sort large datasets that cannot fit entirely into RAM. It emulates tape operations using binary files and provides configurable delays to mimic the time-consuming nature of tape movements.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building the Project](#building-the-project)
  - [Running the Application](#running-the-application)
- [Configuration](#configuration)
- [Implementation Details](#implementation-details)
  - [Tape Interface](#tape-interface)
  - [FileTape Class](#filetape-class)
  - [TapeSorter Class](#tapesorter-class)
- [Testing](#testing)

## Overview

The application sorts integers stored on an input tape (represented as a `.bin` file) and writes the sorted data to an output tape. Due to memory limitations (specified by `M` bytes), the application cannot load all data into RAM at once. It uses k(2) temporary tapes stored in the `tmp` directory to facilitate sorting. The `M` parameter limits the size of the data chunk read from the tape into memory at a time, but it does not restrict the total RAM usage of the application.

## Features

- **Configurable Delays**: Simulate tape read/write and movement delays via a configuration file without recompiling the application.
- **Template Data Types**: Supports various integral data types (`char`, `short`, `int`, `long long`, etc.) using template parameters.
- **Memory Limitation**: Restricts the amount of data loaded from the tape into memory based on the specified `M` bytes.
- **Unit Testing**: Comprehensive tests using the GoogleTest framework to validate functionality across different scenarios, including edge cases.

## Optimization Philosophy
A fundamental design choice of this application is the avoidance of using a single tape for both input and output. This stems from physical considerations inspired by real-world tape drives, where the read/write head operates along a single tape. Attempting to simultaneously handle multiple operations on a single tape would be impractical and inefficient in such a system.

Additionally, the application adheres to the principle of tape integrity. Instead of destroying intermediate tapes during sorting, the program rewrites over them to preserve their continuity. This approach not only aligns with the realistic constraints of physical tape systems but also ensures robustness in handling errors or interruptions during the sorting process. By embracing these physical analogies, the application achieves a balance between simplicity, efficiency, and realism.

## Getting Started

### Prerequisites

- **C++20 Compiler**: Ensure you have a compiler that supports C++20.
- **CMake**: Version 3.5 or higher.
- **GoogleTest Framework**: Required for running unit tests.

### Building the Project

Clone the repository and navigate to the project directory:

```bash
git clone https://github.com/eagerbeaver04/operating_system.git
cd operating_system
```

Use the provided build.sh script to build the project:

```bash
bash build.sh
```

This script will generate three executable files:

 - tape_sorting: The main application for 2-way sorting.
 - k_way_tape_sorting: The main application for k-way sorting.
 - run_tests: Runs the unit tests.

### Running the Application

Execute the application with the configuration file, and memory limit `M` (1000 in example):
```
./tape_sorting config.txt 1000
```
or another sorting algorithm
```
./k_way_tape_sorting config.txt 1000
```
After that:
 - Enter path to the input tape file.
 - Enter path to the output tape file
 - Choose variant of app - to print input, sorted and output data, or not

### Configuration

Create a configuration file (e.g., config.txt) with the following four delay values in milliseconds:
 - read_delay=5
 - write_delay=5
 - rewind_delay=100
 - move_delay=0

Where: 

 - read_delay: Delay for reading an element from the tape.
 - write_delay: Delay for writing an element to the tape.
 - rewind_delay: Delay for rewinding the tape.
 - move_delay: Delay for moving the tape by one position.

## Implementation Details

### Tape Interface
The tape interface is defined in [i_tape.hpp](src/interface/i_tape.hpp). It provides the following essential methods:

```cpp
template<typename T>
class ITape {
public:
    virtual ~ITape() = default;

    virtual std::optional<T> read() = 0;
    virtual void write(const T& value) = 0;
    virtual void rewind() = 0;
    virtual void move_left() = 0;
    virtual void move_right() = 0;
    virtual void read_config(const std::string&) = 0;
    virtual bool is_eof() = 0;
};
```

### FileTape Class
The FileTape class implements the ITape interface and emulates tape operations using a binary file. It is located in [file_tape.hpp](src/tape/file_tape.hpp).

#### Features:

 - Template Parameter T: Supports various integral data types.
 - Configurable Delays: Implements delays specified in the configuration file.
 - Movement Simulation: Simulates tape movements, including rewinding and moving by positions.

### TapeSorter Class
The `TapeSorter class`, implemented in [tape_sorter.hpp](src/tape/tape_sorter.hpp), provides an efficient 2-way merge sort algorithm for sorting datasets using temporary tape storage. This class supports any implementation of the `ITape` interface, allowing seamless integration with various custom tape implementations. The algorithm uses only two temporary tapes, ensuring low resource usage and straightforward operation. Its design prioritizes memory efficiency by sorting and merging data in chunks, making it ideal for systems with limited memory resources. For more detailed information, see [tape_sorting.md](docs/tape_sorting.md).

### KWayTapeSorter Class
The `KWayTapeSorter class`, implemented in [k_way_tape_sorter.hpp](src/tape/k_way_tape_sorter.hpp), extends the functionality of the `TapeSorter` by supporting a k-way merge sort algorithm. This approach leverages multiple temporary tapes to efficiently merge larger datasets in fewer passes. Like `TapeSorter`, it is highly modular and works with any ITape implementation, enabling customization and adaptability for different storage systems. Its k-way sorting strategy significantly reduces the number of merge iterations, improving performance for large-scale datasets. For more detailed information, see [k_way_tape_sorting.md](docs/k_way_tape_sorting.md).
#### Features:

 - Memory Management: Ensures that no more than `M` bytes are loaded into memory at any time.
 - External Sorting Algorithm: Uses an efficient algorithm suitable for tape-based storage (merge sort).
 - Temporary Tapes: Stores intermediate results in the tmp directory.


### Testing
The project includes unit tests using the GoogleTest framework in [tests.cpp](tests/tests.cpp). Tests cover:

 - Different Data Types: Validates sorting with char, short, int, long long, etc.
 - Various Sequences: Tests with randomly generated sequences.
 - Edge Cases: Handles empty input files, insufficient memory (`M`).
 - Comparative Performance: Includes a comparative test of the sorting speed for different algorithms (`2-way TapeSorter` vs. `k-way KWayTapeSorter`). The results of these performance benchmarks are logged in `tests/comparison.txt`, providing detailed metrics for analysis.

Config, input and output files are automatically generated in tests folder.
### Run the tests using:

```bash
./run_tests
```

Adjust the testing parameters (e.g., sequence sizes, data types) in the test code as needed.