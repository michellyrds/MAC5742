import pandas as pd
import matplotlib.pyplot as plt
from scipy import stats
import numpy as np

def plot_mean_ci_by_exec(df, title, group_by: str):
    mean_values = df.groupby(group_by)['elapsed_time'].mean()
    std_values = df.groupby(group_by)['elapsed_time'].std()
    count_values = df.groupby(group_by)['elapsed_time'].count()
    
    confidence_level = 0.95
    degrees_freedom = count_values - 1
    confidence_interval = stats.t.ppf((1 + confidence_level) / 2., degrees_freedom)
    margin_of_error = confidence_interval * (std_values / np.sqrt(count_values))
    
    ci_lower = mean_values - margin_of_error
    ci_upper = mean_values + margin_of_error
    
    plt.figure(figsize=(10, 6))
    plt.plot(mean_values.index, mean_values, marker='o', label='Mean', color='blue')
    plt.fill_between(mean_values.index, ci_lower, ci_upper, color='lightblue', alpha=0.5, label='95% Confidence Interval')
    plt.title(title)
    plt.xlabel(group_by)
    plt.ylabel('Time Elapsed (seconds)')
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.legend()
    plt.tight_layout()
    plt.show()

def plot_side_by_side(df1, df2, title1, title2, group_by="n_points", parameter='elapsed_time', parameter_title='Time Elapsed (seconds)'):
    def calculate_mean_ci(df):
        mean_values = df.groupby(group_by)[parameter].mean()
        std_values = df.groupby(group_by)[parameter].std()
        count_values = df.groupby(group_by)[parameter].count()

        confidence_level = 0.95
        degrees_freedom = count_values - 1
        confidence_interval = stats.t.ppf((1 + confidence_level) / 2., degrees_freedom)
        margin_of_error = confidence_interval * (std_values / np.sqrt(count_values))

        ci_lower = mean_values - margin_of_error
        ci_upper = mean_values + margin_of_error

        return mean_values, ci_lower, ci_upper

    mean_values1, ci_lower1, ci_upper1 = calculate_mean_ci(df1)
    mean_values2, ci_lower2, ci_upper2 = calculate_mean_ci(df2)

    fig, axs = plt.subplots(1, 2, figsize=(14, 6))

    axs[0].plot(mean_values1.index, mean_values1, marker='o', label='Mean', color='blue')
    axs[0].fill_between(mean_values1.index, ci_lower1, ci_upper1, color='lightblue', alpha=0.5, label='95% Confidence Interval')
    axs[0].set_title(title1)
    axs[0].set_xlabel(group_by)
    axs[0].set_ylabel(parameter_title)
    axs[0].grid(True, which='both', linestyle='--', linewidth=0.5)
    axs[0].legend()

    axs[1].plot(mean_values2.index, mean_values2, marker='o', label='Mean', color='orange')
    axs[1].fill_between(mean_values2.index, ci_lower2, ci_upper2, color='lightcoral', alpha=0.5, label='95% Confidence Interval')
    axs[1].set_title(title2)
    axs[1].set_xlabel(group_by)
    axs[1].set_ylabel(parameter_title)
    axs[1].grid(True, which='both', linestyle='--', linewidth=0.5)
    axs[1].legend()

    plt.tight_layout()
    plt.show()
