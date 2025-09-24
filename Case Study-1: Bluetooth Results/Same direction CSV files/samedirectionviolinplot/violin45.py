import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# List of combined CSV files for FFS 60 mph
combined_files_60 = ['combined_LOSA60.csv', 'combined_LOSB60.csv', 'combined_LOSC60.csv', 'combined_LOSD60.csv', 'combined_LOSE60.csv']

# Mapping between file names and labels for FFS 60 mph
file_labels_60 = {
    'combined_LOSA60.csv': 'LOS A',
    'combined_LOSB60.csv': 'LOS B',
    'combined_LOSC60.csv': 'LOS C',
    'combined_LOSD60.csv': 'LOS D',
    'combined_LOSE60.csv': 'LOS E'
}

# List of combined CSV files for FFS 45 mph
combined_files_70 = ['combined_LOSA70.csv', 'combined_LOSB70.csv', 'combined_LOSC70.csv', 'combined_LOSD70.csv', 'combined_LOSE70.csv']

# Mapping between file names and labels for FFS 45 mph
file_labels_70 = {
    'combined_LOSA70.csv': 'LOS A',
    'combined_LOSB70.csv': 'LOS B',
    'combined_LOSC70.csv': 'LOS C',
    'combined_LOSD70.csv': 'LOS D',
    'combined_LOSE70.csv': 'LOS E'
}

# Load data for FFS 60 mph
all_dfs_60 = []
for file in combined_files_60:
    df = pd.read_csv(file)
    df['File'] = file_labels_60[file]
    df['FFS'] = '60 mph'
    all_dfs_60.append(df)

# Load data for FFS 45 mph
all_dfs_70 = []
for file in combined_files_70:
    df = pd.read_csv(file)
    df['File'] = file_labels_70[file]
    df['FFS'] = '70 mph'
    all_dfs_70.append(df)

# Combine both dataframes
combined_df = pd.concat(all_dfs_60 + all_dfs_70)

# Create violin plot
plt.figure(figsize=(12, 6))
ax = sns.violinplot(data=combined_df, x='File', y='Total Msg Time < 100', hue='FFS', palette={'60 mph': 'green', '70 mph': 'blue'}, split=True)
plt.title('Attack Duration for different LOS')
plt.xlabel('Level Of Service')
plt.ylabel('Attack Duration')
plt.xticks(rotation=45, ha='right')
# plt.axhline(y=30, color='yellow', linestyle='--', label='30 Minimum Threshold')  # Adding the horizontal orange dotted line with legend label
plt.axhline(y=45, color='red', linestyle='--', label='45 Threshold')  # Adding the horizontal red dotted line with legend label
# plt.axhline(y=180, color='red', linestyle='--', label='180 Threshold')  # Adding the horizontal red dotted line with legend label
plt.tight_layout()
plt.legend(title='FFS', loc='upper left', bbox_to_anchor=(1, 1))
plt.show()

# Calculate samples above and below minimum threshold for each violin plot
for label, df_group in combined_df.groupby(['File', 'FFS']):
    # above_threshold_30 = df_group[df_group['Attacker Messages'] > 30].shape[0]
    # below_threshold_30 = df_group[df_group['Attacker Messages'] <= 30].shape[0]
    above_threshold_45 = df_group[df_group['Total Msg Time < 100'] > 45].shape[0]
    below_threshold_45 = df_group[df_group['Total Msg Time < 100'] <= 45].shape[0]
    # above_threshold_180 = df_group[df_group['Attacker Messages'] > 180].shape[0]
    # below_threshold_180 = df_group[df_group['Attacker Messages'] <= 180].shape[0]
    print(f"For {label[0]} at {label[1]} FFS:")
    # print(f"  - Samples above 30 threshold: {above_threshold_30}")
    # print(f"  - Samples below 30 threshold: {below_threshold_30}")
    print(f"  - Samples above 45 threshold: {above_threshold_45}")
    print(f"  - Samples below 45 threshold: {below_threshold_45}")
    # print(f"  - Samples above 180 threshold: {above_threshold_180}")
    # print(f"  - Samples below 180 threshold: {below_threshold_180}")

