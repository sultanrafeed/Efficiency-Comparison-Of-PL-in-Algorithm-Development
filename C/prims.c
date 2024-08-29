#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>

// Number of vertices in the graph
int V = 510;

__int64 run_time[500];
double memory[500];
double cpu[500];

// A utility function to find the vertex with
// minimum key value, from the set of vertices
// not yet included in MST
int minKey(int key[], int mstSet[])
{
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (mstSet[v] == 0 && key[v] < min)
            min = key[v], min_index = v;

    return min_index;
}

// A utility function to print the
// constructed MST stored in parent[]
void printMST(int parent[], int** graph)
{
    printf("Edge \tWeight\n");
    for (int i = 1; i < V; i++)
        printf("%d - %d \t%d \n", parent[i], i, graph[i][parent[i]]);
}

// Function to construct and print MST for
// a graph represented using adjacency
// matrix representation
void primMST(int** graph)
{
    // Array to store constructed MST
    int parent[V];

    // Key values used to pick minimum weight edge in cut
    int key[V];

    // To represent set of vertices included in MST
    int mstSet[V];

    // Initialize all keys as INFINITE
    for (int i = 0; i < V; i++)
        key[i] = INT_MAX, mstSet[i] = 0;

    // Always include first 1st vertex in MST.
    // Make key 0 so that this vertex is picked as first
    // vertex.
    key[0] = 0;

    // First node is always root of MST
    parent[0] = -1;

    // The MST will have V vertices
    for (int count = 0; count < V - 1; count++) {

        // Pick the minimum key vertex from the
        // set of vertices not yet included in MST
        int u = minKey(key, mstSet);

        // Add the picked vertex to the MST Set
        mstSet[u] = 1;

        // Update key value and parent index of
        // the adjacent vertices of the picked vertex.
        // Consider only those vertices which are not
        // yet included in MST
        for (int v = 0; v < V; v++)

            // graph[u][v] is non zero only for adjacent
            // vertices of m mstSet[v] is false for vertices
            // not yet included in MST Update the key only
            // if graph[u][v] is smaller than key[v]
            if (graph[u][v] && mstSet[v] == 0
                && graph[u][v] < key[v])
                parent[v] = u, key[v] = graph[u][v];
    }

    // Print the constructed MST
    //printMST(parent, graph);
}

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
        printf("Memory usage: %.2lf MB\n", memoryMB);
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

/*
void printCPUUtilization()
{
    FILETIME idleTime, kernelTime, userTime;
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime))
    {
        ULARGE_INTEGER idleTimeValue, kernelTimeValue, userTimeValue;
        idleTimeValue.LowPart = idleTime.dwLowDateTime;
        idleTimeValue.HighPart = idleTime.dwHighDateTime;
        kernelTimeValue.LowPart = kernelTime.dwLowDateTime;
        kernelTimeValue.HighPart = kernelTime.dwHighDateTime;
        userTimeValue.LowPart = userTime.dwLowDateTime;
        userTimeValue.HighPart = userTime.dwHighDateTime;

        double cpuLoad = CalculateCPULoad(idleTimeValue, kernelTimeValue, userTimeValue);
        printf("CPU utilization: %.2lf\n", cpuLoad);
    }
    else
    {
        fprintf(stderr, "Error getting system times\n");
    }
}
*/
// Driver's code
int main()
{
    int** graph;
    double memory_output, cpuLoad;
    int count;

    for (int i = 10; i < 510; i++) {

        graph = (int**)malloc(i * sizeof(int*));
        for (int j = 0; j < i; j++) {
            graph[j] = (int*)malloc(i * sizeof(int));
            for (int k = 0; k < i; k++) {
                if (j == k) {
                    graph[j][k] = 0;
                }
                else {
                    graph[j][k] = rand() % 501;
                }
            }
        }

        V = i;

        // Start measuring time
        LARGE_INTEGER tu_freq, tu_start, tu_end;
        __int64 t_ns;

        QueryPerformanceFrequency(&tu_freq);
        QueryPerformanceCounter(&tu_start);
         //printCPUUtilization();
        primMST(graph);
        cpuLoad = printCPUUtilization();
        cpu[count] = cpuLoad;
        // Stop measuring time
        QueryPerformanceCounter(&tu_end);
        t_ns = 1000000000ULL * (tu_end.QuadPart - tu_start.QuadPart) / tu_freq.QuadPart;
        t_ns = t_ns / (double)1e+6; // Convert nanoseconds to milliseconds
        run_time[count] = t_ns;

        // Print memory usage and execution time
        memory_output = printMemoryUsage();
        memory[count] = memory_output;

        printf("Execution time: %.2f milliseconds\n", t_ns);

        // Free memory
        for (int j = 0; j < i; j++) {
            free(graph[j]);
        }
        free(graph);

        printf("%d\n", V);

        count++;
    }

    FILE *f_memory, *f_run_time, *f_cpu;

    f_memory = fopen("C_peakMemoryUsage_PrimsAlgorithm.csv", "w+");
    f_run_time = fopen("C_runtimes_PrimsAlgorithm.csv", "w+");
    f_cpu = fopen("C_cpuUtilization_PrimsAlgorithm.csv", "w+");

    for (int i = 0; i < 500; i++){
        //printf("%llu\n", run_time[i]);
        //printf("%lf\n", cpu[i]);
        printf("%lf\n", memory[i]);
        fprintf(f_memory, "%lf\n", memory[i]);
        fprintf(f_run_time, "%llu\n", run_time[i]);
        fprintf(f_cpu, "%lf\n", cpu[i]);
    }
    fclose(f_memory);
    fclose(f_run_time);
    fclose(f_cpu);

    return 0;
}
