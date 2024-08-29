import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.lang.management.ManagementFactory;
import java.lang.management.MemoryMXBean;
import java.lang.management.MemoryUsage;
import java.lang.management.OperatingSystemMXBean;

class PrimsAlgorithm2 {
    static int[] inputs = new int[500];
    static long[] run_time = new long[500];
    static float[] memory = new float[500];
    static double[] cpu = new double[500];
    static int incount = 0;

    public static void main(String[] args) throws IOException {
        for (int i = 0; i < 500; i++) {
            int n = 10 + i;
            inputs[incount] = n;

            int[][] graph = generateRandomGraph(n);
            primMST(graph, n);

            incount++;
        }

//        for (int i = 0; i < 100; i++) {
//            System.out.println("Memory array: " + memory[i]);
//            System.out.println("Runtime array: " + run_time[i]);
//            System.out.println("CPU array: " + cpu[i]);
//            System.out.println("Input array: " + inputs[i]);
//        }

        PrintWriter runtimesOutFile = new PrintWriter(new FileWriter("Java_runtimes_PrimsAlgorithm.csv"));
        for (long l : run_time) {
            runtimesOutFile.append(String.valueOf(l));
            runtimesOutFile.append("\n");
        }
        runtimesOutFile.close();

        PrintWriter cpuUtilizationOutFile = new PrintWriter(new FileWriter("Java_cpuUtilization_PrimsAlgorithm.csv"));
        for (double l : cpu) {
            cpuUtilizationOutFile.append(String.valueOf(l));
            cpuUtilizationOutFile.append("\n");
        }
        cpuUtilizationOutFile.close();

        PrintWriter peakMemoryUsageOutFile = new PrintWriter(new FileWriter("Java_peakMemoryUsage_PrimsAlgorithm.csv"));
        for (double l : memory) {
            peakMemoryUsageOutFile.append(String.valueOf(l));
            peakMemoryUsageOutFile.append("\n");
        }
        peakMemoryUsageOutFile.close();
    }

    public static int[][] generateRandomGraph(int n) {
        int[][] graph = new int[n][n];

        // Generate random weights for the graph edges
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    graph[i][j] = 0;
                } else {
                    graph[i][j] = (int) (Math.random() * 10) + 1;
                }
            }
        }

        return graph;
    }

    public static void primMST(int[][] graph, int V) {
        int[] parent = new int[V];
        int[] key = new int[V];
        boolean[] mstSet = new boolean[V];

        for (int i = 0; i < V; i++) {
            key[i] = Integer.MAX_VALUE;
            mstSet[i] = false;
        }

        key[0] = 0;
        parent[0] = -1;

        OperatingSystemMXBean osBean = ManagementFactory.getOperatingSystemMXBean();
        double cpuUsageStart = osBean.getSystemLoadAverage();
        long startTime = System.currentTimeMillis();

        for (int count = 0; count < V - 1; count++) {
            int u = minKey(key, mstSet, V);
            mstSet[u] = true;

            for (int v = 0; v < V; v++) {
                if (graph[u][v] != 0 && !mstSet[v] && graph[u][v] < key[v]) {
                    parent[v] = u;
                    key[v] = graph[u][v];
                }
            }
        }

        long endTime = System.currentTimeMillis();
        long exTime = endTime - startTime;

        MemoryMXBean memoryBean = ManagementFactory.getMemoryMXBean();
        MemoryUsage heapUsage = memoryBean.getHeapMemoryUsage();
        float memused = heapUsage.getUsed() / 1024;

        double cp = getCPUUtilization();

        cpu[incount] = cp;
        run_time[incount] = exTime;
        memory[incount] = memused / 1024;
    }

    public static int minKey(int[] key, boolean[] mstSet, int V) {
        int min = Integer.MAX_VALUE;
        int minIndex = -1;

        for (int v = 0; v < V; v++) {
            if (!mstSet[v] && key[v] < min) {
                min = key[v];
                minIndex = v;
            }
        }

        return minIndex;
    }

    private static double getCPUUtilization() {
        com.sun.management.OperatingSystemMXBean osBean =
                (com.sun.management.OperatingSystemMXBean) java.lang.management.ManagementFactory.getOperatingSystemMXBean();
        double cpuUtilization = osBean.getProcessCpuLoad() * 100;
        if (cpuUtilization < 0) {
            cpuUtilization = 0; // Set negative values to 0
        }
        return cpuUtilization;
    }
}

