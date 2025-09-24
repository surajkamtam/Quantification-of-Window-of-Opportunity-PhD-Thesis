import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Load data for FFS 60 mph
combined_files_60 = ['combined_LOSAA60.csv', 'combined_LOSAB60.csv', 'combined_LOSAC60.csv', 'combined_LOSAD60.csv', 'combined_LOSAE60.csv',
                     'combined_LOSBA60.csv', 'combined_LOSBB60.csv', 'combined_LOSBC60.csv', 'combined_LOSBD60.csv', 'combined_LOSBE60.csv',
                     'combined_LOSCA60.csv', 'combined_LOSCB60.csv', 'combined_LOSCC60.csv', 'combined_LOSCD60.csv', 'combined_LOSCE60.csv',
                     'combined_LOSDA60.csv', 'combined_LOSDB60.csv', 'combined_LOSDC60.csv', 'combined_LOSDD60.csv', 'combined_LOSDE60.csv',
                     'combined_LOSEA60.csv', 'combined_LOSEB60.csv', 'combined_LOSEC60.csv', 'combined_LOSED60.csv', 'combined_LOSEE60.csv']
file_labels_60 = {'combined_LOSAA60.csv': 'LOS AA', 'combined_LOSAB60.csv': 'LOS AB', 'combined_LOSAC60.csv': 'LOS AC', 'combined_LOSAD60.csv': 'LOS AD', 'combined_LOSAE60.csv': 'LOS AE',
                  'combined_LOSBA60.csv': 'LOS BA', 'combined_LOSBB60.csv': 'LOS BB', 'combined_LOSBC60.csv': 'LOS BC', 'combined_LOSBD60.csv': 'LOS BD', 'combined_LOSBE60.csv': 'LOS BE',
                  'combined_LOSCA60.csv': 'LOS CA', 'combined_LOSCB60.csv': 'LOS CB', 'combined_LOSCC60.csv': 'LOS CC', 'combined_LOSCD60.csv': 'LOS CD', 'combined_LOSCE60.csv': 'LOS CE',
                  'combined_LOSDA60.csv': 'LOS DA', 'combined_LOSDB60.csv': 'LOS DB', 'combined_LOSDC60.csv': 'LOS DC', 'combined_LOSDD60.csv': 'LOS DD', 'combined_LOSDE60.csv': 'LOS DE',
                  'combined_LOSEA60.csv': 'LOS EA', 'combined_LOSEB60.csv': 'LOS EB', 'combined_LOSEC60.csv': 'LOS EC', 'combined_LOSED60.csv': 'LOS ED', 'combined_LOSEE60.csv': 'LOS EE'}

# Load data for FFS 70 mph
combined_files_70 = ['combined_LOSAA.csv', 'combined_LOSAB.csv', 'combined_LOSAC.csv', 'combined_LOSAD.csv', 'combined_LOSAE.csv',
                     'combined_LOSBA.csv', 'combined_LOSBB.csv', 'combined_LOSBC.csv', 'combined_LOSBD.csv', 'combined_LOSBE.csv',
                     'combined_LOSCA.csv', 'combined_LOSCB.csv', 'combined_LOSCC.csv', 'combined_LOSCD.csv', 'combined_LOSCE.csv',
                     'combined_LOSDA.csv', 'combined_LOSDB.csv', 'combined_LOSDC.csv', 'combined_LOSDD.csv', 'combined_LOSDE.csv',
                     'combined_LOSEA.csv', 'combined_LOSEB.csv', 'combined_LOSEC.csv', 'combined_LOSED.csv', 'combined_LOSEE.csv']
file_labels_70 = {'combined_LOSAA.csv': 'LOS AA', 'combined_LOSAB.csv': 'LOS AB', 'combined_LOSAC.csv': 'LOS AC', 'combined_LOSAD.csv': 'LOS AD', 'combined_LOSAE.csv': 'LOS AE',
                  'combined_LOSBA.csv': 'LOS BA', 'combined_LOSBB.csv': 'LOS BB', 'combined_LOSBC.csv': 'LOS BC', 'combined_LOSBD.csv': 'LOS BD', 'combined_LOSBE.csv': 'LOS BE',
                  'combined_LOSCA.csv': 'LOS CA', 'combined_LOSCB.csv': 'LOS CB', 'combined_LOSCC.csv': 'LOS CC', 'combined_LOSCD.csv': 'LOS CD', 'combined_LOSCE.csv': 'LOS CE',
                  'combined_LOSDA.csv': 'LOS DA', 'combined_LOSDB.csv': 'LOS DB', 'combined_LOSDC.csv': 'LOS DC', 'combined_LOSDD.csv': 'LOS DD', 'combined_LOSDE.csv': 'LOS DE',
                  'combined_LOSEA.csv': 'LOS EA', 'combined_LOSEB.csv': 'LOS EB', 'combined_LOSEC.csv': 'LOS EC', 'combined_LOSED.csv': 'LOS ED', 'combined_LOSEE.csv': 'LOS EE'}

# Process and load data
all_dfs_60 = []
for file in combined_files_60:
    df = pd.read_csv(file)
    df['File'] = file_labels_60[file]
    df['FFS'] = '60 mph'
    all_dfs_60.append(df)

all_dfs_70 = []
for file in combined_files_70:
    df = pd.read_csv(file)
    df['File'] = file_labels_70[file]
    df['FFS'] = '70 mph'
    all_dfs_70.append(df)

combined_df = pd.concat(all_dfs_60 + all_dfs_70)

# Print statistics for each LOS and FFS
for file_label in combined_df['File'].unique():
    for ffs in ['60 mph', '70 mph']:
        subset = combined_df[(combined_df['File'] == file_label) & (combined_df['FFS'] == ffs)]['Total Msg Time < 100']
        if subset.empty:
            continue
        mean = subset.mean()
        median = subset.median()
        q1 = subset.quantile(0.25)
        q3 = subset.quantile(0.75)
        min_val = subset.min()
        max_val = subset.max()

        # Calculate width statistics
        max_width = max_val
        min_width = min_val
        width_span = max_width - min_width

        print(f"File: {file_label}, FFS: {ffs}")
        print(f"  Mean: {mean:.2f}")
        print(f"  Median: {median:.2f}")
        print(f"  Q1 (25th percentile): {q1:.2f}")
        print(f"  Q3 (75th percentile): {q3:.2f}")
        print(f"  Min: {min_val}")
        print(f"  Max: {max_val}")
        print(f"  Max Width: {max_width}")
        print(f"  Min Width: {min_width}")
        print(f"  Width Span: {width_span:.2f}")
        print("-" * 50)

# Plot using seaborn
sns.set(style='whitegrid')
plt.figure(figsize=(18, 12))

g = sns.violinplot(data=combined_df, x='File', y='Total Msg Time < 100', hue='FFS', palette={'60 mph': 'green', '70 mph': 'blue'}, dodge=True)

# Add horizontal threshold line
plt.axhline(y=45, color='red', linestyle='--', label='Threshold (45)')

# Final adjustments
plt.title('Attack Duration and Width Statistics for Different LOS', fontsize=16)
plt.xlabel('Level Of Service', fontsize=12)
plt.ylabel('Attack Duration', fontsize=12)
plt.legend(title='FFS', bbox_to_anchor=(1.05, 1), loc='upper left')
plt.xticks(rotation=45)
plt.tight_layout()
plt.show()

