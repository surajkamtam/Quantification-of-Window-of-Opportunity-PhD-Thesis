import os
import re
import numpy as np
import pandas as pd
from tensorflow.keras.preprocessing.sequence import pad_sequences
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import (Conv1D, MaxPooling1D, LSTM, 
                                   TimeDistributed, Dense, Input, Reshape)
from tensorflow.keras.optimizers import Adam
from tensorflow.keras import backend as K
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.metrics import (mean_absolute_error, precision_score, 
                           recall_score, f1_score, confusion_matrix)
import seaborn as sns

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

def prepare_prediction_data(sequences, look_back=18, look_forward=100):
    X, y = [], []
    for seq in sequences.values():
        for i in range(len(seq) - look_back - look_forward):
            X.append(seq[i:i+look_back])
            y.append(seq[i+look_back:i+look_back+look_forward])
    return np.array(X), np.array(y)

def f1(y_true, y_pred):
    def recall(y_true, y_pred):
        true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
        possible_positives = K.sum(K.round(K.clip(y_true, 0, 1)))
        recall = true_positives / (possible_positives + K.epsilon())
        return recall

    def precision(y_true, y_pred):
        true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
        predicted_positives = K.sum(K.round(K.clip(y_pred, 0, 1)))
        precision = true_positives / (predicted_positives + K.epsilon())
        return precision
    
    precision = precision(y_true, y_pred)
    recall = recall(y_true, y_pred)
    return 2*((precision*recall)/(precision+recall+K.epsilon()))

def build_prediction_model(input_shape, output_steps, n_features):
    model = Sequential([
        Input(shape=input_shape),
        Conv1D(filters=32, kernel_size=3, activation='relu', padding='same'),
        MaxPooling1D(pool_size=2),
        LSTM(64, return_sequences=True),
        LSTM(32),
        Dense(32, activation='relu'),
        Dense(output_steps * n_features),
        Reshape((output_steps, n_features))
    ])
    model.compile(optimizer=Adam(learning_rate=0.001), 
                 loss='mse',
                 metrics=['mae', f1])
    return model

def calculate_classification_metrics(y_true, y_pred, threshold=0.5):
    # Convert to binary classification using threshold
    y_true_bin = (y_true > threshold).astype(int)
    y_pred_bin = (y_pred > threshold).astype(int)
    
    precision = precision_score(y_true_bin.flatten(), y_pred_bin.flatten(), average='weighted')
    recall = recall_score(y_true_bin.flatten(), y_pred_bin.flatten(), average='weighted')
    f1 = f1_score(y_true_bin.flatten(), y_pred_bin.flatten(), average='weighted')
    
    return precision, recall, f1

def plot_confusion_matrix(y_true, y_pred, threshold=0.5):
    y_true_bin = (y_true > threshold).astype(int)
    y_pred_bin = (y_pred > threshold).astype(int)
    
    cm = confusion_matrix(y_true_bin.flatten(), y_pred_bin.flatten())
    plt.figure(figsize=(6, 6))
    sns.heatmap(cm, annot=True, fmt='d', cmap='Blues')
    plt.title('Confusion Matrix')
    plt.xlabel('Predicted')
    plt.ylabel('Actual')
    plt.savefig('confusion_matrix.png')
    plt.close()

def plot_sequence_predictions(model, X_test, y_test, n_samples=3, look_back=18, look_forward=100):
    plt.figure(figsize=(15, 5*n_samples))
    indices = np.random.choice(len(X_test), n_samples, replace=False)
    
    for i, idx in enumerate(indices):
        input_seq = X_test[idx:idx+1]
        actual = y_test[idx]
        predicted = model.predict(input_seq)[0]
        
        # Calculate metrics for this sample
        prec, rec, f1 = calculate_classification_metrics(actual, predicted)
        
        # Create time axis
        total_steps = look_back + look_forward
        time_axis = np.arange(total_steps)
        
        # Plot each feature
        n_features = input_seq.shape[-1]
        feature_names = ['speed', 'acceleration']
        
        for f in range(n_features):
            plt.subplot(n_samples, n_features, i*n_features + f + 1)
            plt.plot(time_axis[:look_back], input_seq[0, :, f], 'b-', label='Input')
            plt.plot(time_axis[look_back:], actual[:, f], 'g-', label='Actual')
            plt.plot(time_axis[look_back:], predicted[:, f], 'r--', 
                    label=f'Predicted\nP: {prec:.2f}, R: {rec:.2f}, F1: {f1:.2f}')
            
            plt.title(f'Sample {i+1} - {feature_names[f]}')
            plt.xlabel('Time step')
            plt.ylabel('Value')
            plt.legend()
            plt.grid(True)
    
    plt.tight_layout()
    plt.savefig('sequence_predictions.png')
    plt.close()

