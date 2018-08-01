import argparse
import pandas as pd
import time
import os
from pathlib import Path


def getmaxfromfile(input: str):
    df = pd.read_csv(input, ';')
    return df['VALUE'].max()


def getmaxfromdirectory(directory: str):
    totalmax = 0
    for root, dirs, files in os.walk(directory):
        for filename in files:
            input = os.path.join(root, filename)
            max = getmaxfromfile(input)
            print('Max value from {} is {}'.format(input, max))
            if max > totalmax:
                totalmax = max
    return totalmax


def normalize(input: str, output: str, norm: float):
    df = pd.read_csv(input, ';')
    df['NORM'] = df['VALUE'] / norm * 100.0  # qgis needs to have 0-100 values
    df.to_csv(output, index=False, sep=';', header=['line_id', 'value1', 'value1_norm'])


def normalize_directory(directory: str):
    start_time = time.time()
    norm_value = getmaxfromdirectory(directory)
    print('Norm value found {}'.format(norm_value))
    # Read and norm files in directory and results save to other directory
    output_directory = directory + '_norm'
    Path(output_directory).mkdir(exist_ok=True)

    for root, dirs, files in os.walk(directory):
        for filename in files:
            input = os.path.join(root, filename)
            output = os.path.join(output_directory, filename)
            print('{} {}'.format(input, output))
            normalize(input, output, norm_value)

    elapsed_time = time.time() - start_time
    print('Elapsed time {}'.format(elapsed_time))


def parse_args():
    directory = 'edges_7-8_5_beta'
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", type=str, help="input directory")

    args = parser.parse_args()
    if args.d:
        directory = args.d

    return directory


if __name__ == "__main__":
    # python .\normalization.py -d betweenness/vienna_speed_profiled/edges
    directory = parse_args()
    print('Params: -d {}'.format(directory))
    normalize_directory(directory)
