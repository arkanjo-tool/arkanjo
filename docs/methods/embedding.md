# Embedding-based detection method {#embedding}

A function-level code duplication detector that measures *semantic* similarity
between function bodies. Instead of comparing tokens or lines, it embeds each
function with a Hugging Face embedding model
(default `jinaai/jina-embeddings-v2-base-code`) and computes the pairwise cosine
similarity, scaled to a 0.0-100.0 range. This lets it flag functions that do the
same thing even when names, formatting, or syntax differ.

## Preparing the environment

The embedding dependencies are **not** installed with the rest of the project.

```sh
python3 -m pip install -r requirements/embedding.txt
```

The first run downloads the model (~hundreds of MB) to `~/.cache/huggingface`.
On an offline machine, that cache must be pre-populated beforehand.

## Parameters

The method is selected and tuned through the preprocessor. The similarity
threshold is asked interactively. Method-specific args are passed after a `--`
separator and forwarded to the embedding script:

```sh
arkanjo-preprocessor build --path <path> \
    -- [--max-seq-length N] [--batch-size N] [--model NAME]
```

| Parameter | What it does | Default |
| --- | --- | --- |
| similarity threshold | Minimum similarity (0-100) for a pair to be reported. Asked at the prompt *"Enter minimum similarity desired"*. | `0.0` (everything is reported) |
| `--max-seq-length` | Maximum token length per function body; longer bodies are truncated. Attention memory grows as O(length²), so raising it sharply increases RAM use. | `1024` |
| `--batch-size` | How many function bodies are embedded in parallel. Lower it on machines with less RAM. | `4` |
| `--model` | Hugging Face `sentence-transformers` model used to embed the bodies. Any compatible Hub model works. | `jinaai/jina-embeddings-v2-base-code` |

## Notes

- <b>Cost is @f$O(n^2)@f$</b>: every function is compared against every other. The
  embedding step is vectorized, but the comparison matrix sets the cost ceiling
  on large codebases.
- To switch the embedding model, either pass `-- --model NAME` or edit
  the `MODEL_NAME` constant at the top of
  `third-party/embedding/llm_detection.py`.
