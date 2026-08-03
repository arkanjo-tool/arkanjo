from collections import defaultdict
import hashlib
from itertools import combinations
from math import comb

from tqdm import tqdm

from ..utils import normalize
from .adapter import DatasetAdapter

class POJ104Adapter(DatasetAdapter):
    pair_metadata = {}
    extension = 'cpp'

    def __init__(self, config, args, parser):
        config.arkanjo.granularity = 'file'
        config.dataset = 'code_x_glue_cc_clone_detection_poj104'
        config.namespace = 'google'
        config.arkanjo.extension = self.extension

    def get_test_dataset(self, ds):
        return ds["test"]
    
    def get_name(self, row):
        code = row["code"]
        hash_sha256 = hashlib.sha256(code.encode("utf-8")).hexdigest()

        return hash_sha256

    def build_ground_truth(self, ds):
        universe_pairs = set()
        true_pairs_by_type = defaultdict(set)

        items = [
            (self.get_name(row), row["label"])
            for row in ds
        ]

        total_combinations = comb(len(items), 2)

        with tqdm(total=total_combinations, desc="Build Ground Truth") as pbar:
            for (id1, label1), (id2, label2) in combinations(items, 2):
                pair = normalize(id1, id2)

                universe_pairs.add(pair)

                if label1 == label2:
                    true_pairs_by_type["aggregate"].add(pair)

                pbar.update(1)

        return universe_pairs, true_pairs_by_type

    def create_batch_iter(self, workdir, row):
        code = row["code"].replace("\r\n", "\n").replace("\r", "\n")

        with open(workdir / f"{self.get_name(row)}.{self.extension}", "w", encoding="utf-8", newline="\n") as f:
            f.write(code)

    def get_syntactic_type(self, pair):
        return "aggregate"
    
    def get_types_order(self):
        return { }
