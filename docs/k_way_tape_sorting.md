
# K-Way Merge Sort Algorithm
The implementation now includes the K-Way Merge Sort Algorithm, a versatile sorting method that efficiently handles large datasets using multiple temporary tapes. The algorithm divides the input data into manageable chunks, sorts them individually, and then merges these chunks using a priority queue for optimal performance.

## Features of K-Way Merge Sort:
- **Configurable Number of Temporary Tapes (k)**: The algorithm uses k+2 tapes, including one for input and one for output, to manage intermediate sorted data and streamline the merge process.
 - **Chunk-Based Sorting**: Data is processed in chunks of size chunk_size (calculated as M / sizeof(N)), ensuring the memory usage adheres to the limit M.
 - **Priority Queue for Merging**: A priority queue ensures efficient merging of sorted chunks by always selecting the smallest available element across all active tapes.
 - **Alternating Temporary Tapes**: During multi-stage merges, the algorithm alternates between temporary tapes to optimize data flow and minimize tape rewinds.

## Workflow:
### Chunk Creation:

The input data is divided into chunks, each of size chunk_size, which are sorted and stored in temporary tapes.
If the input size is not an exact multiple of chunk_size, the last chunk may be smaller.
### Merging Chunks:

Sorted chunks are merged in stages using a priority queue.
Each stage reduces the number of active tapes until the final output is written to the designated output tape.
### Error Handling:

The algorithm validates file existence and ensures the memory limit (M) is sufficient for at least one element of the specified type N.
Exceptions are thrown for invalid configurations, missing files, or insufficient memory.
### Code Location:
The implementation of this algorithm can be found in the KWayTapeSorter class, located in the [k_way_tape_sorter.hpp](../src/tape/k_way_tape_sorter.hpp) file.

## Advantages:
 - **Scalability** : By configuring the number of temporary tapes (k), the algorithm balances performance and resource utilization.
 - **Memory Efficiency**: The use of chunks ensures that memory usage stays within the specified limit.
Realistic Tape Handling: Temporary tapes are reused, rewound, and alternated to simulate the behavior of real tape storage systems.

This addition complements the existing TapeSorter class by offering an alternative method for sorting large datasets, particularly useful for scenarios requiring fine-grained control over resource allocation.