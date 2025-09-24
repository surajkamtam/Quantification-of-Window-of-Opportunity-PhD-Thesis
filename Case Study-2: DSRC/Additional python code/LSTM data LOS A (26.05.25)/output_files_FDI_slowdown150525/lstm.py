import os
import re
import numpy as np
import pandas as pd
from tensorflow.keras.preprocessing.sequence import pad_sequences
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv1D, MaxPooling1D, LSTM, TimeDistributed, Dense, Input, Reshape
from tensorflow.keras.optimizers import Adam
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split

def dataseq(folders_with_los):
    sequences = {}
    
    mobility_pattern = re.compile(r'RSUExampleScenario3\.target\[\d+\]\.veinsmobility$')

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

                # Extract features: speed, acceleration
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

                sequences[(los, dep, iter_num)] = np.array(feature_matrix, dtype=float)

    if len(sequences) == 0:
        raise RuntimeError("No sequences loaded! Check your data paths and files.")

    print(f"[INFO] Loaded total sequences: {len(sequences)}")
    return sequences


def prepare_prediction_data(sequences, look_back=10, look_forward=5):
    """
    Prepare data for sequence prediction:
    - look_back: number of timesteps to use as input
    - look_forward: number of timesteps to predict
    """
    X, y = [], []
    for seq in sequences.values():
        for i in range(len(seq) - look_back - look_forward):
            X.append(seq[i:i+look_back])
            y.append(seq[i+look_back:i+look_back+look_forward])
    return np.array(X), np.array(y)


def pad_sequences_to_length(sequences, maxlen):
    """Pad sequences to a fixed length"""
    padded = np.zeros((len(sequences), maxlen, sequences[0].shape[-1]))
    for i, seq in enumerate(sequences):
        padded[i, :len(seq)] = seq
    return padded


def build_prediction_model(input_shape, output_steps, n_features):
    """
    Build model for sequence prediction
    - input_shape: (look_back, n_features)
    - output_steps: look_forward
    - n_features: number of features
    """
    model = Sequential([
        Input(shape=input_shape),
        Conv1D(filters=64, kernel_size=3, activation='relu', padding='same'),
        MaxPooling1D(pool_size=2),
        LSTM(64, return_sequences=True),
        LSTM(64),
        Dense(64, activation='relu'),
        Dense(output_steps * n_features),
        Reshape((output_steps, n_features))
    ])
    model.compile(optimizer=Adam(learning_rate=0.0001), loss='mse')
    return model


def plot_sequence_predictions(model, X_test, y_test, n_samples=3, look_back=10, look_forward=5):
    """Visualize predictions vs actual sequences"""
    plt.figure(figsize=(15, 5*n_samples))
    
    # Select random samples
    indices = np.random.choice(len(X_test), n_samples, replace=False)
    
    for i, idx in enumerate(indices):
        # Get prediction
        input_seq = X_test[idx:idx+1]
        actual = y_test[idx]
        predicted = model.predict(input_seq)[0]
        
        # Create time axis
        total_steps = look_back + look_forward
        time_axis = np.arange(total_steps)
        
        # Plot each feature
        n_features = input_seq.shape[-1]
        feature_names = ['speed', 'acceleration']
        
        for f in range(n_features):
            plt.subplot(n_samples, n_features, i*n_features + f + 1)
            
            # Plot input sequence
            plt.plot(time_axis[:look_back], input_seq[0, :, f], 'b-', label='Input')
            
            # Plot actual continuation
            plt.plot(time_axis[look_back:], actual[:, f], 'g-', label='Actual')
            
            # Plot predicted continuation
            plt.plot(time_axis[look_back:], predicted[:, f], 'r--', label='Predicted')
            
            plt.title(f'Sample {i+1} - {feature_names[f]}')
            plt.xlabel('Time step')
            plt.ylabel('Value')
            plt.legend()
            plt.grid(True)
    
    plt.tight_layout()
    plt.savefig('sequence_predictions.png')
    plt.close()


def train_sequence_model(sequences):
    print("[INFO] Preparing prediction data...")
    look_back = 10  # Use last 10 timesteps
    look_forward = 5  # Predict next 5 timesteps
    X, y = prepare_prediction_data(sequences, look_back, look_forward)
    
    print(f"[INFO] Input shape: {X.shape}, Target shape: {y.shape}")
    
    print("[INFO] Splitting data...")
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=0.2, random_state=42
    )
    X_train, X_val, y_train, y_val = train_test_split(
        X_train, y_train, test_size=0.2, random_state=42
    )
    
    print("[INFO] Building model...")
    n_features = X.shape[2]
    model = build_prediction_model(
        input_shape=(look_back, n_features),
        output_steps=look_forward,
        n_features=n_features
    )
    model.summary()
    
    print("[INFO] Training model...")
    history = model.fit(
        X_train, y_train,
        validation_data=(X_val, y_val),
        epochs=30,
        batch_size=32,
        verbose=1
    )
    
    print("[INFO] Evaluating model...")
    test_loss = model.evaluate(X_test, y_test)
    print(f"[RESULT] Test Loss: {test_loss:.4f}")
    
    print("[INFO] Generating prediction visualizations...")
    plot_sequence_predictions(model, X_test, y_test, look_back=look_back, look_forward=look_forward)
    
    model.save_weights("sequence_prediction_model.weights.h5")
    print("[INFO] Model weights saved to sequence_prediction_model.weights.h5")
    
    plot_history(history)
    print("[INFO] Training loss plot saved as 'loss_plot.png'")
    
    return model


def plot_history(history):
    plt.figure(figsize=(10, 4))
    plt.plot(history.history['loss'], label='Train Loss')
    plt.plot(history.history['val_loss'], label='Val Loss')
    plt.title('Model Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.legend()
    plt.grid(True)
    plt.savefig('loss_plot.png')
    plt.close()


if __name__ == "__main__":
    # Define base folders and corresponding LOS values
    folders_with_los = {
        './General_vehsPerHour_500/': 'A',
        './General_vehsPerHour_800/': 'B',
        './General_vehsPerHour_900/': 'C'
    }

    sequences = dataseq(folders_with_los)
    model = train_sequence_model(sequences)
