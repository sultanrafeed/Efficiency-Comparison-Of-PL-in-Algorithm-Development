import random
import time
import psutil
import csv

#from time import perf_counter

def _print_dist(dist, v):
    print("\nThe shortest path matrix using Floyd Warshall algorithm\n")
    for i in range(v):
        for j in range(v):
            if dist[i][j] != float("inf"):
                print(int(dist[i][j]), end="\t")
            else:
                print("INF", end="\t")
        print()


def floyd_warshall(graph, v):

    start = time.time()
    # start_cpu_usage = psutil.cpu_percent()

    dist = [[float("inf") for _ in range(v)] for _ in range(v)]

    for i in range(v):
        for j in range(v):
            dist[i][j] = graph[i][j]

            # check vertex k against all other vertices (i, j)
    for k in range(v):
        # looping through rows of graph array
        for i in range(v):
            # looping through columns of graph array
            for j in range(v):
                if (
                    dist[i][k] != float("inf")
                    and dist[k][j] != float("inf")
                    and dist[i][k] + dist[k][j] < dist[i][j]
                ):
                    dist[i][j] = dist[i][k] + dist[k][j]

    end = time.time()
    time_taken = round((end-start)*1000)
    #print(f"Time taken:{time_taken} milliseconds ")
    process = psutil.Process()
    peak_memory = process.memory_info().peak_wset / (1024 * 1024)  # Peak memory in MB

    end_cpu_usage = psutil.cpu_percent()
    # Calculate the CPU usage percentage difference
    cpu_usage_percentage = end_cpu_usage
    print(f"CPU usage: {cpu_usage_percentage}%")

    #print(f"Peak memory usage: {peak_memory}KB")
    #_print_dist(dist, v)
    return dist, v,time_taken, peak_memory, cpu_usage_percentage


if __name__ == "__main__":
    it= 200
    runtime= []
    memory= []
    ver=[]
    cpu= []

    for i in range(it):
        v = 2 + i
        #e=random.randint(1,v*(v-1))
        e=v*(v-1)
        graph = [[float("inf") for i in range(v)] for j in range(v)]

        for i in range(v):
            graph[i][i] = 0.0

            # src and dst are indices that must be within the array size graph[e][v]
            # failure to follow this will result in an error
        for i in range(e):
        #print("\nEdge ", i + 1)
            src = random.randint(0, v-1)
            dst = random.randint(0, v-1)
            weight = random.uniform(0.0, 10.0)
        while src == dst:
                dst = random.randint(0, v-1)
        graph[src][dst] = weight

        dist,v,t,p,c= floyd_warshall(graph, v)
        runtime.append(t)
        memory.append(p)
        ver.append(v)
        cpu.append(c)

    with open('../../csvs/algorithms/FloydWarshall/CPU/Python_cpuUtilization_FloydWarshall.csv', 'w', newline="",
              encoding='UTF8') as f:
        writer = csv.writer(f)

        writer.writerows(map(lambda x: [x], cpu))

    with open('../../csvs/algorithms/FloydWarshall/MEM/Python_peakMemoryUsage_FloydWarshall.csv', 'w', newline="",
              encoding='UTF8') as f:
        writer = csv.writer(f)

        writer.writerows(map(lambda x: [x], memory))

    with open('../../csvs/algorithms/FloydWarshall/TIME/Python_runtimes_FloydWarshall.csv', 'w', newline="", encoding='UTF8') as f:
        writer = csv.writer(f)

        writer.writerows(map(lambda x: [x], runtime))

    with open('csvs/Python_vertex_FloydWarshall.csv', 'w', newline="", encoding='UTF8') as f:
        writer = csv.writer(f)

        writer.writerows(map(lambda x: [x], ver))


    print(runtime[0:10])
    print(memory[0:10])
    print(ver[0:10])
    print(cpu[0:10])
