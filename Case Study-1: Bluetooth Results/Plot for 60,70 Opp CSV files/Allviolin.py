import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

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

# Load data for FFS 60 mph
all_dfs_60 = []
for file in combined_files_60:
    df = pd.read_csv(file)
    df['File'] = file_labels_60[file]
    df['FFS'] = '60 mph'
    all_dfs_60.append(df)

# Load data for FFS 70 mph
all_dfs_70 = []
for file in combined_files_70:
    df = pd.read_csv(file)
    df['File'] = file_labels_70[file]
    df['FFS'] = '70 mph'
    all_dfs_70.append(df)

# Combine both dataframes
combined_df = pd.concat(all_dfs_60 + all_dfs_70)

g = sns.catplot(data=combined_df, y='Total Msg Time < 100', hue='FFS', palette={'60 mph': 'green', '70 mph': 'blue'}, kind='violin', col='File', col_wrap=5, split=True, height=4, aspect=1.2)

# Create the legend manually on the LOS AE subplot
legend_ax = g.axes.flatten()[4]  # LOS AE subplot
legend_labels = ['60 mph', '70 mph']
legend_handles = [plt.Line2D([0], [0], color='green', lw=4), plt.Line2D([0], [0], color='blue', lw=4)]
legend = legend_ax.legend(title='FFS', bbox_to_anchor=(1, 1), loc='upper left', labels=legend_labels, handles=legend_handles, fontsize='x-large', handletextpad=0.5)
legend.set_title('FFS')

# Set titles and labels
g.fig.suptitle('Attack Duration for different LOS', y=1.05)
g.set_titles('')
g.set_axis_labels('Level Of Service', 'Attack Duration')

# Add file labels below each subplot and x-label for each subplot
for ax, file_label in zip(g.axes.flatten(), combined_df['File'].unique()):
    ax.text(0.5, -0.15, file_label, horizontalalignment='center', verticalalignment='center', transform=ax.transAxes, fontsize=10)
    ax.set_xlabel('')

    # Add horizontal threshold line at 45
    ax.axhline(y=45, color='red', linestyle='--')

    # Calculate samples above and below the threshold
    #above_threshold = combined_df[(combined_df['File'] == file_label) & (combined_df['Total Msg Time < 100'] > 45)]['Total Msg Time < 100'].count()
    #below_threshold = combined_df[(combined_df['File'] == file_label) & (combined_df['Total Msg Time < 100'] <= 45)]['Total Msg Time < 100'].count()
    #ax.text(0.5, 0.9, f"Above Threshold: {above_threshold}\nBelow Threshold: {below_threshold}", horizontalalignment='center', verticalalignment='center', transform=ax.transAxes, fontsize=10, color='purple')

# Move the legend outside of the main plot grid
g.fig.subplots_adjust(top=0.9, right=0.8)

# Remove the legend created by sns.catplot
g._legend.remove()

plt.tight_layout()
plt.show()

