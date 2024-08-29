#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>

int incount=0;
int inputs[500];
__int64 run_time[500];
double memory[500];
double cpu[500];

double printMemoryUsage() {
    // Get handle to current process
    HANDLE hProcHandle = GetCurrentProcess();

    // Create memory structure to hold memory information
    PROCESS_MEMORY_COUNTERS_EX memory;
    memory.cb = sizeof(memory);

    double memoryMB;
    // Get process memory information
    if (GetProcessMemoryInfo(hProcHandle, (PROCESS_MEMORY_COUNTERS_EX*)&memory, sizeof(memory))) {
        // Convert memory usage to MB
        memoryMB = (double)memory.PrivateUsage / 1048576.0;

        // Print memory usage to console
        printf("Memory usage: %.2f MB\n", memoryMB);
    }
    else {
        // Handle error
        fprintf(stderr, "Error getting process memory information\n");
    }

    return memoryMB;
}

double CalculateCPULoad(ULARGE_INTEGER idleTime, ULARGE_INTEGER kernelTime, ULARGE_INTEGER userTime)
{
    ULONGLONG idle = idleTime.QuadPart;
    ULONGLONG kernel = kernelTime.QuadPart;
    ULONGLONG user = userTime.QuadPart;

    ULONGLONG system = kernel + user;
    ULONGLONG totalTime = system + idle;

    static ULONGLONG previousTotalTime = 0;
    static ULONGLONG previousIdleTime = 0;

    ULONGLONG deltaTime = totalTime - previousTotalTime;
    ULONGLONG idleDeltaTime = idle - previousIdleTime;

    previousTotalTime = totalTime;
    previousIdleTime = idle;

    double cpuLoad = 0.0;
    if ((double) deltaTime != 0) {
        //cpuLoad = (double)(system - previousTotalTime) / deltaTime * 100.0;
        cpuLoad = 100.0 - ((double)idleDeltaTime / (double)deltaTime) * 100.0;
    }

    return cpuLoad;

    /*
    double cpuLoad = 100.0 - ((double)idleDeltaTime / (double)deltaTime) * 100.0;
    printf("%lf\n", (double)idleDeltaTime);
    printf("%lf\n", (double)deltaTime);
    return cpuLoad;
    */
}

double printCPUUtilization()
{
    FILETIME idleTime, kernelTime, userTime;
    double cpuLoad;

    if (GetSystemTimes(&idleTime, &kernelTime, &userTime))
    {
        ULARGE_INTEGER idleTimeValue, kernelTimeValue, userTimeValue;
        idleTimeValue.LowPart = idleTime.dwLowDateTime;
        idleTimeValue.HighPart = idleTime.dwHighDateTime;
        kernelTimeValue.LowPart = kernelTime.dwLowDateTime;
        kernelTimeValue.HighPart = kernelTime.dwHighDateTime;
        userTimeValue.LowPart = userTime.dwLowDateTime;
        userTimeValue.HighPart = userTime.dwHighDateTime;

        cpuLoad = CalculateCPULoad(idleTimeValue, kernelTimeValue, userTimeValue);

        if (!isnan(cpuLoad))
        {
            printf("CPU utilization: %.2lf%%\n", cpuLoad);
        }
        else
        {
            fprintf(stderr, "Error: Invalid CPU load value\n");
        }
    }
    else
    {
        fprintf(stderr, "Error getting system times\n");
    }

    return cpuLoad;
}


int findMinCoins(int arr[], int n, int N) {
    double memory_output, cpuLoad;
    // Start measuring time
    LARGE_INTEGER tu_freq, tu_start, tu_end;
    __int64 t_ns;
    QueryPerformanceFrequency(&tu_freq);
    QueryPerformanceCounter(&tu_start);
         //printCPUUtilization();
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
    cpuLoad = printCPUUtilization();
        cpu[incount] = cpuLoad;
        // Stop measuring time
        QueryPerformanceCounter(&tu_end);
        t_ns = 1000000000ULL * (tu_end.QuadPart - tu_start.QuadPart) / tu_freq.QuadPart;
        t_ns = t_ns / (double)1e+6;
        //time_spent = t_ns / (double)1e+6; // Convert nanoseconds to milliseconds
        run_time[incount] = t_ns;

        // Print memory usage and execution time
        memory_output = printMemoryUsage();
        memory[incount] = memory_output;
        incount++;
        //printf("Execution time: %.2f milliseconds\n", t_ns);
        return dp[N];
}

int main() {
    // No of Coins We Have
    FILE *f_memory, *f_run_time, *f_cpu;

    f_memory = fopen("C_peakMemoryUsage_CoinChange.csv", "w+");
    f_run_time = fopen("C_runtimes_CoinChange.csv", "w+");
    f_cpu = fopen("C_cpuUtilization_CoinChange.csv", "w+");

    int arr[] = {1, 2, 3, 4,5};
    int n = sizeof(arr) / sizeof(arr[0]);
    int N=1000;
    // Total Change Required
    for(int i=0;i<500;i++){
        N=N+i;
        int a=findMinCoins(arr, n, N);
        printf("%d \n",a);
        inputs[i]=N;
    }
    for (int i = 0; i < 500; i++){
        //printf("%llu\n", run_time[i]);
        //printf("%lf\n", cpu[i]);
        //printf("%lf\n", memory[i]);
        fprintf(f_memory, "%lf\n", memory[i]);
        fprintf(f_run_time, "%llu\n", run_time[i]);
        fprintf(f_cpu, "%lf\n", cpu[i]);
    }
    fclose(f_memory);
    fclose(f_run_time);
    fclose(f_cpu);
    return 0;
}


