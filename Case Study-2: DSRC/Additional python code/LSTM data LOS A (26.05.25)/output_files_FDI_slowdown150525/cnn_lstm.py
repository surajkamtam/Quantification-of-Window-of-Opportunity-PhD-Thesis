import os
import re
import numpy as np
import pandas as pd
from tensorflow.keras.preprocessing.sequence import pad_sequences
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv1D, MaxPooling1D, LSTM, TimeDistributed, Dense, Input
from tensorflow.keras.optimizers import Adam
import matplotlib.pyplot as plt

def dataseq(folders_with_los):
    sequences = {}
    
    mobility_pattern = re.compile(r'RSUExampleScenario3\.target\[\d+\]\.veinsmobility$')
    appl_pattern = re.compile(r'RSUExampleScenario3\.target\[\d+\]\.appl$')

    for base_path, los in folders_with_los.items():
        print(f"[INFO] Processing folder {base_path} with LOS={los}")
        for dep in range(895, 906):
            folder = os.path.join(base_path, f'depart_{dep}')
            for iter_num in range(1, 31):
                file_path = os.path.join(folder, f'General_depart_{dep}_iter_{iter_num}_vector.csv')
                if not os.path.exists(file_path):
                    print(f"⚠️ File not found: {file_path}")
                    continue

                try:
                    df = pd.read_csv(file_path)
                except Exception as e:
                    print(f"❌ Error reading {file_path}: {e}")
                    continue

                # Extract anomaly timestamps
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
                        for t, v in zip(times, values):
                            if v == 1:
                                anomaly_times.add(t)
                    except Exception as e:
                        print(f"⚠️ Error parsing anomaly data in {file_path}: {e}")

                # Extract features: speed, posx, posy, acceleration
                features_data = {}
                feature_names = ['speed', 'acceleration']
                for feature in feature_names:
                    feature_rows = df[
                        (df['type'] == 'vector') &
                        (df['module'].str.match(mobility_pattern)) &
                        (df['name'] == feature)
                    ]
                    times_vals = []
                    for _, row in feature_rows.iterrows():
                        try:
                            times = [int(float(t)) for t in row['vectime'].split()]
                            vals = [float(v) for v in row['vecvalue'].split()]
                            if len(times) == len(vals):
                                times_vals.append((times, vals))
                        except Exception as e:
                            print(f"⚠️ Error parsing {feature} data in {file_path}: {e}")

                    # Merge all times/values from multiple rows for this feature
                    all_time_vals = {}
                    for t_list, v_list in times_vals:
                        for t, v in zip(t_list, v_list):
                            if t not in all_time_vals:
                                all_time_vals[t] = v
                    features_data[feature] = all_time_vals

                # Align all times across features
                all_times = set()
                for feature_dict in features_data.values():
                    all_times.update(feature_dict.keys())
                all_times = sorted(all_times)

                if not all_times:
                    print(f"⚠️ No feature data found in {file_path}")
                    continue

                # Build feature matrix (times x features)
                feature_matrix = []
                for t in all_times:
                    row = []
                    for feature in feature_names:
                        row.append(features_data[feature].get(t, 0.0))  # fill missing with 0.0
                    feature_matrix.append(row)

                # Build label vector
                labels = [1 if t in anomaly_times else 0 for t in all_times]

                sequences[(los, dep, iter_num)] = (
                    np.array(all_times, dtype=int),
                    np.array(feature_matrix, dtype=float),
                    np.array(labels, dtype=int)
                )

    if len(sequences) == 0:
        raise RuntimeError("No sequences loaded! Check your data paths and files.")

    print(f"[INFO] Loaded total sequences: {len(sequences)}")
    return sequences


def load_data(sequences, available_sequences):
    all_features = []
    all_labels = []
    for key in available_sequences:
        if key not in sequences:
            print(f"⚠️ Warning: sequence key {key} not found in loaded sequences")
            continue
        _, features, labels = sequences[key]
        all_features.append(features)
        all_labels.append(labels)
    print(f"[INFO] Loaded {len(all_features)} sequences for training")
    return all_features, all_labels


