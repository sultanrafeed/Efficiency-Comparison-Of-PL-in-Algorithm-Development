#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <limits.h>
#include <time.h>
#include <psapi.h>

__int64 run_time[500];
double memory[500];
double cpu[500];

typedef struct tPoint {
    int x, y;
} Point;

bool ccw(const Point *a, const Point *b, const Point *c) {
    return (b->x - a->x) * (c->y - a->y)
         > (b->y - a->y) * (c->x - a->x);
}

int comparePoints(const void *lhs, const void *rhs) {
    const Point* lp = lhs;
    const Point* rp = rhs;
    if (lp->x < rp->x)
        return -1;
    if (rp->x < lp->x)
        return 1;
    if (lp->y < rp->y)
        return -1;
    if (rp->y < lp->y)
        return 1;
    return 0;
}

void fatal(const char* message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

void* xmalloc(size_t n) {
    void* ptr = malloc(n);
    if (ptr == NULL)
        fatal("Out of memory");
    return ptr;
}

void* xrealloc(void* p, size_t n) {
    void* ptr = realloc(p, n);
    if (ptr == NULL)
        fatal("Out of memory");
    return ptr;
}

void printPoints(const Point* points, int len) {
    printf("[");
    if (len > 0) {
        const Point* ptr = points;
        const Point* end = points + len;
        printf("(%d, %d)", ptr->x, ptr->y);
        ++ptr;
        for (; ptr < end; ++ptr)
            printf(", (%d, %d)", ptr->x, ptr->y);
    }
    printf("]");
}

Point* convexHull(Point p[], int len, int* hsize) {
    if (len == 0) {
        *hsize = 0;
        return NULL;
    }

    int i, size = 0, capacity = 4;
    Point* hull = xmalloc(capacity * sizeof(Point));

    qsort(p, len, sizeof(Point), comparePoints);

    /* lower hull */
    for (i = 0; i < len; ++i) {
        while (size >= 2 && !ccw(&hull[size - 2], &hull[size - 1], &p[i]))
            --size;
        if (size == capacity) {
            capacity *= 2;
            hull = xrealloc(hull, capacity * sizeof(Point));
        }
        assert(size >= 0 && size < capacity);
        hull[size++] = p[i];
    }

    /* upper hull */
    int t = size + 1;
    for (i = len - 1; i >= 0; i--) {
        while (size >= t && !ccw(&hull[size - 2], &hull[size - 1], &p[i]))
            --size;
        if (size == capacity) {
            capacity *= 2;
            hull = xrealloc(hull, capacity * sizeof(Point));
        }
        assert(size >= 0 && size < capacity);
        hull[size++] = p[i];
    }
    --size;
    assert(size >= 0);
    hull = xrealloc(hull, size * sizeof(Point));
    *hsize = size;
    return hull;
}

double printMemoryUsage() {
    // Get handle to current process
    HANDLE hProcHandle = GetCurrentProcess();

    // Create memory structure to hold memory information
    PROCESS_MEMORY_COUNTERS_EX memory;
    memory.cb = sizeof(memory);

    double usage;

    // Get process memory information
    if (GetProcessMemoryInfo(hProcHandle, (PROCESS_MEMORY_COUNTERS_EX*)&memory, sizeof(memory))) {
        // Print memory usage to console
        usage = (double)(memory.PrivateUsage / (1048576.0));
        // printf("Memory usage: %lf KB\n", usage);
    }
    else {
        // Handle error
        fprintf(stderr, "Error getting process memory information\n");
    }

    return usage;
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
        cpuLoad = 100.0 - ((double)idleDeltaTime / (double)deltaTime) * 100.0;
    }

    return cpuLoad;
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
            // printf("CPU utilization: %.2lf%%\n", cpuLoad);
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

int main() {
    Point* points;
    Point* hull;
    int num_of_itrations = 505;
    int count = 0;
    double memory_usage, cpu_load;

    LARGE_INTEGER tu_freq, tu_start, tu_end;
    __int64 t_ns;

    for (int i = 5; i < num_of_itrations; i++){
        points = (Point*)malloc(i * sizeof(Point));

        for (int j = 0; j < i; j++){
            Point point = {rand() % 101, rand() % 101};
            points[j] = point;
        }

        int hsize;

        QueryPerformanceFrequency(&tu_freq);

        QueryPerformanceCounter(&tu_start);
        hull = convexHull(points, sizeof(points)/sizeof(Point), &hsize);
        QueryPerformanceCounter(&tu_end);

        cpu_load = printCPUUtilization();
        cpu[count] = cpu_load;

        memory_usage = printMemoryUsage();
        memory[count] = memory_usage;

        run_time[count] = 1000000000ULL * (tu_end.QuadPart - tu_start.QuadPart) / tu_freq.QuadPart;

        free(hull);
        free(points);

        count++;
    }

    FILE *f_memory, *f_run_time, *f_cpu;

    f_memory = fopen("C_peakMemoryUsage_ConvexHull.csv", "w+");
    f_run_time = fopen("C_runtimes_ConvexHull.csv", "w+");
    f_cpu = fopen("C_cpuUtilization_ConvexHull.csv", "w+");

    for (int i = 0; i < 500; i++){
        //printf("%llu\n", run_time[i]);
        printf("%lf\n", cpu[i]);
        //printf("%lf\n", memory[i]);
        fprintf(f_memory, "%lf\n", memory[i]);
        fprintf(f_run_time, "%llu\n", run_time[i]);
        fprintf(f_cpu, "%lf\n", cpu[i]);
    }

    printf("%d", count);
}