def plot_training_history(history):
    plt.figure(figsize=(15, 5))
    
    # Plot loss
    plt.subplot(1, 3, 1)
    plt.plot(history.history['loss'], label='Train Loss')
    plt.plot(history.history['val_loss'], label='Val Loss')
    plt.title('Model Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss (MSE)')
    plt.legend()
    plt.grid(True)
    
    # Plot MAE
    plt.subplot(1, 3, 2)
    plt.plot(history.history['mae'], label='Train MAE')
    plt.plot(history.history['val_mae'], label='Val MAE')
    plt.title('Model MAE')
    plt.xlabel('Epoch')
    plt.ylabel('MAE')
    plt.legend()
    plt.grid(True)
    
    # Plot F1 Score
    plt.subplot(1, 3, 3)
    plt.plot(history.history['f1'], label='Train F1')
    plt.plot(history.history['val_f1'], label='Val F1')
    plt.title('Model F1 Score')
    plt.xlabel('Epoch')
    plt.ylabel('F1 Score')
    plt.legend()
    plt.grid(True)
    
    plt.tight_layout()
    plt.savefig('training_history.png')
    plt.close()

def train_sequence_model(sequences):
    print("[INFO] Preparing prediction data...")
    look_back = 18
    look_forward = 100
    X, y = prepare_prediction_data(sequences, look_back, look_forward)
    
    print(f"[INFO] Input shape: {X.shape}, Target shape: {y.shape}")
    
    print("[INFO] Splitting data...")
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=0.3, random_state=42
    )
    X_train, X_val, y_train, y_val = train_test_split(
        X_train, y_train, test_size=0.3, random_state=42
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
    test_loss, test_mae, test_f1 = model.evaluate(X_test, y_test)
    test_prec, test_rec, test_f1_calc = calculate_classification_metrics(y_test, model.predict(X_test))
    
    print("\n[FINAL RESULTS]")
    print(f"Test Loss (MSE): {test_loss:.4f}")
    print(f"Test MAE: {test_mae:.4f}")
    print(f"Test Precision: {test_prec:.4f}")
    print(f"Test Recall: {test_rec:.4f}")
    print(f"Test F1-Score: {test_f1_calc:.4f}")
    
    # Calculate training metrics
    train_prec, train_rec, train_f1 = calculate_classification_metrics(y_train, model.predict(X_train))
    print("\n[TRAINING METRICS]")
    print(f"Train Precision: {train_prec:.4f}")
    print(f"Train Recall: {train_rec:.4f}")
    print(f"Train F1-Score: {train_f1:.4f}")
    
    print("[INFO] Generating visualizations...")
    plot_sequence_predictions(model, X_test, y_test)
    plot_training_history(history)
    plot_confusion_matrix(y_test, model.predict(X_test))
    
    model.save_weights("sequence_prediction_model.weights.h5")
    print("[INFO] Model weights saved to sequence_prediction_model.weights.h5")
    
    return model

if __name__ == "__main__":
    folders_with_los = {
        './General_vehsPerHour_500/': 'A',
        #'./General_vehsPerHour_800/': 'B',
        #'./General_vehsPerHour_900/': 'C',
        #'./General_vehsPerHour_1100/': 'D',
        #'./General_vehsPerHour_1300/': 'E'
    }

    sequences = dataseq(folders_with_los)
    model = train_sequence_model(sequences)
