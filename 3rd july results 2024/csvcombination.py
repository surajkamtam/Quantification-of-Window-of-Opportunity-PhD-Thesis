# code for combining each of the csvs
import os
import pandas as pd

# Define the directory path
# /home/sidharth/Downloads/Same60/LOSE60
directory = '/home/sidharth/Downloads/CSV files/Same70/LOSE70'

# Get all CSV files in the directory
csv_files = [file for file in os.listdir(directory) if file.endswith('.csv')]

# Initialize an empty list to store dataframes
dfs = []

# Read each CSV file and append its dataframe to the list
for file in csv_files:
    df = pd.read_csv(os.path.join(directory, file))
    dfs.append(df)

# Concatenate all dataframes into one
combined_df = pd.concat(dfs, ignore_index=True)

# Write the combined dataframe to a new CSV file
combined_csv_filename = 'combined_' + os.path.basename(directory) + '.csv'
combined_df.to_csv(combined_csv_filename, index=False)

print(f'Combined CSV file saved as {combined_csv_filename}')

