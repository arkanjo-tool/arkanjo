#!/usr/bin/env python3

import argparse
import math
import random
import subprocess
import json

from collections import defaultdict
from pathlib import Path
from tempfile import TemporaryDirectory
import time
from types import SimpleNamespace

import pandas as pd
from tqdm import tqdm

import pyarrow as pa
import pyarrow.parquet as pq

from commands.benchmark.datasets.poj104 import POJ104Adapter

from .datasets.bigclonebench import BigCloneBenchAdapter

from .utils import \
    create_name, create_batch, arkanjo_preprocessor, arkanjo_explorer, normalize

def parse_output(output: subprocess.CompletedProcess[str]):
    pairs = set()

    for line in output.stdout.splitlines():
        if "," not in line:
            continue

        a, b = line.strip().split(",")
        if not "." in a or not "." in b:
            continue

        a, _ = a.split(".")
        b, _ = b.split(".")
        pairs.add(normalize(a, b))

    return pairs

def compute_metrics(true_pairs, predicted_pairs, universe_pairs):
    # The metrics are calculated only on labeled pairs.
    # Pairs that are not present in the benchmark are ignored, as there is
    # no ground truth available to classify them as clones or non-clones.
    false_pairs = universe_pairs - true_pairs

    tp_pairs = predicted_pairs & true_pairs
    fp_pairs = predicted_pairs & false_pairs
    fn_pairs = true_pairs - predicted_pairs
    tn_pairs = false_pairs - predicted_pairs

    return {
        "tp_pairs": tp_pairs,
        "fp_pairs": fp_pairs,
        "fn_pairs": fn_pairs,
        "tn_pairs": tn_pairs,
    }

def save_inspection(
    rows,
    metrics,
    threshold,
    syntactic_type
):
    mapping = {
        "tp_pairs": (1, 1), "fp_pairs": (0, 1),
        "fn_pairs": (1, 0), "tn_pairs": (0, 0),
    }

    for result_name, (y_true, y_pred) in mapping.items():
        result = result_name.replace("_pairs", "").upper()
        for pair in metrics[result_name]:
            rows.append({
                "id1": pair[0],
                "id2": pair[1],
                "threshold": threshold,
                "result": result,
                "y_true": y_true,
                "y_pred": y_pred,
                "syntactic_type": syntactic_type
            })

def predict_pairs(config, ds, num_batches, labeled_pairs, adapter):
    predicted_pairs_by_threshold = defaultdict(lambda: defaultdict(set))

    config.arkanjo.names = []

    timestamp_preprocessor = 0

    for batch_index in tqdm(
        range(num_batches), desc="Batch Method", leave=False
    ):
        # Uses an in-memory tmpfs to reduce the write overhead of temporary
        # files generated for each batch.
        with TemporaryDirectory(dir="/dev/shm") as workdir:
            workdir = Path(workdir)

            create_batch(ds, batch_index, workdir, config, adapter)

            config.arkanjo.name = "".join(create_name(config, batch_index))
            config.arkanjo.names.append(config.arkanjo.name)

            start = time.perf_counter()

            output = arkanjo_preprocessor(workdir, config)

            end = time.perf_counter()
            timestamp_preprocessor =+ (end - start)

            if output.returncode:
                raise Exception(output.stderr)

            if output.stdout:
                tqdm.write(output.stdout)

            steps = 10 if config.fast else 5
            for threshold in tqdm(
                range(0, 100, steps), desc=f"Exploring with thresholds", leave=False
            ):
                output = arkanjo_explorer(config, threshold)
                predicted_pairs = parse_output(output)

                # The comparison is performed across all files in the batch and
                # may detect pairs that are not annotated in the dataset. Since
                # the evaluation can only be performed on labeled pairs, we
                # keep only the known predictions.
                known_predictions = predicted_pairs & labeled_pairs

                for pair in known_predictions:
                    predicted_pairs_by_threshold[threshold]["aggregate"].add(pair)

                    syntactic_type = adapter.get_syntactic_type(pair)
                    if syntactic_type != "aggregate":
                        predicted_pairs_by_threshold[threshold][syntactic_type].add(pair)

    return predicted_pairs_by_threshold, timestamp_preprocessor

adapters = {
    "bigclonebench": BigCloneBenchAdapter,
    "poj104": POJ104Adapter
}

def main__run(args: argparse.Namespace, parser):
    from datasets import load_dataset

    config = SimpleNamespace(
        dataset=None,
        namespace=None,
        seed=args.seed,
        max_files=args.max_files,
        methods=args.methods,
        arkanjo=SimpleNamespace(
            extension=None,
            granularity='function'
        ),
        fast=args.fast
    )
    random.seed(config.seed)

    adapter = adapters[args.dataset](config, args, parser)

    ds = load_dataset(f"{config.namespace}/{config.dataset}")

    test_ds = adapter.get_test_dataset(ds).shuffle(seed=config.seed)
    config.max_files = min(len(test_ds), config.max_files)

    if config.fast:
        size = len(test_ds)//10
        test_ds = test_ds.select(range(min(config.max_files, size)))

    num_batches = math.ceil(len(test_ds) / config.max_files)

    labeled_pairs, true_pairs_by_type = adapter.build_ground_truth(test_ds)

    for arkanjo_method in tqdm(args.methods, desc="Predict Pairs", leave=False):
        predicted_pairs_by_threshold = defaultdict(lambda: defaultdict(set))
        timestamp_preprocessor = 0
        try:
            config.arkanjo.method = str(arkanjo_method)
            predicted_pairs_by_threshold, timestamp_preprocessor = predict_pairs(
                config,
                test_ds,
                num_batches,
                labeled_pairs,
                adapter
            )
        except KeyboardInterrupt:
            tqdm.write("\nExecution interrupted by the user. Showing partial results.")
        finally:
            rows = []

            for threshold in sorted(predicted_pairs_by_threshold.keys()):
                for syntactic_type in sorted(true_pairs_by_type.keys()):
                    metrics = compute_metrics(
                        true_pairs_by_type[syntactic_type],
                        predicted_pairs_by_threshold[threshold][syntactic_type],
                        labeled_pairs
                    )

                    save_inspection(rows, metrics, threshold, syntactic_type)

            RESULTS_DIR = Path("benchmarks") / "results"
            RESULTS_DIR.mkdir(parents=True, exist_ok=True)

            df = pd.DataFrame(rows)
            table = pa.Table.from_pandas(df)
            custom_metadata = {
                b"names": json.dumps(config.arkanjo.names).encode("utf-8"),
                b"method": arkanjo_method,
                b"dataset": args.dataset,
                b"timestamp": str(timestamp_preprocessor)
            }
            new_schema = table.schema.with_metadata(custom_metadata)
            table_with_matadata = table.cast(new_schema)

            pq.write_table(table_with_matadata, RESULTS_DIR / f"{args.dataset}_{arkanjo_method}.parquet")
