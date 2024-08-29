#include <iostream>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <bits/stdc++.h>
#include "windows.h"
#include "psapi.h"
#include <fstream>

using namespace std;

int incount=0;
int inputs[200];
double run_time[200];
double memory[200];
double cpu[200];

#define INF 999
int nV = 6;
#define INF 999
double CalculateCPULoad(const FILETIME& idleTime, const FILETIME& kernelTime, const FILETIME& userTime) {
    ULONGLONG idleTime64 = (static_cast<ULONGLONG>(idleTime.dwHighDateTime) << 32) | idleTime.dwLowDateTime;
    ULONGLONG kernelTime64 = (static_cast<ULONGLONG>(kernelTime.dwHighDateTime) << 32) | kernelTime.dwLowDateTime;
    ULONGLONG userTime64 = (static_cast<ULONGLONG>(userTime.dwHighDateTime) << 32) | userTime.dwLowDateTime;

    ULONGLONG systemTime = kernelTime64 + userTime64;
    ULONGLONG totalTime = systemTime - idleTime64;

    double cpuUsage = (totalTime > 0) ? ((systemTime - idleTime64) * 100.0 / totalTime) : 0.0;
    return cpuUsage;
}
void printMatrix(int** matrix, int nV);

// Implementing Floyd-Warshall algorithm
void floydWarshall(int** graph, int nV) {
    auto started = std::chrono::high_resolution_clock::now();
  int** matrix = new int*[nV];
  for (int i = 0; i < nV; i++) {
    matrix[i] = new int[nV];
    for (int j = 0; j < nV; j++)
      matrix[i][j] = graph[i][j];
  }

  // Adding vertices individually
  for (int k = 0; k < nV; k++) {
    for (int i = 0; i < nV; i++) {
      for (int j = 0; j < nV; j++) {
        if (matrix[i][k] + matrix[k][j] < matrix[i][j])
          matrix[i][j] = matrix[i][k] + matrix[k][j];
      }
    }
  }
  //printMatrix(matrix, nV);

  // Deallocate memory
  for (int i = 0; i < nV; i++)
    delete[] matrix[i];
  delete[] matrix;
  auto done = std::chrono::high_resolution_clock::now();
    double runtime=std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count();
    run_time[incount]=runtime;
        //Memory usage
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc));
    SIZE_T physicalMemoryUsed = pmc.WorkingSetSize;
    //std::cout << "Physical Memory Used: " << physicalMemoryUsed / (1024 * 1024) << " MB" << std::endl;
    memory[incount]=physicalMemoryUsed / (1024.0 * 1024.0);
    //CPU Usage
    FILETIME idleTime, kernelTime, userTime;
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime) == 0) {
        std::cerr << "Failed to get system times. Error code: " << GetLastError() << std::endl;
    }

        double cpuUsage = CalculateCPULoad(idleTime, kernelTime, userTime);
        //std::cout << "CPU Usage: " << cpuUsage << "%" << std::endl;
    cpu[incount]=cpuUsage;
    incount++;

}

void printMatrix(int** matrix, int nV) {
  for (int i = 0; i < nV; i++) {
    for (int j = 0; j < nV; j++) {
      if (matrix[i][j] == INF)
        cout << setw(4) << "INF";
      else
        cout << setw(4) << matrix[i][j];
    }
    cout << endl;
  }
}

int main() {

    fstream f_memory, f_run_time, f_cpu;

    f_memory.open("CPP_peakMemoryUsage_FloydWarshall.csv", ios::out | ios::app);
    f_run_time.open("CPP_runtimes_FloydWarshall.csv", ios::out | ios::app);
    f_cpu.open("CPP_cpuUtilization_FloydWarshall.csv", ios::out | ios::app);

    for(int i=0;i<200;i++){

    int** graph = new int*[nV];
    for (int i = 0; i < nV; i++) {
    graph[i] = new int[nV];
    for (int j = 0; j < nV; j++)
      graph[i][j] = INF;
    }
    inputs[i]=nV;
    for(int i=0;i<nV;i++){
        for(int j=0;j<nV;j++){
            graph[i][j]=rand() %10;
        }
    }
   floydWarshall(graph, nV);

  // Deallocate memory
      for (int i = 0; i < nV; i++)
      {
          delete[] graph[i];
      }
      delete[] graph;
      nV++;
      //cout<<"Here i is: "<<i;
    }

    for(int i =0;i<200;i++){
        cout << "Input: " << inputs[i] << "\n";
        cout << "Execution Time: " << fixed << setprecision(2) << run_time[i] << " ms\n";
        cout << "Memory Usage: " << fixed << setprecision(2) << memory[i] << " MB\n";
        cout << "CPU Utilization: " << fixed << setprecision(2) << cpu[i] << "%\n";
        cout << "---------------------------\n";

        f_memory << memory[i] << "\n";
        f_run_time << run_time[i] << "\n";
        f_cpu << cpu[i] << "\n";
    }

  return 0;
}

