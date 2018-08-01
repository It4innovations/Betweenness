import argparse
import pandas as pd
import time
import os
from pathlib import Path


class Arguments:
    def __init__(self, directory: str, outputdirectory: str, top: bool, percentage: float, version: int):
        self.directory = directory
        self.outputdirectory = outputdirectory
        self.top = top
        self.percentage = percentage
        self.version = version


def parse_args() -> Arguments:
    directory = 'betweenness/vienna_speed_profiled/edges_norm'
    outputdirectory = directory + '_output'
    top = True
    percentage = 1.0
    version = 1

    parser = argparse.ArgumentParser()
    parser.add_argument("-d", type=str, help="input directory")
    parser.add_argument("-o", type=str, help="output directory")
    parser.add_argument("-t", type=bool, help="top or bottom values")
    parser.add_argument("-p", type=str, help="top X percent of values returned")
    parser.add_argument("-v", type=int, help="version, 1=top changes, 2=top values, 3=top differencies")

    args = parser.parse_args()
    if args.d:
        directory = args.d
    if args.o:
        outputdirectory = args.o
    if args.t:
        top = args.t
    if args.p:
        percentage = args.p
    if args.v:
        version = args.v

    return Arguments(directory, outputdirectory, top, percentage, version)


class Comparer:
    def __init__(self, percentage: float, greater: bool):
        self.percentage = percentage
        self.greater = greater


def change(old: float, new: float) -> float:
    if old == 0 and new > 0:  # what to do?
        return -101
    elif old == 0 and new == 0:
        return 0
    else:
        return (new - old) / old * 100.0


def diff(old: float, new: float) -> float:
    return new - old


def get_top_differencies(df: pd.DataFrame, comparer: Comparer) -> pd.DataFrame:
    # Calculate percent changes
    df['DIFF'] = df.apply(lambda row: diff(row.value1_norm_x, row.value1_norm_y), axis=1)
    # Select top X percent largest changes
    count = int(len(df.index) * comparer.percentage / 100.0)
    if comparer.greater:
        df = df.nlargest(count, 'DIFF')
    else:
        df = df.nsmallest(count, 'DIFF')

    return df


def get_top_changes(df: pd.DataFrame, comparer: Comparer) -> pd.DataFrame:
    # Calculate percent changes
    df['CHANGE'] = df.apply(lambda row: change(row.value1_norm_x, row.value1_norm_y), axis=1)
    # Select top X percent largest changes
    count = int(len(df.index) * comparer.percentage / 100.0)
    if comparer.greater:
        df = df.nlargest(count, 'CHANGE')
    else:
        df = df.nsmallest(count, 'CHANGE')

    return df


def get_top_values(df: pd.DataFrame, comparer: Comparer) -> pd.DataFrame:
    # Select top X percent largest values
    count = int(len(df.index) * comparer.percentage / 100.0)
    if comparer.greater:
        df = df.nlargest(count, 'value1_norm')
    else:
        df = df.nsmallest(count, 'value1_norm')

    return df


def save_log(path: str, result: list):
    with open(path, 'w') as f:
        f.write('ID\n')
        for item in result:
            f.write('{}\n'.format(item))


def process_top_values(directory: str, outputdirectory: str, comparer: Comparer):
    for root, dirs, files in os.walk(directory):
        for filename in files:
            file = os.path.join(root, filename)
            df = pd.read_csv(file, ';')
            result = get_top_values(df, comparer)['line_id'].tolist()
            filenamewx = filename.split('.')[0]
            outputname = 'values_{}_{}.csv'.format(filenamewx, len(result))
            outputpath = os.path.join(outputdirectory, outputname)
            save_log(outputpath, result)


def process_top_changes(directory: str, outputdirectory: str, comparer: Comparer):
    for root, dirs, files in os.walk(directory):
        basefile = files[0]
        basefilePath = os.path.join(root, basefile)
        df_base = pd.read_csv(basefilePath, ';')
        del files[0]
        for file in files:
            derived = os.path.join(root, file)
            df_derived = pd.read_csv(derived, ';')
            df = pd.merge(df_base, df_derived, on='line_id')
            result = get_top_changes(df, comparer)['line_id'].tolist()
            basefilenamewx = basefile.split('.')[0]
            filenamewx = file.split('.')[0]
            outputname = 'changes_{}_{}_{}.csv'.format(basefilenamewx, filenamewx, len(result))
            outputpath = os.path.join(outputdirectory, outputname)
            save_log(outputpath, result)


def process_top_differencies(directory: str, outputdirectory: str, comparer: Comparer):
    for root, dirs, files in os.walk(directory):
        basefile = files[0]
        basefilePath = os.path.join(root, basefile)
        df_base = pd.read_csv(basefilePath, ';')
        del files[0]
        for file in files:
            derived = os.path.join(root, file)
            df_derived = pd.read_csv(derived, ';')
            df = pd.merge(df_base, df_derived, on='line_id')
            result = get_top_differencies(df, comparer)['line_id'].tolist()
            basefilenamewx = basefile.split('.')[0]
            filenamewx = file.split('.')[0]
            outputname = 'differencies_{}_{}_{}.csv'.format(basefilenamewx, filenamewx, len(result))
            outputpath = os.path.join(outputdirectory, outputname)
            save_log(outputpath, result)


def main():
    # example on windows
    # python .\comparator.py -d betweenness\vienna_speed_profiled\edges_norm -o betweenness\vienna_speed_profiled\edges_norm_outputs -v 3
    start_time = time.time()
    arguments = parse_args()

    Path(arguments.outputdirectory).mkdir(exist_ok=True)

    if arguments.version == 1:
        process_top_changes(arguments.directory, arguments.outputdirectory, Comparer(arguments.percentage, arguments.top))
    elif arguments.version == 2:
        process_top_values(arguments.directory, arguments.outputdirectory, Comparer(arguments.percentage, arguments.top))
    elif arguments.version == 3:
        process_top_differencies(arguments.directory, arguments.outputdirectory, Comparer(arguments.percentage, arguments.top))

    elapsed_time = time.time() - start_time
    print('Elapsed time {}'.format(elapsed_time))


if __name__ == "__main__":
    main()
