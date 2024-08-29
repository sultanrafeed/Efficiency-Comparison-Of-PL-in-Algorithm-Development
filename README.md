# Efficiency Comparison of Programming Languages in Algorithm Development

## Overview

This project aims to determine which programming language is the most efficient in terms of runtime efficiency, memory consumption, and CPU utilization. By implementing various algorithms with differing time complexities in different programming languages, we can compare and contrast how each language allocates resources and performs under different conditions.

## Abstract

In this project, we compared four different programming languages: **Java, C, C++,** and **Python**. We used four different algorithms to measure the performance:

1. Floyd-Warshall (All-pairs shortest path)
2. Prim’s Algorithm (Minimum spanning tree)
3. Coin Change Problem (Dynamic programming)
4. Convex Hull using Graham scan (Computational geometry)

The comparison was based on three key metrics:
- **Execution Speed**
- **Memory Consumption**
- **CPU Usage**

Our evaluation included varying input sizes to measure the scalability and performance of each language. The results showed that **C and C++** generally outperform **Java** and **Python** across all categories.

## Hardware Specification

All programs were run on a consistent hardware setup to ensure fair comparison:

- **Operating System:** Windows 10 Pro
- **Processor:** AMD Ryzen 5 3600 @ 3.60 GHz
- **RAM:** 16 GB DDR4 memory (3200 MHz Bus speed)

## Methodology

1. **Algorithm Selection:** Each selected algorithm represents a different area of computational challenges (graph theory, dynamic programming, computational geometry), providing a broad range of performance scenarios.
2. **Language Selection:** The languages chosen are widely used in both academic and industrial settings, each with its own strengths:
   - **C:** Known for its close-to-hardware efficiency and control.
   - **C++:** Offers object-oriented features along with C-like performance.
   - **Java:** Provides portability and built-in memory management.
   - **Python:** Popular for its simplicity and extensive libraries, despite being an interpreted language.
3. **Performance Metrics:** 
   - **Execution Speed:** Time taken to complete each algorithm.
   - **Memory Consumption:** Amount of RAM used during execution.
   - **CPU Usage:** Percentage of CPU resources utilized during execution.

## Results Summary

- **Execution Speed:** C and C++ exhibited the fastest execution times due to their low-level operations and efficient memory management. Java, with its Just-In-Time (JIT) compiler, performed moderately well. Python, being an interpreted language, had the slowest execution times.
- **Memory Consumption:** C++ showed slightly higher memory usage than C due to object-oriented features. Java's memory usage was higher due to its garbage collection overhead, while Python's dynamic typing resulted in significant memory usage.
- **CPU Usage:** C and C++ utilized CPU resources more efficiently. Java's CPU usage was higher due to its virtual machine, and Python's overhead led to the highest CPU usage.

## Conclusion

Our findings indicate that **C and C++** are the most efficient in terms of runtime efficiency, memory consumption, and CPU utilization. These languages are better suited for performance-critical applications. **Java** and **Python** offer other advantages such as easier syntax, garbage collection, and extensive libraries but at the cost of lower raw performance.

## Future Work

- Testing with additional programming languages such as **Go**, **Rust**, and **JavaScript**.
- Incorporating additional algorithms to cover a wider range of computational problems.
- Running tests on different hardware setups to analyze performance consistency.

## Contact

For more information, questions, or contributions, please contact [Your Name] at [Your Email].
