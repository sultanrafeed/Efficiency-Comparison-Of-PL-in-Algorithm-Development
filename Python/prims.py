from heapq import heappop, heappush
import random
import time
import matplotlib.pyplot as plt
import psutil
import csv

class Point:
    def __init__(self, x=None, y=None):
        self.x = x
        self.y = y

class Vertex:
    def __init__(self, index=None):
        self.index = index
        self.key = float('inf')
        self.visited = False

    def __lt__(self, other):
        return self.key < other.key

def dist(p1, p2):
    return ((p1.x - p2.x) ** 2 + (p1.y - p2.y) ** 2) ** 0.5

def prim(points, n):
    vertices = [Vertex(i) for i in range(n)]
    vertices[0].key = 0

    min_heap = [(0, vertices[0])]

    while min_heap:
        _, current = heappop(min_heap)
        current.visited = True

        for i in range(n):
            if not vertices[i].visited:
                weight = dist(points[current.index], points[i])
                if weight < vertices[i].key:
                    vertices[i].key = weight
                    heappush(min_heap, (weight, vertices[i]))

    # Calculate total weight and return the result
    total_weight = sum(vertex.key for vertex in vertices)
    return total_weight

# Increase input size up to 10,000
data_size = list(range(10, 510))
run_times = []
peak_memory_usage = []
cpu_utilization = []

for i in data_size:
    input_points = []
    for j in range(i):
        input_points.append((random.randint(0, 100), random.randint(0, 100)))

    points = []
    for point in input_points:
        points.append(Point(point[0], point[1]))

    # start_cpu_usage = psutil.cpu_percent()

    start = time.time()
    total_weight = prim(points, len(points))
    end = time.time()
    time_taken = round((end - start) * 1000, 2)
    run_times.append(time_taken)

    process = psutil.Process()
    peak_memory = process.memory_info().peak_wset / 1024  # Peak memory in KB
    peak_memory_usage.append(peak_memory / 1024)

    end_cpu_usage = psutil.cpu_percent()
    cpu_usage_percentage = end_cpu_usage
    cpu_utilization.append(cpu_usage_percentage)

    if i % 100 == 0:
        print("Data size:", i)
        print("Runtime:", time_taken, "milliseconds")
        print("Peak memory usage:", peak_memory, "KB")
        print("CPU utilization:", cpu_usage_percentage, "%")

# Plotting runtime
plt.figure(figsize=(10, 5))
plt.xlabel("Number of Datapoints")
plt.ylabel("Time in milliseconds")
plt.title("Time vs Number of Datapoints (Prim's Algorithm)")
plt.plot(data_size, run_times)
plt.show()

# Plotting memory consumption
plt.figure(figsize=(10, 5))
plt.xlabel("Number of Datapoints")
plt.ylabel("Memory Usage in MB")
plt.title("Memory Usage vs Number of Datapoints (Prim's Algorithm)")
plt.plot(data_size, peak_memory_usage)
plt.show()


# Plotting CPU utilization
plt.figure(figsize=(10, 5))
plt.xlabel("Number of Datapoints")
plt.ylabel("CPU Utilization (%)")
plt.title("CPU Utilization vs Number of Datapoints (Prim's Algorithm)")
plt.plot(data_size,cpu_utilization)
plt.show()

with open('../../csvs/algorithms/PrimsAlgorithm/CPU/Python_cpuUtilization_PrimsAlgorithm.csv', 'w', newline="", encoding='UTF8') as f:
    writer = csv.writer(f)

    writer.writerows(map(lambda x: [x], cpu_utilization))

with open('../../csvs/algorithms/PrimsAlgorithm/MEM/Python_peakMemoryUsage_PrimsAlgorithm.csv', 'w', newline="", encoding='UTF8') as f:
    writer = csv.writer(f)

    writer.writerows(map(lambda x: [x], peak_memory_usage))

with open('../../csvs/algorithms/PrimsAlgorithm/TIME/Python_runtimes_PrimsAlgorithm.csv', 'w', newline="", encoding='UTF8') as f:
    writer = csv.writer(f)

    writer.writerows(map(lambda x: [x], run_times))
