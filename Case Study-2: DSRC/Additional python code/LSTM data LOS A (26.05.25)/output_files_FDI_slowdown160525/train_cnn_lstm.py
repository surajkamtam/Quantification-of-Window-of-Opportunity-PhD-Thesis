import numpy as np
from sklearn.model_selection import train_test_split
from tensorflow.keras.preprocessing.sequence import pad_sequences
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv1D, MaxPooling1D, LSTM, Dense
from tensorflow.keras.optimizers import Adam

import os
import pandas as pd
import re
import matplotlib.pyplot as plt

def dataseq(base_path):
    sequences = {}
    
    mobility_pattern = re.compile(r'RSUExampleScenario3\.target\[\d+\]\.veinsmobility$')
    appl_pattern = re.compile(r'RSUExampleScenario3\.target\[\d+\]\.appl$')

    for dep in range(895, 906):
        folder = os.path.join(base_path, f'depart_{dep}')
        for iter_num in range(1, 31):
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

            speed_data = df[
                (df['type'] == 'vector') &
                (df['module'].str.match(mobility_pattern)) &
                (df['name'] == 'speed')
            ]

            time_speed_pairs = []
            for _, row in speed_data.iterrows():
                try:
                    times = [int(float(t)) for t in row['vectime'].split()]
                    speeds = [float(v) for v in row['vecvalue'].split()]
                    if len(times) == len(speeds):
                        time_speed_pairs.extend(zip(times, speeds))
                except Exception as e:
                    print(f"⚠️ Error parsing speed data in {file_path}: {e}")

            if time_speed_pairs:
                unique_pairs = {}
                for t, s in time_speed_pairs:
                    if t not in unique_pairs:
                        unique_pairs[t] = s

                times = sorted(unique_pairs.keys())
                speeds = [unique_pairs[t] for t in times]
                labels = [1 if t in anomaly_times else 0 for t in times]

                sequences[(dep, iter_num)] = (
                    np.array(times, dtype=int),
                    np.array(speeds, dtype=float),
                    np.array(labels, dtype=int)
                )
            else:
                print(f"⚠️ No speed data found in {file_path}")

    return sequences

def load_data(sequences, available_sequences):
    all_speeds = []
    all_labels = []
    for i in available_sequences:
        _, speeds, label = sequences[i]
        all_speeds.append(speeds)
        all_labels.append(label)
    return all_speeds, all_labels

def pad_time_series(sequences, maxlen=None):
    return pad_sequences(sequences, padding='post', dtype='float32', maxlen=maxlen)

def split_data(X, y, test_size=0.3, val_size=0.5):
    # Flatten labels to 1D (from shape (samples, timesteps)) by majority label in sequence
    y_flat = np.array([1 if np.sum(label) > 0 else 0 for label in y])  # label = 1 if anomaly anywhere in sequence

    X_train, X_temp, y_train, y_temp = train_test_split(
        X, y_flat, test_size=test_size, stratify=y_flat, random_state=42
    )
    X_val, X_test, y_val, y_test = train_test_split(
        X_temp, y_temp, test_size=val_size, stratify=y_temp, random_state=42
    )
    return X_train, X_val, X_test, y_train, y_val, y_test

def build_model(input_shape):
    model = Sequential([
        Conv1D(filters=64, kernel_size=3, activation='relu', input_shape=input_shape),
        MaxPooling1D(pool_size=2),
        LSTM(64),
        Dense(32, activation='relu'),
        Dense(1, activation='sigmoid')
    ])
    model.compile(optimizer=Adam(learning_rate=0.001), loss='binary_crossentropy', metrics=['accuracy'])
    return model

def plot_history(history):
    # Accuracy plot
    plt.figure(figsize=(10,4))
    plt.plot(history.history['accuracy'], label='Train Accuracy')
    plt.plot(history.history['val_accuracy'], label='Validation Accuracy')
    plt.title('Model Accuracy')
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy')
    plt.legend()
    plt.grid(True)
    plt.savefig('accuracy_plot.png')

    # Loss plot
    plt.figure(figsize=(10,4))
    plt.plot(history.history['loss'], label='Train Loss')
    plt.plot(history.history['val_loss'], label='Validation Loss')
    plt.title('Model Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.legend()
    plt.grid(True)
    plt.savefig('loss_plot.png')

def train_cnn_lstm_model(sequences, available_sequences):
    print("[INFO] Loading data...")
    X_raw, y_raw = load_data(sequences, available_sequences)
    
    print("[INFO] Padding sequences...")
    X_padded = pad_time_series(X_raw)
    X_padded = np.expand_dims(X_padded, axis=-1)  # (samples, timesteps, 1)

    print("[INFO] Splitting data...")
    X_train, X_val, X_test, y_train, y_val, y_test = split_data(X_padded, y_raw)

    print("[INFO] Building model...")
    model = build_model(input_shape=X_train.shape[1:])
    model.summary()

    print("[INFO] Training model...")
    history = model.fit(
        X_train, y_train,
        validation_data=(X_val, y_val),
        epochs=20,
        batch_size=32,
        verbose=1
    )

    print("[INFO] Evaluating model...")
    test_loss, test_acc = model.evaluate(X_test, y_test)
    print(f"[RESULT] Test Accuracy: {test_acc:.4f}")

    # Save weights with correct extension
    weights_path = "cnn_lstm_model_weights.weights.h5"
    model.save_weights(weights_path)
    print(f"[INFO] Model weights saved to {weights_path}")

    # Plot training history
    plot_history(history)
    print("[INFO] Training plots saved as 'accuracy_plot.png' and 'loss_plot.png'")

    return model

if __name__ == "__main__":
    sequences = dataseq('./PeriodicAttack_vehsPerHour_500/')
    available_sequences = list(sequences.keys())
    model = train_cnn_lstm_model(sequences, available_sequences)

