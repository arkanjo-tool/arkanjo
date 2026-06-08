#!/usr/bin/env python3

import argparse
import os
import sys

MODEL_NAME = "jinaai/jina-embeddings-v2-base-code"

# Cap the token sequence length. jina-v2 accepts up to 8192 tokens, but 
# a single very long function body can try to
# allocate tens of GB. Longer bodies will be  truncated.
DEFAULT_MAX_SEQ_LENGTH = 1024

# Conservative batch size to keep peak memory bounded on large codebases.
DEFAULT_BATCH_SIZE = 4


def log(message):
    print(message, file=sys.stderr, flush=True)


def find_function_files(source_dir):
    """Recursively collect every regular file under source_dir (absolute paths)."""
    file_paths = []
    for root, _dirs, files in os.walk(source_dir):
        for name in files:
            file_paths.append(os.path.abspath(os.path.join(root, name)))
    file_paths.sort()
    return file_paths


def read_function_bodies(file_paths):
    bodies = []
    for path in file_paths:
        try:
            with open(path, "r", encoding="utf-8", errors="replace") as handle:
                bodies.append(handle.read())
        except OSError as error:
            log(f"Could not read {path}: {error}")
            bodies.append("")
    return bodies


def compute_embeddings(bodies, max_seq_length, batch_size, model_name):
    import numpy as np
    from sentence_transformers import SentenceTransformer

    log(f"Loading model {model_name} (first run downloads weights)...")
    model = SentenceTransformer(model_name, trust_remote_code=True)
    model.max_seq_length = max_seq_length

    log(f"Embedding {len(bodies)} function bodies "
        f"(max_seq_length={max_seq_length}, batch_size={batch_size})...")

    embeddings = model.encode(
        bodies,
        normalize_embeddings=True,
        batch_size=batch_size,
        show_progress_bar=True,
    )
    return np.asarray(embeddings, dtype=np.float32)


def emit_similar_pairs(file_paths, embeddings, min_similarity):
    similarity_matrix = embeddings @ embeddings.T
    count = len(file_paths)

    out = sys.stdout
    for i in range(count):
        for j in range(i + 1, count):
            cosine = float(similarity_matrix[i][j])
            # Clamp before scaling to guard against floating-point drift.
            cosine = max(-1.0, min(1.0, cosine))
            score = cosine * 100.0
            if score >= min_similarity:
                out.write(f"{file_paths[i]}\t{file_paths[j]}\t{score:.2f}\n")
    out.flush()


def main():
    parser = argparse.ArgumentParser(
        description="LLM-embedding code duplication detector for arkanjo."
    )
    parser.add_argument(
        "--source-dir",
        required=True,
        help="Directory containing the materialized function bodies.",
    )
    parser.add_argument(
        "--min-similarity",
        type=float,
        default=0.0,
        help="Minimum similarity (0-100) for a pair to be reported.",
    )
    parser.add_argument(
        "--max-seq-length",
        type=int,
        default=DEFAULT_MAX_SEQ_LENGTH,
        help=(
            "Maximum token sequence length per function body. Longer bodies are "
            f"truncated. Attention memory scales as O(seq_len^2). Default: {DEFAULT_MAX_SEQ_LENGTH}."
        ),
    )
    parser.add_argument(
        "--batch-size",
        type=int,
        default=DEFAULT_BATCH_SIZE,
        help=(
            "Number of function bodies embedded in parallel. Lower this on "
            f"machines with less RAM. Default: {DEFAULT_BATCH_SIZE}."
        ),
    )
    parser.add_argument(
        "--model",
        type=str,
        default=MODEL_NAME,
        help=(
            "Hugging Face sentence-transformers model used to embed function "
            f"bodies. Default: {MODEL_NAME}."
        ),
    )
    args = parser.parse_args()

    file_paths = find_function_files(args.source_dir)
    if len(file_paths) < 2:
        log("Fewer than two function bodies found; nothing to compare.")
        return 0

    bodies = read_function_bodies(file_paths)
    embeddings = compute_embeddings(bodies, args.max_seq_length, args.batch_size, args.model)

    relative_paths = [os.path.relpath(p, args.source_dir) for p in file_paths]
    emit_similar_pairs(relative_paths, embeddings, args.min_similarity)
    return 0


if __name__ == "__main__":
    sys.exit(main())
