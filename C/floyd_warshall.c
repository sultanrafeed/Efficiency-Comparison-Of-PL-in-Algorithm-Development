// Floyd-Warshall Algorithm in C
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>

int incount=0;
int inputs[200];
__int64 run_time[200];
double memory[200];
double cpu[200];

// defining the number of vertices

#define INF 999
int nV=2;
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



void printMatrix(int matrix[][nV]);
// Implementing floyd warshall algorithm


void floydWarshall(int graph[][nV]) {
    double memory_output, cpuLoad;
    // Start measuring time
    LARGE_INTEGER tu_freq, tu_start, tu_end;
    __int64 t_ns;
    QueryPerformanceFrequency(&tu_freq);
    QueryPerformanceCounter(&tu_start);
  int matrix[nV][nV], i, j, k;

  for (i = 0; i < nV; i++)
    for (j = 0; j < nV; j++)
      matrix[i][j] = graph[i][j];

  // Adding vertices individually
  for (k = 0; k < nV; k++) {
    for (i = 0; i < nV; i++) {
      for (j = 0; j < nV; j++) {
        if (matrix[i][k] + matrix[k][j] < matrix[i][j])
          matrix[i][j] = matrix[i][k] + matrix[k][j];
      }
    }
  }

        cpuLoad = printCPUUtilization();
        cpu[incount] = cpuLoad;
        // Stop measuring time
        QueryPerformanceCounter(&tu_end);
        t_ns = 1000000000ULL * (tu_end.QuadPart - tu_start.QuadPart) / tu_freq.QuadPart;
        t_ns=t_ns / (double)1e+6;
        //time_spent = t_ns / (double)1e+6; // Convert nanoseconds to milliseconds
        run_time[incount] = t_ns;

        // Print memory usage and execution time
        memory_output = printMemoryUsage();
        memory[incount] = memory_output;
        incount++;
        printf("Execution time: %.2f milliseconds\n", t_ns);

  //printMatrix(matrix);
}

void printMatrix(int matrix[][nV]) {
  for (int i = 0; i < nV; i++) {
    for (int j = 0; j < nV; j++) {
      if (matrix[i][j] == INF)
        printf("%4s", "INF");
      else
        printf("%4d", matrix[i][j]);
    }
    printf("\n");
  }
}



int main() {
    FILE *f_memory, *f_run_time, *f_cpu;

    f_memory = fopen("C_peakMemoryUsage_FloydWarshall.csv", "w+");
    f_run_time = fopen("C_runtimes_FloydWarshall.csv", "w+");
    f_cpu = fopen("C_cpuUtilization_FloydWarshall.csv", "w+");


   for(int i=0;i<200;i++){
        inputs[i]=nV;
        int graph[nV][nV];
            for(int i=0;i<nV;i++){
                for(int j=0;j<nV;j++){
                    graph[i][j]=rand() %10;

            }
        }
    floydWarshall(graph);
    nV++;


    }

    for (int i = 0; i < 200; i++){
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
