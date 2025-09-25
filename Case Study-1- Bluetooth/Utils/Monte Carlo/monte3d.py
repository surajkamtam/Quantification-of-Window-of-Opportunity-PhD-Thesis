import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import os
from collections import defaultdict
import numpy as np

# Function to extract data from a single text file
def extract_data(file_path):
    time_values = []
    message_values = []
    with open(file_path, 'r') as file:
        for line in file:
            parts = line.strip().split(',')
            if len(parts) >= 3:
                time_values.append(float(parts[2]))  # Extract the time value
                message_values.append(int(parts[3]))  # Extract the message value
    return time_values, message_values

# List of base directories to include
base_directories = ['LOSA', 'LOSB', 'LOSC', 'LOSD', 'LOSE']  # The directories LOSA to LOSE
folders = [f'depart_{i}' for i in range(885, 916)]  # List of folder names from depart_885 to depart_915

# Initialize a dictionary to store data for each folder in each base directory
all_folder_data = {}

# List of colors for each base directory (color coding for average lines)
colors = ['turquoise', 'royalblue', 'goldenrod', 'darkorange', 'firebrick']

# Loop through each base directory
for base_directory in base_directories:
    folder_data = {}

    # Loop through each folder within the base directory
    for folder in folders:
        directory_path = os.path.join(base_directory, folder)
        grouped_messages = defaultdict(int)

        # Loop through all files in the folder
        for filename in os.listdir(directory_path):
            if filename.startswith('rt_') and filename.endswith('.txt'):
                file_path = os.path.join(directory_path, filename)
                time_values, message_values = extract_data(file_path)
                for time, message in zip(time_values, message_values):
                    rounded_time = round(time)  # Round the time to the nearest integer
                    grouped_messages[rounded_time] += message  # Aggregate messages by rounded time

        # Sort the grouped data by time and store it
        sorted_times = sorted(grouped_messages.keys())
        sorted_messages = [grouped_messages[time] for time in sorted_times]
        folder_data[folder] = (sorted_times, sorted_messages)

    # Store the folder data for this base directory
    all_folder_data[base_directory] = folder_data

# Initialize a 3D plot
fig = plt.figure(figsize=(15, 8))
ax = fig.add_subplot(111, projection='3d')

# Loop through each base directory
for i, (base_directory, folder_data) in enumerate(all_folder_data.items()):
    all_times = set()

    # Collect all unique times for this base directory
    for times, _ in folder_data.values():
        all_times.update(times)
    all_times = sorted(all_times)

    # Calculate the average messages for each time across the folders
    average_messages = []
    for time in all_times:
        total = 0
        count = 0
        for _, (times, messages) in folder_data.items():
            if time in times:
                total += messages[times.index(time)]
                count += 1
        average_messages.append(total / count if count > 0 else 0)

    # Plot the individual folder lines for this base directory (without legend)
    for folder, (times, messages) in folder_data.items():
        ax.plot(times, messages, zs=i, zdir='y', linestyle='-', alpha=0.1)  # Plot with 3D effect

    # Plot the overall average line for this base directory (with legend)
    ax.plot(all_times, average_messages, zs=i, zdir='y', linewidth=2, label=f'{base_directory} Avg.', color=colors[i])

# Set labels for the plot with smaller font sizes
ax.set_title('3D Monte Carlo Simulation', fontsize=12)  # Smaller title
ax.set_xlabel('Simtime', fontsize=8)  # Smaller x-axis label
ax.set_ylabel('LOS', fontsize=8)  # Smaller y-axis label
ax.set_zlabel('Attacker Messages', fontsize=8)  # Smaller z-axis label

ax.tick_params(axis='x', labelsize=8)  # Smaller font size for x-ticks (Simtime)
ax.tick_params(axis='z', labelsize=8)
# Set the ticks for the y-axis to represent the base directories
ax.set_yticks(range(len(base_directories)))
ax.set_yticklabels(base_directories, fontsize=8)  # Smaller font for y-tick labels

# Show the legend with smaller font size for only the average lines
ax.legend(fontsize=8, loc='upper left')

# Show the plot
plt.show()

