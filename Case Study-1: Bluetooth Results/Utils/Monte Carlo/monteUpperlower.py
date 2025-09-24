import matplotlib.pyplot as plt
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

# List of colors for each base directory (you can customize these colors as needed)
colors = ['turquoise', 'royalblue', 'yellow', 'darkorange', 'firebrick']

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

# Initialize a plot
plt.figure(figsize=(15, 8))

# Loop through each base directory
for i, (base_directory, folder_data) in enumerate(all_folder_data.items()):
    all_times = set()
    
    # Collect all unique times for this base directory
    for times, _ in folder_data.values():
        all_times.update(times)
    all_times = sorted(all_times)

    # Collect messages for each time across folders
    all_messages_by_time = {time: [] for time in all_times}
    for time in all_times:
        for _, (times, messages) in folder_data.items():
            if time in times:
                all_messages_by_time[time].append(messages[times.index(time)])

    # Calculate the bounds
    upper_bound = [max(all_messages_by_time[time]) if all_messages_by_time[time] else 0 for time in all_times]
    lower_bound = [min(all_messages_by_time[time]) if all_messages_by_time[time] else 0 for time in all_times]
    
    # Calculate the overall mean, upper bound, and lower bound across all times
    overall_mean = np.mean([np.mean(all_messages_by_time[time]) for time in all_times if all_messages_by_time[time]])
    overall_upper = max(upper_bound)
    overall_lower = min(lower_bound)

    # Print the overall results for this LOS
    print(f"Results for {base_directory}:")
    print(f"  Overall Mean: {overall_mean:.2f}")
    print(f"  Overall Upper Bound: {overall_upper}")
    print(f"  Overall Lower Bound: {overall_lower}")
    print("\n")

    # Plot the individual folder lines for this base directory (optional, faint lines for context)
    for folder, (times, messages) in folder_data.items():
        plt.plot(times, messages, linestyle='-', alpha=0.05, color=colors[i])  # Low intensity for individual lines

    # Plot the bounds as a shaded region
    plt.fill_between(all_times, lower_bound, upper_bound, color=colors[i], alpha=0.3, label=f'{base_directory} Bounds')

# Set plot titles and labels
plt.title('Monte Carlo Simulation', fontsize=16)
plt.xlabel('Simtime', fontsize=12)
plt.ylabel('Attacker Messages', fontsize=12)
plt.grid(True)

# Show the legend for the bounds
plt.legend(fontsize=10, loc='upper left', bbox_to_anchor=(1, 1))

# Save the combined plot to a file
output_path = 'monte_bounds.png'  # Final plot file name
plt.savefig(output_path, bbox_inches='tight')
plt.show()

