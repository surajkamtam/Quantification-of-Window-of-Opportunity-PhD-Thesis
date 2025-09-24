import os
import pandas as pd
import numpy as np
import re

def dataseq(base_path):
    sequences = {}
    
    # Regex patterns to match the modules (with any target number)
    mobility_pattern = re.compile(r'RSUExampleScenario3\.target\[\d+\]\.veinsmobility$')
    appl_pattern = re.compile(r'RSUExampleScenario3\.target\[\d+\]\.appl$')

    for dep in range(895, 906):  # Process depart_895 to depart_905
        folder = os.path.join(base_path, f'depart_{dep}')
        
        for iter_num in range(1, 31):  # Process iter_1 to iter_30
            file_path = os.path.join(folder, f'PeriodicAttack_depart_{dep}_iter_{iter_num}_vector.csv')
            print(file_path)
            if not os.path.exists(file_path):
                print(f"⚠️ File not found: {file_path}")
                continue

            try:
                df = pd.read_csv(file_path)
            except Exception as e:
                print(f"❌ Error reading {file_path}: {e}")
                continue

            # Step 1: Extract anomaly timestamps (only where vecvalue=1)
            anomaly_times = set()
            anomaly_data = df[
                (df['type'] == 'vector') &
                (df['module'].str.match(appl_pattern)) &
                (df['name'] == 'Target Received Messages Timestamp')
            ]
            
            for _, row in anomaly_data.iterrows():
                try:
                    times = [int(float(t)) for t in row['vectime'].split()]
                    values = [int(float(v)) for v in row['vecvalue'].split()]
                    
                    # Only add times where the corresponding value is 1
                    for t, v in zip(times, values):
                        if v == 1:
                            anomaly_times.add(t)
                except Exception as e:
                    print(f"⚠️ Error parsing anomaly data in {file_path}: {e}")

            # Step 2: Extract speed data
            speed_data = df[
                (df['type'] == 'vector') &
                (df['module'].str.match(mobility_pattern)) &
                (df['name'] == 'speed')
            ]
            
            # Collect all time-speed pairs
            time_speed_pairs = []
            for _, row in speed_data.iterrows():
                try:
                    times = [int(float(t)) for t in row['vectime'].split()]
                    speeds = [float(v) for v in row['vecvalue'].split()]
                    
                    if len(times) == len(speeds):
                        time_speed_pairs.extend(zip(times, speeds))
                except Exception as e:
                    print(f"⚠️ Error parsing speed data in {file_path}: {e}")

            # Step 3: Create aligned sequence
            if time_speed_pairs:
                # Remove duplicate times (keep first occurrence)
                unique_pairs = {}
                for t, s in time_speed_pairs:
                    if t not in unique_pairs:
                        unique_pairs[t] = s
                
                # Convert to sorted lists
                times = sorted(unique_pairs.keys())
                speeds = [unique_pairs[t] for t in times]
                labels = [1 if t in anomaly_times else 0 for t in times]
                
                # Store as numpy arrays
                sequences[(dep, iter_num)] = (
                    np.array(times, dtype=int),
                    np.array(speeds, dtype=float),
                    np.array(labels, dtype=int)
                )
            else:
                print(f"⚠️ No speed data found in {file_path}")

    return sequences