def pad_time_series(sequences, maxlen=None):
    # sequences is list of (timesteps x features) arrays
    # pad along time dimension (axis=0)
    padded = pad_sequences(sequences, padding='post', dtype='float32', maxlen=maxlen)
    return padded


def split_data(X, y, test_size=0.3, val_size=0.5):
    # Flatten labels to 1D (from shape (samples, timesteps)) by majority label in sequence
    y_flat = np.array([1 if np.sum(label) > 0 else 0 for label in y])

    from sklearn.model_selection import train_test_split
    X_train, X_temp, y_train, y_temp = train_test_split(
        X, y_flat, test_size=test_size, stratify=y_flat, random_state=42
    )
    X_val, X_test, y_val, y_test = train_test_split(
        X_temp, y_temp, test_size=val_size, stratify=y_temp, random_state=42
    )
    return X_train, X_val, X_test, y_train, y_val, y_test



def build_sequence_model(input_shape, output_features):
    model = Sequential([
        Input(shape=(None, input_shape[-1])),  # Accept variable-length sequences
        Conv1D(filters=16, kernel_size=3, activation='relu'),
        MaxPooling1D(pool_size=2),
        LSTM(16, return_sequences=True),
        TimeDistributed(Dense(output_features))
    ])
    model.compile(optimizer=Adam(learning_rate=0.0001), loss='mse')
    return model


def plot_history(history):
    plt.figure(figsize=(10, 4))
    plt.plot(history.history['loss'], label='Train Loss')
    plt.plot(history.history.get('val_loss', []), label='Val Loss')
    plt.title('Model Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.legend()
    plt.grid(True)
    plt.savefig('loss_plot.png')


def train_sequence_model(sequences, available_sequences):
    print("[INFO] Loading data...")
    X_raw, y_raw = load_data(sequences, available_sequences)

    if len(X_raw) == 0:
        raise RuntimeError("No data loaded for training.")

    print("[INFO] Padding sequences...")
    X_padded = pad_time_series(X_raw)
    y_padded = pad_time_series(y_raw, maxlen=X_padded.shape[1])

    # Expand dims for Conv1D input: (samples, timesteps, features)
    X_padded = np.array(X_padded)
    y_padded = np.array(y_padded)
    print(f"[INFO] Feature input shape: {X_padded.shape}")
    print(f"[INFO] Label output shape: {y_padded.shape}")

    print("[INFO] Splitting data...")
    X_train, X_val, X_test, y_train, y_val, y_test = split_data(X_padded, y_padded)

    print("[INFO] Building model...")
    input_shape = X_train.shape[1:]  # (timesteps, features)
    output_features = y_train.shape[1] if len(y_train.shape) > 1 else 1
    # In case labels have features dimension, else 1
    if len(y_train.shape) == 3:
        output_features = y_train.shape[2]
    else:
        output_features = 1

    model = build_sequence_model(input_shape, output_features)
    model.summary()

    print("[INFO] Training model...")
    history = model.fit(
        X_train, y_train,
        validation_data=(X_val, y_val),
        epochs=10,
        batch_size=32,
        verbose=1
    )

    print("[INFO] Evaluating model...")
    test_loss = model.evaluate(X_test, y_test)
    print(f"[RESULT] Test Loss: {test_loss:.4f}")

    model.save_weights("cnn_lstm_sequence_model.weights.h5")
    print("[INFO] Model weights saved to cnn_lstm_sequence_model.weights.h5")

    plot_history(history)
    print("[INFO] Training loss plot saved as 'loss_plot.png'")

    return model


if __name__ == "__main__":
    # Define base folders and corresponding LOS values
    folders_with_los = {
        './General_vehsPerHour_500/': 'A',
        './General_vehsPerHour_800/': 'B',
        './General_vehsPerHour_900/': 'C'
    }

    sequences = dataseq(folders_with_los)
    available_sequences = list(sequences.keys())

    model = train_sequence_model(sequences, available_sequences)

