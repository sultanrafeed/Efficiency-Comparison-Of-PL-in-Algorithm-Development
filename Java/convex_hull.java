import java.io.IOException;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;
import java.io.FileWriter;
import java.io.PrintWriter;

class Point
{
    int x, y;
    Point(int x, int y){
        this.x=x;
        this.y=y;
    }
}

class ConvexHull {

    // To find orientation of ordered triplet (p, q, r).
    // The function returns following values
    // 0 --> p, q and r are collinear
    // 1 --> Clockwise
    // 2 --> Counterclockwise
    public static int orientation(Point p, Point q, Point r)
    {
        int val = (q.y - p.y) * (r.x - q.x) -
                (q.x - p.x) * (r.y - q.y);

        if (val == 0) return 0;  // collinear
        return (val > 0)? 1: 2; // clock or counterclock wise
    }

    // Prints convex hull of a set of n points.
    public static void convexHull(Point[] points, int n)
    {
        // There must be at least 3 points
        if (n < 3) return;

        // Initialize Result
        Vector<Point> hull = new Vector<>();

        // Find the leftmost point
        int l = 0;
        for (int i = 1; i < n; i++)
            if (points[i].x < points[l].x)
                l = i;

        // Start from leftmost point, keep moving 
        // counterclockwise until reach the start point
        // again. This loop runs O(h) times where h is
        // number of points in result or output.
        int p = l, q;
        do
        {
            // Add current point to result
            hull.add(points[p]);

            // Search for a point 'q' such that 
            // orientation(p, q, x) is counterclockwise 
            // for all points 'x'. The idea is to keep 
            // track of last visited most counterclock-
            // wise point in q. If any point 'i' is more 
            // counterclock-wise than q, then update q.
            q = (p + 1) % n;

            for (int i = 0; i < n; i++)
            {
                // If i is more counterclockwise than
                // current q, then update q
                if (orientation(points[p], points[i], points[q])
                        == 2)
                    q = i;
            }

            // Now q is the most counterclockwise with
            // respect to p. Set p as q for next iteration, 
            // so that q is added to result 'hull'
            p = q;

        } while (p != l);  // While we don't come to first 
        // point

        // Print Result
//        for (Point temp : hull)
//            System.out.println("(" + temp.x + ", " +
//                    temp.y + ")");
    }

    /* Driver program to test above function */
    private static double getCPUUtilization() {
        com.sun.management.OperatingSystemMXBean osBean =
                (com.sun.management.OperatingSystemMXBean) java.lang.management.ManagementFactory.getOperatingSystemMXBean();
        double cpuUtilization = osBean.getProcessCpuLoad() * 100;
        if (cpuUtilization < 0) {
            cpuUtilization = 0; // Set negative values to 0
        }
        return cpuUtilization;
    }

    private static long getPeakMemoryUsage() {
        Runtime runtime = Runtime.getRuntime();
        return runtime.totalMemory() - runtime.freeMemory();
    }

    public static void main(String[] args) throws IOException {
        List<Integer> datasize = IntStream.range(5, 505).boxed().collect(Collectors.toList());
        List<Long> runTimes = new ArrayList<>();
        List<Double> cpuUtilization = new ArrayList<>();
        List<Double> peakMemoryUsage = new ArrayList<>();

        for (int i: datasize) {
            Point[] points = new Point[i];

            for (int j = 0; j < i; j++) {
                Point new_point = new Point((int) (Math.random() * (100 + 1) + 0), (int) (Math.random() * (100 + 1) + 0));
                points[j] = new_point;
            }

            double peakMemory = getPeakMemoryUsage() / 1024.0; // Peak memory in KB
            peakMemoryUsage.add(peakMemory / 1024);

            long start = System.nanoTime();
            convexHull(points, points.length);
            long end = System.nanoTime();

            long runtime = end - start;

            double cpuUsageAfter = getCPUUtilization();

            cpuUtilization.add(cpuUsageAfter);
            runTimes.add(runtime);

            if (i % 10 == 0) {
                System.out.println(i + " Done");
            }
        }

        PrintWriter runtimesOutFile = new PrintWriter(new FileWriter("Java_runtimes_ConvexHull.csv"));
        runTimes.stream().forEach(j -> runtimesOutFile.append(String.valueOf(j)).append("\n"));
        runtimesOutFile.close();

        PrintWriter cpuUtilizationOutFile = new PrintWriter(new FileWriter("Java_cpuUtilization_ConvexHull.csv"));
        cpuUtilization.stream().forEach(j -> cpuUtilizationOutFile.append(String.valueOf(j)).append("\n"));
        cpuUtilizationOutFile.close();

        PrintWriter peakMemoryUsageOutFile = new PrintWriter(new FileWriter("Java_peakMemoryUsage_ConvexHull.csv"));
        peakMemoryUsage.stream().forEach(j -> peakMemoryUsageOutFile.append(String.valueOf(j)).append("\n"));
        peakMemoryUsageOutFile.close();

    }
}

// This code is contributed by Arnav Kr. Mandal.