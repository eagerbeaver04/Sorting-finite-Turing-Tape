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

The application sorts integers stored on an input tape (represented as a `.bin` file) and writes the sorted data to an output tape. Due to memory limitations (specified by `M` bytes), the application cannot load all data into RAM at once. It uses 2 temporary tapes stored in the `tmp` directory to facilitate sorting. The `M` parameter limits the size of the data chunk read from the tape into memory at a time, but it does not restrict the total RAM usage of the application.

## Features

- **Configurable Delays**: Simulate tape read/write and movement delays via a configuration file without recompiling the application.
- **Template Data Types**: Supports various integral data types (`char`, `short`, `int`, `long long`, etc.) using template parameters.
- **Memory Limitation**: Restricts the amount of data loaded from the tape into memory based on the specified `M` bytes.
- **Unit Testing**: Comprehensive tests using the GoogleTest framework to validate functionality across different scenarios, including edge cases.

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

This script will generate two executable files:

 - tape_sorting: The main application.
 - run_tests: Runs the unit tests.

### Running the Application

Execute the application with the configuration file, and memory limit `M` (1000 in example):
```
./tape_sorting config.txt 1000
```
After that:
 - Enter path to the input tape file.
 - Enter path to the output tape file

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
The TapeSorter class, found in [tape_sorter.hpp](src/tape/tape_sorter.hpp), handles the sorting algorithm using temporary tapes. It takes as template any implementation of `ITape` for more to facilitate integration with custom tape implementations.

#### Features:

 - Memory Management: Ensures that no more than `M` bytes are loaded into memory at any time.
 - External Sorting Algorithm: Uses an efficient algorithm suitable for tape-based storage (merge sort).
 - Temporary Tapes: Stores intermediate results in the tmp directory.


### Testing
The project includes 48 unit tests using the GoogleTest framework in [tests.cpp](tests/tests.cpp). Tests cover:

 - Different Data Types: Validates sorting with char, short, int, long long, etc.
 - Various Sequences: Tests with randomly generated sequences.
 - Edge Cases: Handles empty input files, insufficient memory (`M`).

Config, input and output files are automatically generated in tests folder.
### Run the tests using:

```bash
./run_tests
```

Adjust the testing parameters (e.g., sequence sizes, data types) in the test code as needed.