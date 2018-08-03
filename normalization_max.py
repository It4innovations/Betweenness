import argparse
import pandas as pd
import time
import os
from pathlib import Path


def walklevel(some_dir, level=1):
    some_dir = some_dir.rstrip(os.path.sep)
    assert os.path.isdir(some_dir)
    num_sep = some_dir.count(os.path.sep)
    for root, dirs, files in os.walk(some_dir):
        yield root, dirs, files
        num_sep_this = root.count(os.path.sep)
        if num_sep + level <= num_sep_this:
            del dirs[:]


def getmaxfromfile(input: str):
    df = pd.read_csv(input, ';')
    return df['VALUE'].max()


def getmaxfromdirectory(directory: str):
    totalmax = 0
    for root, dirs, files in walklevel(directory, 0):
        for filename in files:
            input = os.path.join(root, filename)
            if not os.path.isdir(input):
                max = getmaxfromfile(input)
                print('Max value from {} is {}'.format(input, max))
                if max > totalmax:
                    totalmax = max
    return totalmax


def normalize(input: str, output: str, norm: float):
    df = pd.read_csv(input, ';')
    df['NORM'] = df['VALUE'] / norm * 100.0  # qgis needs to have 0-100 values
    df.to_csv(output, index=False, sep=';', header=['line_id', 'value1', 'value1_norm'])


def normalize_directory(directory: str, outputdir: str):
    start_time = time.time()
    norm_value = getmaxfromdirectory(directory)
    print('Norm value found {}'.format(norm_value))
    # Read and norm files in directory and results save to other directory
    Path(outputdir).mkdir(exist_ok=True)
    print('Output directory {}'.format(outputdir))
    for root, dirs, files in walklevel(directory, 0):
        for filename in files:
            input = os.path.join(root, filename)
            if not os.path.isdir(input):
                output = os.path.join(outputdir, filename)
                normalize(input, output, norm_value)

    elapsed_time = time.time() - start_time
    print('Elapsed time {}'.format(elapsed_time))


def parse_args():
    directory = 'edges_7-8_5_beta'
    outputdir = 'normalized'

    parser = argparse.ArgumentParser()
    parser.add_argument("-d", type=str, help="input directory")
    parser.add_argument("-o", type=str, help="output directory")
    args = parser.parse_args()
    if args.d:
        directory = args.d
    if args.o:
        outputdir = args.o
    return [directory, outputdir]


if __name__ == "__main__":
    # python .\normalization.py -d betweenness/vienna/edges
    params = parse_args()
    print('Params: -d {} -o {}'.format(params[0], params[1]))
    normalize_directory(params[0], params[1])
