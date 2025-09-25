import pandas as pd

# Path to your CSV file
csv_file_path = '/content/70 westbound.csv'
output_csv_file_path = '/content/test2.csv'

try:
    # Read the CSV file into a DataFrame, using on_bad_lines='skip' to skip bad lines
    df = pd.read_csv(csv_file_path, delimiter=',', quotechar='"', skiprows=3, on_bad_lines='skip')

    # Columns to delete
    columns_to_delete = [' Day Type ID', ' Total Carriageway Flow', ' Quality Index', ' Network Link Id', ' NTIS Model Version']

    # Drop the specified columns
    df = df.drop(columns=columns_to_delete)

    df[' Total Flow vehicles less than 5.2m'] = df[' Total Flow vehicles less than 5.2m'].interpolate(method='linear')
    df[' Total Flow vehicles 5.21m - 6.6m'] = df[' Total Flow vehicles 5.21m - 6.6m'].interpolate(method='linear')
    df[' Total Flow vehicles 6.61m - 11.6m'] = df[' Total Flow vehicles 6.61m - 11.6m'].interpolate(method='linear')
    df[' Total Flow vehicles above 11.6m'] = df[' Total Flow vehicles above 11.6m'].interpolate(method='linear')
    df[' Speed Value'] = df[' Speed Value'].interpolate(method='linear')


    # Perform the multiplications and renaming
    df['Category 1'] = df[' Total Flow vehicles less than 5.2m'] * 1
    df['Category 2'] = df[' Total Flow vehicles 5.21m - 6.6m'] * 1.5
    df['Category 3'] = df[' Total Flow vehicles 6.61m - 11.6m'] * 2
    df['Category 4'] = df[' Total Flow vehicles above 11.6m'] * 2.3
    df['Avg Speed in mph'] = df[' Speed Value'] * 0.621371

    # Drop the original columns that have been transformed
    df = df.drop(columns=[' Total Flow vehicles less than 5.2m',
                          ' Total Flow vehicles 5.21m - 6.6m',
                          ' Total Flow vehicles 6.61m - 11.6m',
                          ' Total Flow vehicles above 11.6m',
                          ' Speed Value'])

    # Add the new column for total number of vehicles
    df['Total number of vehicles'] = df['Category 1'] + df['Category 2'] + df['Category 3'] + df['Category 4']

    # Group the data in 4-row chunks and sum the values
    def aggregate_chunk(chunk):
        return pd.Series({
            'Local Date': chunk['Local Date'].iloc[0],
            ' Local Time': chunk[' Local Time'].iloc[0][:2] + ':00:00',
            'Category 1': chunk['Category 1'].sum(),
            'Category 2': chunk['Category 2'].sum(),
            'Category 3': chunk['Category 3'].sum(),
            'Category 4': chunk['Category 4'].sum(),
            'Avg Speed in mph': chunk['Avg Speed in mph'].mean(),  # Assuming average speed is desired
            'Total number of vehicles': chunk['Total number of vehicles'].sum()
        })

    # Apply the aggregation function to each 4-row chunk
    aggregated_df = df.groupby(df.index // 4).apply(aggregate_chunk).reset_index(drop=True)

    # Round the Total number of vehicles to the nearest integer
    aggregated_df['Total number of vehicles'] = aggregated_df['Total number of vehicles'].round()
    aggregated_df['LOS'] = aggregated_df['Total number of vehicles'] / aggregated_df['Avg Speed in mph']

    # Categorize LOS values
    los_bins = [0, 11, 18, 26, 35, 40, float('inf')]
    los_labels = ['A', 'B', 'C', 'D', 'E', 'F']
    aggregated_df['LOS Category'] = pd.cut(aggregated_df['LOS'], bins=los_bins, labels=los_labels, right=False)

    df_pivot = aggregated_df.pivot(index='Local Date', columns=' Local Time', values=['LOS', 'LOS Category'])

    # Count values for each LOS category
    los_category_counts = aggregated_df['LOS Category'].value_counts().reindex(los_labels)

    # Save the aggregated DataFrame to a new CSV file
    # df_pivot.to_csv(output_csv_file_path, index=True)

    print(f"Aggregated data saved to '{output_csv_file_path}'")
    print("LOS Category Counts:")
    print(los_category_counts)

    pa=los_category_counts['A']/(los_category_counts['A']+los_category_counts['B']+los_category_counts['C']+los_category_counts['D']+los_category_counts['E']+los_category_counts['F'])
    pb=los_category_counts['B']/(los_category_counts['A']+los_category_counts['B']+los_category_counts['C']+los_category_counts['D']+los_category_counts['E']+los_category_counts['F'])
    pc=los_category_counts['C']/(los_category_counts['A']+los_category_counts['B']+los_category_counts['C']+los_category_counts['D']+los_category_counts['E']+los_category_counts['F'])
    pd=los_category_counts['D']/(los_category_counts['A']+los_category_counts['B']+los_category_counts['C']+los_category_counts['D']+los_category_counts['E']+los_category_counts['F'])
    pe=los_category_counts['E']/(los_category_counts['A']+los_category_counts['B']+los_category_counts['C']+los_category_counts['D']+los_category_counts['E']+los_category_counts['F'])
    pf=los_category_counts['F']/(los_category_counts['A']+los_category_counts['B']+los_category_counts['C']+los_category_counts['D']+los_category_counts['E']+los_category_counts['F'])
    print(pa,pb,pc,pd,pe,pf)
except pd.errors.ParserError as e:
    print(f'ParserError: {e}')
except Exception as e:
    print(f'Error: {e}')

