import numpy as np
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv1D, MaxPooling1D, LSTM, TimeDistributed, Dense, Input
import matplotlib.pyplot as plt

# =============================================
# MODEL LOADING (Updated for 2 features)
# =============================================
def load_trained_model(input_shape=(None, 2), output_features=2):
    model = Sequential([
        Input(shape=input_shape),
        Conv1D(filters=64, kernel_size=3, activation='relu'),
        MaxPooling1D(pool_size=2),
        LSTM(64, return_sequences=True),
        TimeDistributed(Dense(output_features))
    ])
    model.compile(optimizer='adam', loss='mse')
    model.load_weights("cnn_lstm_sequence_model2.weights.h5")
    return model

# =============================================
# ANOMALY DETECTION (Now uses both features)
# =============================================
def detect_anomalies(model, sequence, threshold=None):
    times, features, true_labels = sequence

    # Convert dict to array
    if isinstance(features, dict):
        ordered_keys = sorted(features.keys())  # Ensure consistent order
        features_array = np.stack([features[k] for k in ordered_keys], axis=-1)
    else:
        features_array = np.array(features)

    print(f"features_array.shape after conversion: {features_array.shape}")

    # Ensure correct shape
    if features_array.ndim == 2 and features_array.shape[1] != 2:
        if features_array.shape[0] == 2:
            features_array = features_array.T

    if features_array.shape[1] != 2:
        raise ValueError(f"Features must be shape (timesteps, 2), got {features_array.shape}")

    # Handle NaNs
    nan_mask = np.isnan(features_array)
    if np.any(nan_mask):
        print(f"⚠️ Detected {np.sum(nan_mask)} NaN values in features. Replacing with column mean.")
        col_means = np.nanmean(features_array, axis=0)
        inds = np.where(nan_mask)
        features_array[inds] = np.take(col_means, inds[1])

    # Predict using model
    X = np.expand_dims(features_array, axis=0)  # shape: (1, timesteps, 2)
    predictions = model.predict(X, verbose=0)[0]  # shape: (timesteps, 2)
    pred_len = predictions.shape[0]

    # Downsample input to match prediction shape
    downsample_rate = max(1, int(np.floor(features_array.shape[0] / pred_len)))
    features_downsampled = features_array[::downsample_rate][:pred_len]
    times_downsampled = times[::downsample_rate][:pred_len]
    true_labels_downsampled = true_labels[::downsample_rate][:pred_len]

    print(f"features_downsampled.shape: {features_downsampled.shape}")
    print(f"predictions.shape: {predictions.shape}")

    # Compute reconstruction error across all features
    errors = np.mean(np.square(features_downsampled - predictions), axis=-1)

    if np.any(np.isnan(errors)):
        raise ValueError("NaN encountered in prediction errors. Check your input or model output.")

    if threshold is None:
        threshold = np.percentile(errors, 95)

    anomalies = errors > threshold

    return anomalies, errors, threshold, times_downsampled, features_downsampled, true_labels_downsampled, predictions

# =============================================
# VISUALIZATION
# =============================================
def plot_results(times, features, errors, anomalies, true_labels, threshold):
    plt.figure(figsize=(15, 8))
    n_features = features.shape[1]

    for i in range(n_features):
        plt.plot(times, features[:, i] / np.max(features[:, i]) - i * 1.1,
                 label=f'Feature {i}', alpha=0.7)

    plt.plot(times, errors, 'k-', label='Prediction Error', linewidth=2)
    plt.axhline(threshold, color='r', linestyle='--', label='Anomaly Threshold')

    anomaly_times = times[anomalies]
    plt.scatter(anomaly_times, [threshold] * np.sum(anomalies),
                color='r', marker='x', s=100, label='Detected Anomalies')

    if true_labels is not None and np.any(true_labels == 1):
        true_anomaly_times = times[true_labels == 1]
        plt.scatter(true_anomaly_times, [threshold * 1.1] * len(true_anomaly_times),
                    color='g', marker='o', s=30, label='True Anomalies')

    plt.title("Anomaly Detection Results")
    plt.xlabel("Time")
    plt.yticks([])
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.grid(True)
    plt.tight_layout()
    plt.show()

def plot_predictions(times, features, predictions):
    plt.figure(figsize=(15, 6))
    feature_names = ['Speed', 'Acceleration']

    for i in range(features.shape[1]):
        plt.plot(times, features[:, i], label=f'True {feature_names[i]}', linestyle='-')
        plt.plot(times, predictions[:, i], label=f'Predicted {feature_names[i]}', linestyle='--')

    plt.title("Predicted vs True Speed & Acceleration")
    plt.xlabel("Time")
    plt.ylabel("Value")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

# =============================================
# MAIN EXECUTION
# =============================================
if __name__ == "__main__":
    times, features, true_labels = sequences[available_sequences[24]]
    sequence = (times, features, true_labels)

    model = load_trained_model(input_shape=(None, 2), output_features=2)

    try:
        anomalies, errors, threshold, times_plot, features_plot, labels_plot, predictions = detect_anomalies(model, sequence)

        print(f"Threshold: {threshold:.4f}")
        print(f"Anomalies detected at timesteps: {times_plot[anomalies]}")
        if np.any(labels_plot == 1):
            print(f"True anomalies at: {times_plot[labels_plot == 1]}")
            detection_rate = np.sum(anomalies & (labels_plot == 1)) / np.sum(labels_plot == 1)
            print(f"True anomaly detection rate: {detection_rate:.1%}")

        plot_results(times_plot, features_plot, errors, anomalies, labels_plot, threshold)
        plot_predictions(times_plot, features_plot, predictions)

    except Exception as e:
        print(f"❌ Error during detection: {str(e)}")
        print("Please check your input data shape. Expected features shape: (timesteps, 2)")

