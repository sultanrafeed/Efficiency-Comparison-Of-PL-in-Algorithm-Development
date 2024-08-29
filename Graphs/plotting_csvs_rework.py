import os
import csv
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt

mapping_dictionary = {
    "cpuUtilization": "CPU Utilization (Percentage)",
    "peakMemoryUsage": "Peak Memory Usage (MB)",
    "runtimes": "Runtimes (Logarithmic)",

    "ConvexHull": "Convex Hull Algorithm",
    "PrimsAlgorithm": "Prim's Algorithm",
    "CoinChange": "Coin Change Algorithm",
    "FloydWarshall": "Floyd Warshall Algorithm",

    "Python": "red",
    "Java": "blue",
    "C": "green",
    "CPP": "purple",
}

datapoint_mapping = {
    "ConvexHull": 500,
    "PrimsAlgorithm": 500,
    "CoinChange": 500,
    "FloydWarshall": 200,
}

for algorithm in os.listdir("./algorithms"):
    for resource in os.listdir(f"./algorithms/{algorithm}"):
        df_csvs = []

        data = []
        for csv_file in os.listdir(f"./algorithms/{algorithm}/{resource}"):
            filename = os.path.splitext(csv_file)[0]

            df_csvs.append([pd.read_csv(f"algorithms/{algorithm}/{resource}/{csv_file}"), filename])

            count = 0
            with open(f"algorithms/{algorithm}/{resource}/{csv_file}", 'r') as f:
                reader = csv.reader(f)
                for row in reader:
                    data.append(float(row[0]))
                    count += 1
            print(count)

        cpp_filename_parts = df_csvs[0][1].split("_")
        c_filename_parts = df_csvs[1][1].split("_")
        java_filename_parts = df_csvs[2][1].split("_")
        python_filename_parts = df_csvs[3][1].split("_")

        datapoints = datapoint_mapping[c_filename_parts[2]]

        cpp_data = data[0: datapoints - 1]
        c_data = data[datapoints: datapoints * 2 - 1]
        java_data = data[datapoints * 2: datapoints * 3 - 1]
        python_data = data[datapoints * 3: datapoints * 4 - 1]

        print(cpp_data)
        print(c_data)
        print(java_data)
        print(python_data)

        avg_cpp = [sum(cpp_data) / len(cpp_data)] * len(cpp_data)
        avg_c = [sum(c_data) / len(c_data)] * len(c_data)
        avg_java = [sum(java_data) / len(java_data)] * len(java_data)
        avg_python = [sum(python_data) / len(python_data)] * len(python_data)

        x = np.arange(1, len(cpp_data) + 1)

        if cpp_filename_parts[1] == "runtimes":
            cpp_coefficients = np.polyfit(x, cpp_data, 1)  # Adjust the polynomial degree as needed
            c_coefficients = np.polyfit(x, c_data, 1)  # Adjust the polynomial degree as needed
            java_coefficients = np.polyfit(x, java_data, 1)  # Adjust the polynomial degree as needed
            python_coefficients = np.polyfit(x, python_data, 1)  # Adjust the polynomial degree as needed

            cpp_curve = np.polyval(cpp_coefficients, x)
            c_curve = np.polyval(c_coefficients, x)
            java_curve = np.polyval(java_coefficients, x)
            python_curve = np.polyval(python_coefficients, x)

            plt.figure(figsize=(10, 5))
            plt.xlabel("Number of Datapoints")
            plt.ylabel(mapping_dictionary[java_filename_parts[1]])
            plt.title(
                f"Implementation of {mapping_dictionary[java_filename_parts[2]]}\n{mapping_dictionary[java_filename_parts[1]]} vs Number of Datapoints")
            plt.plot(x, cpp_curve, color=f"{mapping_dictionary['C']}", label="CPP")
            plt.plot(x, c_curve, color=f"{mapping_dictionary['CPP']}", label="C")
            plt.plot(x, java_curve, color=f"{mapping_dictionary['Java']}", label="Java")
            plt.plot(x, python_curve, color=f"{mapping_dictionary['Python']}", label="Python")

            ax = plt.gca()
            ax.set_yscale('log')

            plt.legend()
            plt.grid()
            plt.savefig(f"new_graphs/{resource}/{java_filename_parts[1]}_{java_filename_parts[2]}.jpg")
            plt.show()

        else:
            ax = plt.gca()

            ax.set_ylim([0, 110])

            plt.figure(figsize=(10, 5))
            plt.xlabel("Number of Datapoints")
            plt.ylabel(mapping_dictionary[java_filename_parts[1]])
            plt.title(f"Implementation of {mapping_dictionary[java_filename_parts[2]]}\n{mapping_dictionary[java_filename_parts[1]]} vs Number of Datapoints")
            plt.plot(x, avg_c, color=f"{mapping_dictionary['C']}", label="CPP")
            plt.plot(x, avg_cpp, color=f"{mapping_dictionary['CPP']}", label="C")
            plt.plot(x, avg_java, color=f"{mapping_dictionary['Java']}", label="Java")
            plt.plot(x, avg_python, color=f"{mapping_dictionary['Python']}", label="Python")
            plt.legend()
            plt.grid()
            plt.savefig(f"new_graphs/{resource}/{java_filename_parts[1]}_{java_filename_parts[2]}.jpg")
            plt.show()
