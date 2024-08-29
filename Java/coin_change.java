import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.lang.management.ManagementFactory;
import java.lang.management.MemoryMXBean;
import java.lang.management.MemoryUsage;
import java.lang.management.OperatingSystemMXBean;

public class Coin_change {
    static int[] inputs=new int[500];
    static long[] run_time=new long[500];
    static float[] memory = new float[500];
    static double[] cpu = new double[500];
    static int incount=0;
    
    public static void main(String[] args) throws IOException {
        int[] coins = {1,2,3,4,5};
        for(int i=0;i<500;i++){
            int n= 1 + i;
            inputs[incount]=n;
//            System.out.println(
//            "Minimum number of coins required for amount :" +
//            n +
//            " is: " +
//            minimumCoins(coins, n)
//        );
        minimumCoins(coins, n);
        incount++;    
        }
        
        for(int i=0;i<100;i++){
            System.out.println("Memory array"+memory[i]);
            System.out.println("Runtime array"+run_time[i]);
            System.out.println("Cpu array"+cpu[i]);
            System.out.println("Input array"+inputs[i]);
            
        }
        PrintWriter runtimesOutFile = new PrintWriter(new FileWriter("Java_runtimes_CoinChange.csv"));
        for (long l : run_time) {
            runtimesOutFile.append(String.valueOf(l));
            runtimesOutFile.append("\n");
        }
        runtimesOutFile.close();

        PrintWriter cpuUtilizationOutFile = new PrintWriter(new FileWriter("Java_cpuUtilization_CoinChange.csv"));
        for (double l : cpu) {
            cpuUtilizationOutFile.append(String.valueOf(l));
            cpuUtilizationOutFile.append("\n");
        }
        cpuUtilizationOutFile.close();

        PrintWriter peakMemoryUsageOutFile = new PrintWriter(new FileWriter("Java_peakMemoryUsage_CoinChange.csv"));
        for (double l : memory) {
            peakMemoryUsageOutFile.append(String.valueOf(l));
            peakMemoryUsageOutFile.append("\n");
        }
        peakMemoryUsageOutFile.close();

        PrintWriter inputsOutFile = new PrintWriter(new FileWriter("Java_inputs_CoinChange.csv"));
        for (double l : inputs) {
            inputsOutFile.append(String.valueOf(l));
            inputsOutFile.append("\n");
        }
        inputsOutFile.close();
    }

    public static int minimumCoins(int[] coins, int amount) {
        
        OperatingSystemMXBean osBean = ManagementFactory.getOperatingSystemMXBean();
        double cpuUsageStart = osBean.getSystemLoadAverage();
        //System.out.println("CPU Usage: " + cpuUsageStart);
        long startTime = System.currentTimeMillis();
        // minimumCoins[i] will store the minimum coins needed for amount i
        int[] minimumCoins = new int[amount + 1];

        minimumCoins[0] = 0;

        for (int i = 1; i <= amount; i++) {
            minimumCoins[i] = Integer.MAX_VALUE;
        }
        for (int i = 1; i <= amount; i++) {
            for (int coin : coins) {
                if (coin <= i) {
                    int sub_res = minimumCoins[i - coin];
                    if (
                        sub_res != Integer.MAX_VALUE &&
                        sub_res + 1 < minimumCoins[i]
                    ) {
                        minimumCoins[i] = sub_res + 1;
                    }
                }
            }
        }
        // Uncomment the below line to see the state of combinations for each coin
        // printAmount(minimumCoins);
        //Execution time
        long endTime = System.currentTimeMillis();
        long exTime= endTime-startTime;
        //System.out.println("That took " + (exTime) + " milliseconds");
        
        //Memory Usage
        MemoryMXBean memoryBean = ManagementFactory.getMemoryMXBean();
        MemoryUsage heapUsage = memoryBean.getHeapMemoryUsage();
        float memused= heapUsage.getUsed() / 1024;
        
        //System.out.println("Used memory: " + heapUsage.getUsed() / 1024 + " KB");
        //System.out.println("Used memory: " + memused + " KB");
        
        //Cpu usage
        double cp=getCPUUtilization();
        
        //System.out.println("CPU Usage: " + cpuUsageEnd);
        cpu[incount]=cp;
        run_time[incount]=exTime;
        memory[incount]=memused / 1024;
        
        return minimumCoins[amount];
    }
        public static void printAmount(int[] arr) {
        for (int i = 0; i < arr.length; i++) {
            System.out.print(arr[i] + " ");
        }
        System.out.println();
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
    
    

