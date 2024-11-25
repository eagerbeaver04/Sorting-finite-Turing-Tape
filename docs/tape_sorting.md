
# Merge Sort Algorithm for Tape Storage
The project also includes an implementation of the Merge Sort Algorithm, specifically adapted for tape-based storage devices. This algorithm optimally combines in-memory sorting and tape operations, ensuring efficient handling of datasets too large to fit into memory.

## Key Features
 - **In-Memory Sorting**: Each data chunk is sorted in memory using a recursive merge sort algorithm before being written back to the tape. This ensures the chunks are optimally prepared for merging.
 - **Chunk Merging**: The algorithm uses only two temporary tapes to merge sorted chunks incrementally, maintaining the sorted order while minimizing tape movements.
 - **Adaptive Resource Utilization**: It calculates chunk sizes based on the available memory limit (M), maximizing the efficiency of both memory and tape operations.
## Workflow
 - **Reading Data**: The input tape is read in chunks, determined by the chunk_size (M divided by the size of the data type).
 - **Sorting Chunks**: Each chunk is sorted in memory using the recursive merge_sort function, which leverages the C++ ranges library for concise and efficient operations.
 - **Saving Sorted Chunks**: Sorted chunks are sequentially written to one of the two temporary tapes.
 - **Merging Chunks**: The algorithm iteratively merges sorted chunks from the temporary tapes into larger sorted segments. Only two temporary tapes are used during this process, alternating between reading and writing, until the final sorted dataset is written to the output tape.
## Advantages
 - **Low Memory Usage**: By processing one chunk at a time, the algorithm avoids exceeding memory limits, making it suitable for environments with strict resource constraints.
 - **Minimal Temporary Storage**: Using just two temporary tapes simplifies the implementation and reduces the need for additional resources.
For further details and examples, this algorithm is implemented in the TapeSorter class, located in [tape_sorter.hpp](../src/tape/tape_sorter.hpp).