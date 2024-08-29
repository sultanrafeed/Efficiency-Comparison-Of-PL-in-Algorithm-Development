import random
import time
import psutil
import csv
def coin_change(coins, n):
    start = time.time()
    # start_cpu_usage = psutil.cpu_percent()
    dp = [float('inf')] * (n + 1)
    dp[0] = 0

    for i in range(1, n + 1):
        for coin in coins:
            if coin <= i:
                dp[i] = min(dp[i], dp[i - coin] + 1)

    if dp[n] == float('inf'):
        process = psutil.Process()
        peak_memory = process.memory_info().peak_wset / (1024 * 1024)  # Peak memory in KB
        print(f"Peak memory usage: {peak_memory}KB")
        end_cpu_usage = psutil.cpu_percent()
        # Calculate the CPU usage percentage difference
        cpu_usage_percentage = end_cpu_usage

        end = time.time()
        extime=end-start
        curr_time = round(extime*1000)
        print("Milliseconds:",curr_time)
        return -1
    else:
        process = psutil.Process()
        peak_memory = process.memory_info().peak_wset / (1024 * 1024)  # Peak memory in KB
        # print(f"Peak memory usage: {peak_memory}KB")
        end_cpu_usage = psutil.cpu_percent()
        # Calculate the CPU usage percentage difference
        cpu_usage_percentage = end_cpu_usage

        end = time.time()
        extime=end-start
        curr_time = round(extime*1000)
        #print("Milliseconds:",curr_time)

        return dp[n],curr_time,peak_memory,cpu_usage_percentage

if __name__ == "__main__":
    run_time=[]
    mem=[]
    it_list=500
    coins=[1,2,3,4,5]
    inputs=[]
    cpu=[]
    #size = int(input("Enter the size of the coin values array: "))
    for i in range(it_list):
        #coins = [random.randint(1, 1000) for _ in range(size)]
        n = 1 + i
        #print("Target Sum:", n)
        result,curr_time,peak_memory,c = coin_change(coins, n)
        inputs.append(n)
        run_time.append(curr_time)
        mem.append(peak_memory)
        cpu.append(c)
    print("Minimum number of coins required:", result)
    #print(f"{run_time[0:20]} runtime")
    #run_time=run_time.sort()
    #inputs=inputs.sort()
    #mem=mem.sort()
    #cpu=cpu.sort()

    #print(inputs[0:50])
    print(mem[0:50])
    #print(cpu[0:50])

    with open('../../csvs/algorithms/CoinChange/CPU/Python_cpuUtilization_CoinChange.csv', 'w', newline="",
              encoding='UTF8') as f:
        writer = csv.writer(f)

        writer.writerows(map(lambda x: [x], cpu))

    with open('../../csvs/algorithms/CoinChange/MEM/Python_peakMemoryUsage_CoinChange.csv', 'w', newline="",
              encoding='UTF8') as f:
        writer = csv.writer(f)

        writer.writerows(map(lambda x: [x], mem))

    with open('../../csvs/algorithms/CoinChange/TIME/Python_runtimes_CoinChange.csv', 'w', newline="", encoding='UTF8') as f:
        writer = csv.writer(f)

        writer.writerows(map(lambda x: [x], run_time))

    with open('csvs/Python_inputs_CoinChange.csv', 'w', newline="", encoding='UTF8') as f:
        writer = csv.writer(f)

        writer.writerows(map(lambda x: [x], inputs))
