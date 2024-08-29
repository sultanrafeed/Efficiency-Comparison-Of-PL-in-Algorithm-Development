import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.lang.management.ManagementFactory;
import java.lang.management.MemoryMXBean;
import java.lang.management.MemoryUsage;
import java.util.Random;
import java.util.Scanner;

public class Floyd {
    static int[] inputs=new int[200];
    static long[] run_time=new long[200];
    static float[] memory = new float[200];
    static double[] cpu = new double[200];
    static int incount=0;
 private int[][] DistanceMatrix;
    private int numberofvertices; 
    public static final int INFINITY = 999;

    public Floyd(int numberofvertices) {
        DistanceMatrix = new int[numberofvertices + 1][numberofvertices + 1]; 
        this.numberofvertices = numberofvertices;
    }
    public void floydwarshall(int[][] AdjacencyMatrix) { 
        long startTime = System.currentTimeMillis();
        
        for (int source = 1; source <= numberofvertices; source++) {
            for (
                int destination = 1;
                destination <= numberofvertices;
                destination++
            ) {
                DistanceMatrix[source][destination] =
                    AdjacencyMatrix[source][destination];
            }
        }
        for (
            int intermediate = 1;
            intermediate <= numberofvertices;
            intermediate++
        ) {
            for (int source = 1; source <= numberofvertices; source++) {
                for (
                    int destination = 1;
                    destination <= numberofvertices;
                    destination++
                ) {
                    if (
                        DistanceMatrix[source][intermediate] +
                        DistanceMatrix[intermediate][destination] <
                        DistanceMatrix[source][destination]
                    ) { // calculated distance it get replaced as new shortest distance // if the new distance calculated is less then the earlier shortest
                        DistanceMatrix[source][destination] =
                            DistanceMatrix[source][intermediate] +
                            DistanceMatrix[intermediate][destination];
                    }
                }
            }
        }
         long endTime = System.currentTimeMillis();

//        for (int source = 1; source <= numberofvertices; source++) {
//            System.out.print("\t" + source);
//        }
//        System.out.println();
//        for (int source = 1; source <= numberofvertices; source++) {
//            System.out.print(source + "\t");
//            for (
//                int destination = 1;
//                destination <= numberofvertices;
//                destination++
//            ) {
//                System.out.print(DistanceMatrix[source][destination] + "\t");
//            }
//            System.out.println();
//
//        }
        double cpu1 = getCPUUtilization();

        MemoryMXBean memoryBean = ManagementFactory.getMemoryMXBean();
        MemoryUsage heapUsage = memoryBean.getHeapMemoryUsage();
        long exeTime=endTime-startTime;
        float mem= heapUsage.getUsed() / 1024;
        run_time[incount]=exeTime;
        memory[incount]=mem / 1024;
        cpu[incount]=cpu1;
       // System.out.println("Memory used: " + heapUsage.getUsed() / 1024 + " KB");
        //System.out.println("Time taken :" + (endTime - startTime) + " milliseconds"); 
        
    }

    public static void main(String... arg) throws IOException {
        for(int i=0;i<200;i++){
            int numberOfVertices = 2 + i;
            inputs[incount] = numberOfVertices;
            int[][] adjacencyMatrix = new int[numberOfVertices + 1][numberOfVertices + 1];
            for (int source = 1; source <= numberOfVertices; source++) {
                for (int destination = 1;destination <= numberOfVertices;destination++)
                {
                    adjacencyMatrix[source][destination] = 1 + (int)(Math.random() * ((10 - 1) + 1));
                    if (source == destination) {
                        adjacencyMatrix[source][destination] = 0;
                        continue;
                    }
                    if (adjacencyMatrix[source][destination] == 0) {
                        adjacencyMatrix[source][destination] = INFINITY;
                    }
                }
             }
            Floyd floydwarshall = new Floyd(numberOfVertices);
            floydwarshall.floydwarshall(adjacencyMatrix);
            incount++;
//            for(int j=0;j<20;j++){
//                System.out.println("Memory array "+memory[j]);
//                System.out.println("Runtime array "+run_time[j]);
//                System.out.println("cpu array "+cpu[j]);
//                System.out.println("Input array "+inputs[j]);
//
//            }
            if ((incount % 10) == 0) {
                System.out.println(incount + " Done");
            }
        }

        PrintWriter runtimesOutFile = new PrintWriter(new FileWriter("Java_runtimes_FloydWarshall.csv"));
        for (long l : run_time) {
            runtimesOutFile.append(String.valueOf(l));
            runtimesOutFile.append("\n");
        }
        runtimesOutFile.close();

        PrintWriter cpuUtilizationOutFile = new PrintWriter(new FileWriter("Java_cpuUtilization_FloydWarshall.csv"));
        for (double l : cpu) {
            cpuUtilizationOutFile.append(String.valueOf(l));
            cpuUtilizationOutFile.append("\n");
        }
        cpuUtilizationOutFile.close();

        PrintWriter peakMemoryUsageOutFile = new PrintWriter(new FileWriter("Java_peakMemoryUsage_FloydWarshall.csv"));
        for (double l : memory) {
            peakMemoryUsageOutFile.append(String.valueOf(l));
            peakMemoryUsageOutFile.append("\n");
        }
        peakMemoryUsageOutFile.close();

        PrintWriter vertexOutFile = new PrintWriter(new FileWriter("Java_vertex_FloydWarshall.csv"));
        for (double l : inputs) {
            vertexOutFile.append(String.valueOf(l));
            vertexOutFile.append("\n");
        }
        vertexOutFile.close();

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
