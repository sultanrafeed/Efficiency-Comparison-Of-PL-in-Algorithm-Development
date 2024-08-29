#include <iostream>
#include <iomanip>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <fstream>

using namespace std;

int inputs[500];
__int64 run_time[500];
double memory[500];
double cpu[500];

double CalculateCPULoad(const FILETIME& idleTime, const FILETIME& kernelTime, const FILETIME& userTime) {
    ULONGLONG idleTime64 = (static_cast<ULONGLONG>(idleTime.dwHighDateTime) << 32) | idleTime.dwLowDateTime;
    ULONGLONG kernelTime64 = (static_cast<ULONGLONG>(kernelTime.dwHighDateTime) << 32) | kernelTime.dwLowDateTime;
    ULONGLONG userTime64 = (static_cast<ULONGLONG>(userTime.dwHighDateTime) << 32) | userTime.dwLowDateTime;

    ULONGLONG systemTime = kernelTime64 + userTime64;
    ULONGLONG totalTime = systemTime - idleTime64;

    double cpuUsage = (totalTime > 0) ? ((systemTime - idleTime64) * 100.0 / totalTime) : 0.0;
    return cpuUsage;
}

int minKey(int key[], bool mstSet[], int V) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++) {
        if (mstSet[v] == false && key[v] < min) {
            min = key[v];
            min_index = v;
        }
    }
    return min_index;
}

void primMST(int** graph, int V) {
    int parent[V];
    int key[V];
    bool mstSet[V];

    for (int i = 0; i < V; i++)
        key[i] = INT_MAX, mstSet[i] = false;

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V - 1; count++) {
        int u = minKey(key, mstSet, V);
        mstSet[u] = true;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v])
                parent[v] = u, key[v] = graph[u][v];
        }
    }
}

int main() {
    int incount = 0;

    fstream f_memory, f_run_time, f_cpu;

    f_memory.open("CPP_peakMemoryUsage_PrimsAlgorithm.csv", ios::out | ios::app);
    f_run_time.open("CPP_runtimes_PrimsAlgorithm.csv", ios::out | ios::app);
    f_cpu.open("CPP_cpuUtilization_PrimsAlgorithm.csv", ios::out | ios::app);

    for (int i = 10; i < 510; i++) {
        int V = i;
        int** graph = new int*[V];
        for (int j = 0; j < V; j++) {
            graph[j] = new int[V];
            for (int k = 0; k < V; k++) {
                if (j == k) {
                    graph[j][k] = 0;
                }
                else {
                    graph[j][k] = rand() % 501;
                }
            }
        }

        // Measure CPU utilization, execution time, and memory usage
        FILETIME idleTime, kernelTime, userTime;
        if (GetSystemTimes(&idleTime, &kernelTime, &userTime) == 0) {
            cerr << "Failed to get system times. Error code: " << GetLastError() << endl;
            return 1;
        }

        // Start execution time measurement
        auto start = chrono::high_resolution_clock::now();

        // Execute the algorithm
        primMST(graph, V);

        // End execution time measurement
        auto end = chrono::high_resolution_clock::now();
        double runtime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        run_time[incount] = runtime;

        // Memory usage
        PROCESS_MEMORY_COUNTERS_EX pmc;
        if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc))) {
            SIZE_T physicalMemoryUsed = pmc.WorkingSetSize;
            memory[incount] = physicalMemoryUsed / (1024.0 * 1024.0);
        }
        else {
            cerr << "Failed to get process memory info. Error code: " << GetLastError() << endl;
            return 1;
        }

        // CPU usage
        if (GetSystemTimes(&idleTime, &kernelTime, &userTime) == 0) {
            cerr << "Failed to get system times. Error code: " << GetLastError() << endl;
            return 1;
        }
        else {
            double cpuUsage = CalculateCPULoad(idleTime, kernelTime, userTime);
            cpu[incount] = cpuUsage;
        }

        // Store input
        inputs[incount] = V;

        // Free memory
        for (int j = 0; j < V; j++) {
            delete[] graph[j];
        }
        delete[] graph;

        incount++;
    }

    // Print the stored values
    for (int i = 0; i < incount; i++) {
        cout << "Input: " << inputs[i] << "\n";
        cout << "Execution Time: " << fixed << setprecision(2) << run_time[i] << " ms\n";
        cout << "Memory Usage: " << fixed << setprecision(2) << memory[i] << " MB\n";
        cout << "CPU Utilization: " << fixed << setprecision(2) << cpu[i] << "%\n";
        cout << "---------------------------\n";

        f_memory << memory[i] << "\n";
        f_run_time << run_time[i] << "\n";
        f_cpu << cpu[i] << "\n";
    }
    cout << incount;
    return 0;
}
