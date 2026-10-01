from collections import defaultdict

from tqdm import tqdm

from ..utils import CursorManager, cursor_foreach, normalize
from .adapter import DatasetAdapter

def get_syntactic_type(syntactic_type, similarity_token, similarity_line):
    # BigCloneEval -> BOTH
    similarity = min(similarity_token, similarity_line)

    type = "aggregate"
    subtype = ""
    if syntactic_type == "1":
        type = "Type-1"
    if syntactic_type == "2":
        type = "Type-2"
    if syntactic_type == "3":
        type = "Type-3"

        if similarity >= 0.9:
            subtype = "Very Strong"
        elif similarity >= 0.7:
            subtype = "Strong"
        elif similarity >= 0.5:
            subtype = "Moderate"
        else:
            type = "Type-4"
    
    return f"{subtype} {type}".strip()

def load_pair_metadata(cursor):
    cursor.execute("SELECT COUNT(*) FROM clones")
    total = cursor.fetchone()[0]

    cursor.execute(f"""
        SELECT
            clones.function_id_one as id1,
            clones.function_id_two as id2,
            clones.syntactic_type,
            clones.functionality_id,
            clones.similarity_token,
            clones.similarity_line
        FROM
            clones
    """)

    pair_metadata = {}

    with tqdm(total=total, desc="BigCloneBench: Loading clones") as pbar:
        for row in cursor_foreach(cursor, 1000):
            id1, id2, syntactic_type, functionality_id, similarity_token, similarity_line = row
            pair = normalize(id1, id2)

            pair_metadata[pair] = {
                "syntactic_type":  get_syntactic_type(str(syntactic_type), similarity_token, similarity_line),
                "functionality_id": functionality_id
            }

            pbar.update(1)

    return pair_metadata

class BigCloneBenchAdapter(DatasetAdapter):
    pair_metadata = {}
    extension = 'java'

    def __init__(self, config, args, parser):
        if parser is None:
            return

        config.arkanjo.granularity = 'function'
        config.dataset = 'code_x_glue_cc_clone_detection_big_clone_bench'
        config.namespace = 'google'
        config.arkanjo.extension = self.extension

        if args.fast:
            return

        missing = []

        if not args.h2_path:
            missing.append("--h2-path")
        if not args.h2_jar:
            missing.append("--h2-jar")

        if missing:
            parser.error(
                f"the following arguments are required: {', '.join(missing)}"
            )

        with CursorManager(args.h2_path, args.h2_jar) as cursor:
            self.pair_metadata = load_pair_metadata(cursor)

    def get_test_dataset(self, ds):
        return ds["test"]
    
    def build_ground_truth(self, ds):
        labeled_pairs = set()
        true_pairs_by_type = defaultdict(set)

        for row in tqdm(ds, desc="Build Ground Truth"):
            pair = normalize(row["id1"], row["id2"])

            labeled_pairs.add(pair)

            if not row["label"]:
                continue

            true_pairs_by_type["aggregate"].add(pair)
            syntactic_type = self.get_syntactic_type(pair)

            if syntactic_type != "aggregate":
                true_pairs_by_type[syntactic_type].add(pair)

        return labeled_pairs, true_pairs_by_type

    def create_batch_iter(self, workdir, row):
        with open(workdir / f"{row["id1"]}.{self.extension}", "w") as f:
            f.write(row["func1"])
        with open(workdir / f"{row["id2"]}.{self.extension}", "w") as f:
            f.write(row["func2"])

    def get_syntactic_type(self, pair):
        info = self.pair_metadata.get(pair)
        if info:
            return info["syntactic_type"]

        return "aggregate"
    
    def get_types_order(self):
        return {
            "Type-1": 0,
            "Type-2": 1,
            "Very Strong Type-3": 2,
            "Strong Type-3": 3,
            "Moderate Type-3": 4,
            "Type-4": 5,
        }
