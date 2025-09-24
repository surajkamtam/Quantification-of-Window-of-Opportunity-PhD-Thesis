import os
import pandas as pd

def classify_distance(distance):
    if distance < 100:
        return 'Less than 100'
    else:
        return 'Other'

def filegeneration(out_file, rt_file, test_file, output_file, mode):
    # Read the input files
    print("Reading files:")
    print("Out file:", out_file)
    dfout = pd.read_csv(out_file, header=None, names=["HID", "HPosition", "HSpeed", "HTime"])
    print(dfout.head())  # Check if dfout is empty

    print("RT file:", rt_file)
    dfrt = pd.read_csv(rt_file, header=None, names=["MHID", "MDistance", "MTime", "Messages"])
    print(dfrt.head())   # Check if dfrt is empty

    print("Test file:", test_file)
    dftest = pd.read_csv(test_file, header=None, names=["VHID", "VPosition", "VSpeed", "VTime"])
    print(dftest.head())  # Check if dftest is empty

    # Classify distances
    dfrt['DistanceCategory'] = dfrt['MDistance'].apply(classify_distance)

    # Group by distance category and count messages
    message_counts_distance = dfrt.groupby(['MHID', 'DistanceCategory'])['Messages'].count().unstack(fill_value=0)

    # Calculate various metrics
    data_speed = {"Atk Average Speed": dfout["HSpeed"].mean(), "Vo Average Speed": dftest["VSpeed"].mean()}
    data_time = {
        "Atk Entry Time": dfout["HTime"].iloc[0], "Atk Exit Time": dfout["HTime"].iloc[-1], 
        "AtK Sim Time": dfout["HTime"].iloc[-1] - dfout["HTime"].iloc[0],
        "Vo Entry Time": dftest["VTime"].iloc[0], "Vo Exit Time": dftest["VTime"].iloc[-1], 
        "Vo Sim Time": dftest["VTime"].iloc[-1] - dftest["VTime"].iloc[0]
    }
    data_messages = {
        "Attacker Messages": dfrt["Messages"].sum(), "Atk First Msg Time": dfrt["MTime"].iloc[0], 
        "Atk Last Msg Time": dfrt["MTime"].iloc[-1], "AtK Msg Duration": dfrt["MTime"].iloc[-1] - dfrt["MTime"].iloc[0]
    }

    # Calculate total message time for messages with distance less than 100
    filtered_data = dfrt[dfrt['MDistance'] < 100]
    if not filtered_data.empty:
        total_msg_time = filtered_data['MTime'].iloc[-1] - filtered_data['MTime'].iloc[0]
    else:
        total_msg_time = 0
    data_messages["Total Msg Time < 100"] = total_msg_time

    # Create dataframes for the metrics
    df_speed = pd.DataFrame(data_speed, index=[0])
    df_time = pd.DataFrame(data_time, index=[0])
    df_messages = pd.DataFrame(data_messages, index=[0])

    # Reset index for message counts dataframe
    message_counts_distance.reset_index(inplace=True)

    # Create dataframe for the 'Less than 100' category if it exists
    less_than_100 = pd.DataFrame(columns=["MHID", "Less than 100"])
    for _, row in message_counts_distance.iterrows():
        if 'Less than 100' in row:
            new_row = pd.DataFrame({"MHID": [row["MHID"]], "Less than 100": [row["Less than 100"]]})
            less_than_100 = pd.concat([less_than_100, new_row], ignore_index=True)

    # Concatenate all dataframes into one
    result = pd.concat([df_speed, df_time, df_messages, less_than_100], axis=1)

    # Write or append to CSV
    if mode == 'w':
        result.to_csv(output_file, index=False, header=True)
    elif mode == 'a':
        result.to_csv(output_file, mode='a', index=False, header=False)

# Main script
#/home/sidharth/Downloads/60Oppositecsv/LOSBD/
#/home/sidharth/Downloads/60Oppositecsv/LOSBD
folder_path = '/home/sidharth/Downloads/60Oppositecsv/LOSEE/'
output_folder_path = '/home/sidharth/Downloads/60Oppositecsv/LOSEE'

for folder_name in os.listdir(folder_path):
    folder_full_path = os.path.join(folder_path, folder_name)
    if os.path.isdir(folder_full_path):
        result_file = os.path.join(output_folder_path, folder_name + '.csv')

        # Run file generation for each iteration within the current folder
        for i in range(1, 51):  # Loop for 50 iterations
            mode = 'w' if i == 1 else 'a'
            filegeneration(
                os.path.join(folder_full_path, f"out_{i}.txt"),
                os.path.join(folder_full_path, f"rt_{i}.txt"),
                os.path.join(folder_full_path, f"test_{i}.txt"),
                result_file, mode
            )

