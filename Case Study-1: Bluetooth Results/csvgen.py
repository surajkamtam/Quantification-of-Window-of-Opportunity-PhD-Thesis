#Code for generating csvs for all subfolders old
import os
import pandas as pd

def classify_distance(distance):
    if distance < 50:
        return 'Less than 50'
    elif 50 <= distance <= 150:
        return '51-150'
    elif 151 <= distance <= 300:
        return '151-300'
    elif 301 <= distance <= 500:
        return '301-500'
    else:
        return 'Other'

def filegeneration(o, r, t, output_file, mode):
    dfout = pd.read_csv(o, header=None, names=["HID", "HPosition", "HSpeed", "HTime"])
    dfrt = pd.read_csv(r, header=None, names=["MHID", "MDistance", "MTime", "Messages"])
    dftest = pd.read_csv(t, header=None, names=["VHID", "VPosition", "VSpeed", "VTime"])

    # Check if DataFrames are empty
    if dfout.empty or dfrt.empty or dftest.empty:
        print("One or more DataFrames are empty.")
        return

    dfrt['DistanceCategory'] = dfrt['MDistance'].apply(classify_distance)

    message_counts_distance = dfrt.groupby(['MHID', 'DistanceCategory'])['Messages'].count().unstack(fill_value=0)

    data_speed = {"Atk Average Speed": dfout["HSpeed"].mean(), "Vo Average Speed": dftest["VSpeed"].mean()}
    
    # Check if dfout and dftest have at least one row before accessing first and last elements
    if not dfout.empty:
        data_time = {"Atk Entry Time": dfout["HTime"].iloc[0], "Atk Exit Time": dfout["HTime"].iloc[-1], "AtK Sim Time": dfout["HTime"].iloc[-1]-dfout["HTime"].iloc[0]}
    else:
        data_time = {"Atk Entry Time": None, "Atk Exit Time": None, "AtK Sim Time": None}
    if not dftest.empty:
        data_time.update({"Vo Entry Time": dftest["VTime"].iloc[0], "Vo Exit Time": dftest["VTime"].iloc[-1], "Vo Sim Time": dftest["VTime"].iloc[-1]-dftest["VTime"].iloc[0]})
    else:
        data_time.update({"Vo Entry Time": None, "Vo Exit Time": None, "Vo Sim Time": None})

    data_messages = {"Attacker Messages": dfrt["Messages"].sum(), "Atk First Msg Time": dfrt["MTime"].iloc[0], "Atk Last Msg Time": dfrt["MTime"].iloc[-1], "AtK Msg Duration": dfrt["MTime"].iloc[-1]-dfrt["MTime"].iloc[0]}

    df_speed = pd.DataFrame(data_speed, index=[0])
    df_time = pd.DataFrame(data_time, index=[0])
    df_messages = pd.DataFrame(data_messages, index=[0])

    # Resetting index for all message counts dataframes
    message_counts_distance.reset_index(inplace=True)

    result_rows = []

    for _, row in message_counts_distance.iterrows():
        less_than_50 = 0 if 'Less than 50' not in row else row['Less than 50']
        range_51_150 = 0 if '51-150' not in row else row['51-150']
        range_151_300 = 0 if '151-300' not in row else row['151-300']
        range_301_500 = 0 if '301-500' not in row else row['301-500']

        result_rows.append({"MHID": row["MHID"], "Less than 50": less_than_50,
                            "51-150": range_51_150, "151-300": range_151_300,
                            "301-500": range_301_500})

    result = pd.concat([df_speed, df_time, df_messages, pd.DataFrame(result_rows)], axis=1)

    if mode == 'w':
        result.to_csv(output_file, index=False, header=True)
    elif mode == 'a':
        result.to_csv(output_file, mode='a', index=False, header=False)


# Main script
#/home/sidharth/Downloads/45 mph/vehsPerHour_400_400/
#/home/sidharth/Downloads/45 mph/vehsPerHour_400_500/
#/home/sidharth/Downloads/45 mph/vehsPerHour_400_700/
#/home/sidharth/Downloads/45 mph/vehsPerHour_400_900/
#/home/sidharth/Downloads/45 mph/vehsPerHour_400_1100/
#/home/sidharth/Downloads/45 mph/vehsPerHour_500_400/
#/home/sidharth/Downloads/45 mph/vehsPerHour_500_500/
#/home/sidharth/Downloads/45 mph/vehsPerHour_500_700/
#/home/sidharth/Downloads/45 mph/vehsPerHour_500_900/
#/home/sidharth/Downloads/45 mph/vehsPerHour_500_1100/
#/home/sidharth/Downloads/45 mph/vehsPerHour_700_400/
#/home/sidharth/Downloads/45 mph/vehsPerHour_700_500/
#/home/sidharth/Downloads/45 mph/vehsPerHour_700_700/
#/home/sidharth/Downloads/45 mph/vehsPerHour_700_900/
#/home/sidharth/Downloads/45 mph/vehsPerHour_700_1100/
#/home/sidharth/Downloads/45 mph/vehsPerHour_900_400/
#/home/sidharth/Downloads/45 mph/vehsPerHour_900_500/
#/home/sidharth/Downloads/45 mph/vehsPerHour_900_700/
#/home/sidharth/Downloads/45 mph/vehsPerHour_900_900/
#/home/sidharth/Downloads/45 mph/vehsPerHour_900_1100/
#/home/sidharth/Downloads/45 mph/vehsPerHour_1100_400/
#/home/sidharth/Downloads/45 mph/vehsPerHour_1100_500/
#/home/sidharth/Downloads/45 mph/vehsPerHour_1100_700/
#/home/sidharth/Downloads/45 mph/vehsPerHour_1100_900/
#/home/sidharth/Downloads/45 mph/vehsPerHour_1100_1100/

folder_path = '/home/sidharth/Downloads/45 mph/vehsPerHour_700_1100/'

output_folder_path = '/home/sidharth/Downloads/45 mph/vehsPerHour_700_1100/'

for folder_name in os.listdir(folder_path):
    folder_full_path = os.path.join(folder_path, folder_name)
    if os.path.isdir(folder_full_path):
        result_file = os.path.join(output_folder_path, folder_name + '.csv')  # Define the result file path

        # Run file generation for each iteration within the current folder
        for i in range(1, 51):  # Loop for 2 iterations
            if i == 1:
                filegeneration(os.path.join(folder_full_path, f"out_{i}.txt"),
                               os.path.join(folder_full_path, f"rt_{i}.txt"),
                               os.path.join(folder_full_path, f"test_{i}.txt"),
                               result_file, mode='w')  # Write mode for the first iteration
            else:
                filegeneration(os.path.join(folder_full_path, f"out_{i}.txt"),
                               os.path.join(folder_full_path, f"rt_{i}.txt"),
                               os.path.join(folder_full_path, f"test_{i}.txt"),
                               result_file, mode='a')  # Append mode for subsequent iterations





