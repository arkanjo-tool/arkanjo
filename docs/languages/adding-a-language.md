# Adding a Language

ArKanjo uses Tree-sitter to parse source code. The languages that should be recognized are configured in `config.json`.

Example:

```json
{
    "languages": ["c", "rust"],
    "extensions": {
        "c": [".c", ".h"],
        "rust": [".rs"]
    }
}
```

## `languages`

The `languages` array lists the Tree-sitter grammars that ArKanjo should load.

For official Tree-sitter grammars, the language name corresponds to the repository suffix:

```
tree-sitter-<language>
```

For example:

| Language | Tree-sitter repository | Entry in `languages` |
| -------- | ---------------------- | -------------------- |
| C        | `tree-sitter-c`        | `"c"`                |
| Rust     | `tree-sitter-rust`     | `"rust"`             |
| Python   | `tree-sitter-python`   | `"python"`           |

ArKanjo automatically resolves these names to the corresponding official Tree-sitter repositories.

## Non-official grammars

Grammars that are not maintained under the official Tree-sitter organization are **not** cloned automatically.

To use one of these grammars:

1. Clone the grammar repository manually into the `build/third-party/parsers/` directory.
2. Ensure the directory name matches the language identifier used in `config.json`.

For example, the official C grammar is stored as:

```text
build/third-party/parsers/tree-sitter-c
```

A third-party grammar should follow the same layout. Once the repository is available in `build/third-party/parsers/`, add the corresponding language to `config.json`.

## `extensions`

The `extensions` object maps each configured language to the file extensions that should be be parsed using that grammar.

```json
{
    "extensions": {
        "c": [".c", ".h"],
        "rust": [".rs"]
    }
}
```

## Reconfiguring the build

After modifying `config.json`, rerun CMake from the build directory:

```bash
cd build
cmake ..
```

During configuration, ArKanjo will:

1. Clone any newly added Tree-sitter grammar repositories.
2. Build the grammars.
3. Make them available to the analyzer.

If you add or remove languages, rerunning `cmake ..` is required before rebuilding ArKanjo.
