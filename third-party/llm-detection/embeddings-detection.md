# Embedding-based detection method

A function-level code duplication detector that measures *semantic* similarity
between function bodies. Instead of comparing tokens or lines, it embeds each
function with a Hugging Face embedding model
(default `jinaai/jina-embeddings-v2-base-code`) and computes the pairwise cosine
similarity, scaled to a 0-100 range. This lets it flag functions that do the
same thing even when names, formatting, or syntax differ.

It is one of the four interchangeable detection techniques offered by the
preprocessor (selected as option `4`): same input and output format as the
others, so all query commands (`explorer`, `function`, `duplication`) work
unchanged.

## Preparing the environment

The embedding dependencies are **not** installed with the rest of the project.
Install them in the same `python3` interpreter that Arkanjo will call (watch out
for virtualenvs):

```sh
python3 -m pip install -r third-party/llm-detection/requirements.txt
```

We recommend installing them inside a Python virtual environment (e.g. `venv`,
`virtualenv`, or `conda`), and keeping that environment **activated** whenever
you run the method — Arkanjo invokes the `python3` on your `PATH`, so the
dependencies must be visible to the active interpreter. For example, with
`venv`:

```sh
source .venv/bin/activate
```

The first run downloads the model (~hundreds of MB) to `~/.cache/huggingface`.
On an offline machine, that cache must be pre-populated beforehand.

## Parameters

The method is selected and tuned through the preprocessor. The similarity
threshold is asked interactively; the rest are optional CLI flags:

```sh
arkanjo-preprocessor build [path] \
    [--llm-max-seq-length N] [--llm-batch-size N] [--llm-model NAME]
```

| Parameter | What it does | Default |
| --- | --- | --- |
| similarity threshold | Minimum similarity (0-100) for a pair to be reported. Asked at the prompt *"Enter minimum similarity desired"*. | `0.0` (everything is reported) |
| `--llm-max-seq-length` | Maximum token length per function body; longer bodies are truncated. Attention memory grows as O(length²), so raising it sharply increases RAM use. | `1024` |
| `--llm-batch-size` | How many function bodies are embedded in parallel. Lower it on machines with less RAM. | `4` |
| `--llm-model` | Hugging Face `sentence-transformers` model used to embed the bodies. Any compatible Hub model works. | `jinaai/jina-embeddings-v2-base-code` |

These flags are specific to the `llm` technique and are ignored by the other
methods. When omitted, the defaults above are used.

## Notes

- **Cost is O(n²)**: every function is compared against every other. The
  embedding step is vectorized, but the comparison matrix sets the cost ceiling
  on large codebases.
- To switch the embedding model permanently, either pass `--llm-model` or edit
  the `MODEL_NAME` constant at the top of
  `third-party/llm-detection/llm_detection.py`.
