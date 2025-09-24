import os
import numpy as np
import pandas as pd
import tensorflow as tf
from tensorflow.keras.models import Model
from tensorflow.keras.layers import (Input, Conv1D, MaxPooling1D, 
                                     LSTM, Bidirectional, Dense, 
                                     Reshape, Dropout, Attention, 
                                     Concatenate, LayerNormalization)
from tensorflow.keras.optimizers import Adam
from tensorflow.keras import backend as K
from sklearn.model_selection import train_test_split
from sklearn.metrics import (precision_score, recall_score, 
                             f1_score, confusion_matrix)
import matplotlib.pyplot as plt
import seaborn as sns

# ---------------------- Custom F1 Metric ----------------------
def f1(y_true, y_pred):
    def recall(y_true, y_pred):
        true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
        possible_positives = K.sum(K.round(K.clip(y_true, 0, 1)))
        return true_positives / (possible_positives + K.epsilon())

    def precision(y_true, y_pred):
        true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
        predicted_positives = K.sum(K.round(K.clip(y_pred, 0, 1)))
        return true_positives / (predicted_positives + K.epsilon())

    p = precision(y_true, y_pred)
    r = recall(y_true, y_pred)
    return 2 * ((p * r) / (p + r + K.epsilon()))

# ---------------------- Data Loader ----------------------
def load_sequences_from_csv(csv_file, look_back=15, look_forward=15):
    df = pd.read_csv(csv_file)
    grouped = df.groupby(['LOS', 'DepartureTime', 'Iteration'])
    X, y = [], []

    for _, group in grouped:
        group = group.sort_values(by='Time')
        features = group[['Speed', 'Acceleration', 'Jerk']].values
        for i in range(len(features) - look_back - look_forward):
            X.append(features[i:i+look_back])
            y.append(features[i+look_back:i+look_back+look_forward])
    return np.array(X), np.array(y)

# ---------------------- Model Builder ----------------------
def build_prediction_model(input_shape, output_steps, n_features):
    inputs = Input(shape=input_shape)

    x = Conv1D(filters=32, kernel_size=3, activation='relu', padding='same')(inputs)
    x = MaxPooling1D(pool_size=2)(x)
    x = Bidirectional(LSTM(64, return_sequences=True))(x)

    attention = Attention()([x, x])  # Self-attention
    x = Concatenate()([x, attention])
    x = LayerNormalization()(x)
    x = Dropout(0.2)(x)

    x = LSTM(32)(x)
    x = Dense(64, activation='relu')(x)
    x = Dense(output_steps * n_features)(x)
    outputs = Reshape((output_steps, n_features))(x)

    model = Model(inputs, outputs)
    model.compile(optimizer=Adam(0.001), loss='mse', metrics=['mae', f1])
    return model

# ---------------------- Metrics & Visualization ----------------------
def calculate_classification_metrics(y_true, y_pred, threshold=0.5):
    y_true_bin = (y_true > threshold).astype(int)
    y_pred_bin = (y_pred > threshold).astype(int)

    precision = precision_score(y_true_bin.flatten(), y_pred_bin.flatten(), average='weighted')
    recall = recall_score(y_true_bin.flatten(), y_pred_bin.flatten(), average='weighted')
    f1_val = f1_score(y_true_bin.flatten(), y_pred_bin.flatten(), average='weighted')
    return precision, recall, f1_val

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

def plot_sequence_predictions(model, X_test, y_test, n_samples=3, look_back=15, look_forward=15):
    plt.figure(figsize=(15, 5 * n_samples))
    indices = np.random.choice(len(X_test), n_samples, replace=False)

    for i, idx in enumerate(indices):
        input_seq = X_test[idx:idx+1]
        actual = y_test[idx]
        predicted = model.predict(input_seq)[0]

        prec, rec, f1_val = calculate_classification_metrics(actual, predicted)
        time_axis = np.arange(look_back + look_forward)
        feature_names = ['Speed', 'Acceleration', 'Jerk']

        for f in range(input_seq.shape[-1]):
            plt.subplot(n_samples, input_seq.shape[-1], i * input_seq.shape[-1] + f + 1)
            plt.plot(time_axis[:look_back], input_seq[0, :, f], 'b-', label='Input')
            plt.plot(time_axis[look_back:], actual[:, f], 'g-', label='Actual')
            plt.plot(time_axis[look_back:], predicted[:, f], 'r--', 
                     label=f'Predicted\nP: {prec:.2f}, R: {rec:.2f}, F1: {f1_val:.2f}')
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

    plt.subplot(1, 3, 1)
    plt.plot(history.history['loss'], label='Train Loss')
    plt.plot(history.history['val_loss'], label='Val Loss')
    plt.title('Loss')
    plt.xlabel('Epoch')
    plt.ylabel('MSE')
    plt.grid(True)
    plt.legend()

    plt.subplot(1, 3, 2)
    plt.plot(history.history['mae'], label='Train MAE')
    plt.plot(history.history['val_mae'], label='Val MAE')
    plt.title('MAE')
    plt.xlabel('Epoch')
    plt.ylabel('Mean Abs Error')
    plt.grid(True)
    plt.legend()

    plt.subplot(1, 3, 3)
    plt.plot(history.history['f1'], label='Train F1')
    plt.plot(history.history['val_f1'], label='Val F1')
    plt.title('F1 Score')
    plt.xlabel('Epoch')
    plt.ylabel('F1 Score')
    plt.grid(True)
    plt.legend()

    plt.tight_layout()
    plt.savefig('training_history.png')
    plt.close()

# ---------------------- Training Pipeline ----------------------
def train_sequence_model(csv_path='sequence_data_general.csv'):
    print("[INFO] Loading sequence data with jerk from CSV...")
    look_back = 15
    look_forward = 15

    X, y = load_sequences_from_csv(csv_path, look_back, look_forward)
    print(f"[INFO] Loaded data: {X.shape}, {y.shape}")

    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)
    X_train, X_val, y_train, y_val = train_test_split(X_train, y_train, test_size=0.3, random_state=42)

    n_features = X.shape[2]
    model = build_prediction_model((look_back, n_features), look_forward, n_features)
    model.summary()

    print("[INFO] Training model...")
    history = model.fit(
        X_train, y_train,
        validation_data=(X_val, y_val),
        epochs=100,
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
    print(f"Test F1 Score: {test_f1_calc:.4f}")

    train_prec, train_rec, train_f1 = calculate_classification_metrics(y_train, model.predict(X_train))
    print("\n[TRAINING METRICS]")
    print(f"Train Precision: {train_prec:.4f}")
    print(f"Train Recall: {train_rec:.4f}")
    print(f"Train F1 Score: {train_f1:.4f}")

    print("[INFO] Saving visualizations and model...")
    plot_sequence_predictions(model, X_test, y_test)
    plot_training_history(history)
    plot_confusion_matrix(y_test, model.predict(X_test))
    model.save_weights("sequence_prediction_model.weights.h5")
    print("[INFO] Model weights saved!")

    return model

# ---------------------- Main Entry ----------------------
if __name__ == "__main__":
    model = train_sequence_model('sequence_data_general.csv')
