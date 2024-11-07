import pandas as pd
import matplotlib.pyplot as plt
from scipy import stats
import numpy as np

def plot_mean_ci_by_exec(df, title):
    mean_values = df.groupby('size')['time_elapsed'].mean()
    std_values = df.groupby('size')['time_elapsed'].std()
    count_values = df.groupby('size')['time_elapsed'].count()
    
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
    plt.xlabel('Size')
    plt.ylabel('Time Elapsed (seconds)')
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.legend()
    plt.tight_layout()
    plt.show()


def plot_side_by_side(df1, df2, title):
    def calculate_mean_ci(df):
        mean_values = df.groupby('size')['time_elapsed'].mean()
        std_values = df.groupby('size')['time_elapsed'].std()
        count_values = df.groupby('size')['time_elapsed'].count()

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
    axs[0].set_title(title)
    axs[0].set_xlabel('Size')
    axs[0].set_ylabel('Time Elapsed (seconds)')
    axs[0].grid(True, which='both', linestyle='--', linewidth=0.5)
    axs[0].legend()

    axs[1].plot(mean_values2.index, mean_values2, marker='o', label='Mean', color='orange')
    axs[1].fill_between(mean_values2.index, ci_lower2, ci_upper2, color='lightcoral', alpha=0.5, label='95% Confidence Interval')
    axs[1].set_title(title + " (without I/O)")
    axs[1].set_xlabel('Size')
    axs[1].set_ylabel('Time Elapsed (seconds)')
    axs[1].grid(True, which='both', linestyle='--', linewidth=0.5)
    axs[1].legend()

    plt.tight_layout()
    plt.show()

def plot_mean_ci_by_region(df):
    regions = df['region'].unique()
    
    num_regions = len(regions)

    cols = 2
    fig, axs = plt.subplots(2, cols, figsize=(12, 10))

    for i, region in enumerate(regions):
        region_df = df[df['region'] == region]
        
        mean_values = region_df.groupby('size')['time_elapsed'].mean()
        std_values = region_df.groupby('size')['time_elapsed'].std()
        count_values = region_df.groupby('size')['time_elapsed'].count()
        
        confidence_level = 0.95
        degrees_freedom = count_values - 1
        confidence_interval = stats.t.ppf((1 + confidence_level) / 2., degrees_freedom)
        margin_of_error = confidence_interval * (std_values / np.sqrt(count_values))
        
        ci_lower = mean_values - margin_of_error
        ci_upper = mean_values + margin_of_error
        
        ax = axs[i // cols, i % cols]  # 2x2 grid indexing
        
        ax.plot(mean_values.index, mean_values, marker='o', label='Mean', color='blue')
        
        ax.fill_between(mean_values.index, ci_lower, ci_upper, color='lightblue', alpha=0.5, label='95% Confidence Interval')

        ax.set_title(f'{region}: Mean Time Elapsed by Size')
        ax.set_xlabel('Size')
        ax.set_ylabel('Time Elapsed (seconds)')
        ax.grid(True, which='both', linestyle='--', linewidth=0.5)
        ax.legend()

    plt.tight_layout()
    plt.show()

def plot_mean_ci_by_region_num_threads(df):
    regions = df['region'].unique()
    
    num_regions = len(regions)
    cols = 2
    fig, axs = plt.subplots(2, cols, figsize=(12, 10))

    for i, region in enumerate(regions):
        region_df = df[df['region'] == region]
        
        mean_values = region_df.groupby('num_threads')['time_elapsed'].mean()
        std_values = region_df.groupby('num_threads')['time_elapsed'].std()
        count_values = region_df.groupby('num_threads')['time_elapsed'].count()
        
        confidence_level = 0.95
        degrees_freedom = count_values - 1
        confidence_interval = stats.t.ppf((1 + confidence_level) / 2., degrees_freedom)
        margin_of_error = confidence_interval * (std_values / np.sqrt(count_values))
        
        ci_lower = mean_values - margin_of_error
        ci_upper = mean_values + margin_of_error
        
        ax = axs[i // cols, i % cols]  # 2x2 grid indexing
        
        ax.plot(mean_values.index, mean_values, marker='o', label='Mean', color='blue')
        
        ax.fill_between(mean_values.index, ci_lower, ci_upper, color='lightblue', alpha=0.5, label='95% Confidence Interval')

        ax.set_title(f'{region}: Mean Time Elapsed by num_threads')
        ax.set_xlabel('num_threads')
        ax.set_ylabel('Time Elapsed (seconds)')
        ax.grid(True, which='both', linestyle='--', linewidth=0.5)
        ax.legend()

    plt.tight_layout()
    plt.show()

def plot_side_by_side_parallel(df1, df2, title, parameter="size"):
    def calculate_mean_ci(df):
        mean_values = df.groupby(parameter)['time_elapsed'].mean()
        std_values = df.groupby(parameter)['time_elapsed'].std()
        count_values = df.groupby(parameter)['time_elapsed'].count()

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
    axs[0].set_title(title)
    axs[0].set_xlabel(parameter)
    axs[0].set_ylabel('Time Elapsed (seconds)')
    axs[0].grid(True, which='both', linestyle='--', linewidth=0.5)
    axs[0].legend()

    axs[1].plot(mean_values2.index, mean_values2, marker='o', label='Mean', color='orange')
    axs[1].fill_between(mean_values2.index, ci_lower2, ci_upper2, color='lightcoral', alpha=0.5, label='95% Confidence Interval')
    axs[1].set_title(title + " - Parallel execution")
    axs[1].set_xlabel(parameter)
    axs[1].set_ylabel('Time Elapsed (seconds)')
    axs[1].grid(True, which='both', linestyle='--', linewidth=0.5)
    axs[1].legend()

    plt.tight_layout()
    plt.show()

def plot_side_by_side_parallel_libs(df1, df2, title, parameter="size"):
    def calculate_mean_ci(df):
        mean_values = df.groupby(parameter)['time_elapsed'].mean()
        std_values = df.groupby(parameter)['time_elapsed'].std()
        count_values = df.groupby(parameter)['time_elapsed'].count()

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
    axs[0].set_title(title + " - Pthreads")
    axs[0].set_xlabel(parameter)
    axs[0].set_ylabel('Time Elapsed (seconds)')
    axs[0].grid(True, which='both', linestyle='--', linewidth=0.5)
    axs[0].legend()

    axs[1].plot(mean_values2.index, mean_values2, marker='o', label='Mean', color='orange')
    axs[1].fill_between(mean_values2.index, ci_lower2, ci_upper2, color='lightcoral', alpha=0.5, label='95% Confidence Interval')
    axs[1].set_title(title + " - OpenMP")
    axs[1].set_xlabel(parameter)
    axs[1].set_ylabel('Time Elapsed (seconds)')
    axs[1].grid(True, which='both', linestyle='--', linewidth=0.5)
    axs[1].legend()

    plt.tight_layout()
    plt.show()