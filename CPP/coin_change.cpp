#include <climits>
#include <iostream>
#include <chrono>
#include <bits/stdc++.h>
#include "windows.h"
#include "psapi.h"
#include <fstream>
using namespace std;
int incount=0;
int inputs[500];
double run_time[500];
double memory[500];
double cpu[500];
// Function to find the Minimum number of coins required to get Sum S
double CalculateCPULoad(const FILETIME& idleTime, const FILETIME& kernelTime, const FILETIME& userTime) {
    ULONGLONG idleTime64 = (static_cast<ULONGLONG>(idleTime.dwHighDateTime) << 32) | idleTime.dwLowDateTime;
    ULONGLONG kernelTime64 = (static_cast<ULONGLONG>(kernelTime.dwHighDateTime) << 32) | kernelTime.dwLowDateTime;
    ULONGLONG userTime64 = (static_cast<ULONGLONG>(userTime.dwHighDateTime) << 32) | userTime.dwLowDateTime;

    ULONGLONG systemTime = kernelTime64 + userTime64;
    ULONGLONG totalTime = systemTime - idleTime64;

    double cpuUsage = (totalTime > 0) ? ((systemTime - idleTime64) * 100.0 / totalTime) : 0.0;
    return cpuUsage;
}
int findMinCoins(int arr[], int n, int N) {
    //calculating execution time

    auto started = std::chrono::high_resolution_clock::now();
    // dp[i] = no of coins required to get a total of i
    int dp[N + 1];

    // 0 coins are needed for 0 sum

    dp[0] = 0;

    for (int i = 1; i <= N; i++) {
        // initialize minimum number of coins needed to infinity
        dp[i] = INT_MAX;
        int res = INT_MAX;

        // do for each coin
        for (int c = 0; c < n; c++) {
            if (i - arr[c] >=
                0)  // check if coins doesn't become negative by including it
                res = dp[i - arr[c]];

            // if total can be reached by including current coin c,
            // update minimum number of coins needed dp[i]
            if (res != INT_MAX)
                dp[i] = min(dp[i], res + 1);
        }
    }

    // The Minimum No of Coins Required for N = dp[N]
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
    return dp[N];
}

int main() {
    // No of Coins We Have
    fstream f_memory, f_run_time, f_cpu;

    f_memory.open("CPP_peakMemoryUsage_CoinChange.csv", ios::out | ios::app);
    f_run_time.open("CPP_runtimes_CoinChange.csv", ios::out | ios::app);
    f_cpu.open("CPP_cpuUtilization_CoinChange.csv", ios::out | ios::app);
    int arr[] = {1, 2, 3, 4,5};
    int n = sizeof(arr) / sizeof(arr[0]);
    int N=1000;
    // Total Change Required
    for(int i=0;i<500;i++){
        N=N+i;
        inputs[incount]=N;
        //int N = 1+rand()%10000;
        //cout<<"The value of N is:"<<N;
        //cout << "Minimum Number of Coins Required " << findMinCoins(arr, n, N)
            //<< "\n";
        findMinCoins(arr, n, N);
    }
    for(int i =0;i<500;i++){
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
