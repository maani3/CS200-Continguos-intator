# CS200-Contingos-intator
# Programming Project 01 - C Programming (CS200/EE201)

1.  **Abstracting Contiguous Memory:**
    I implemented a custom memory management library. This library will abstract multiple, potentially non-contiguous memory allocations to make them appear and behave as a single, contiguous block of memory. It involves creating layers for record-keeping, contiguous abstraction, mapping for independent access, and a custom contiguous pointer.

## How to Run

### Part 1: Abstracting Contiguous Memory


1.  To compile all source files:
    ```bash
    make
    ```
2.  To run the test cases for each sub-part:
    *   Part 1.1 (Record Keeping - block.c):
        ```bash
        make run_block
        ```
    *   Part 1.2 (Contiguous memory abstraction - pointer.c):
        ```bash
        make run_pointer
        ```
    *   Part 1.3 (Mapping for independent access - mappings.c):
        ```bash
        make run_mappings
        ```
    *   Part 1.4 (Contiguous pointer - contiguous_pointer.c):
        ```bash
        make run_contiguous
        ```
3.  To clean up compiled files:
    ```bash
    make clean
    ```
