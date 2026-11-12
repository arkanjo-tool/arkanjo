**ArKanjo** is a CLI tool designed to help developers find code duplication within 
their codebases, specifically within the scope of functions.

The current functionalities of the tool are:

- Explore code duplication in a codebase, with a limited number of filters available to the user.

- Find all functions that are duplicates of a function specified by the user.

- Create a report detailing the number of duplications in the codebase, separated by folder.

Some other commands were used for the creator's master's degree, but they are not relevant to end-users.

The tool currently supports only the languages provided by the Tree-sitter API; these must be explicitly added to the `config.json` file.

## Similarity

The tool currently uses the concept of **similarity**. A user can pass a **similarity threshold** to the 
tool, which is a number between 0.0 and 100.0. This threshold is used to limit what the tool 
considers a duplication.

If the threshold is set to **0.0**, everything is considered a duplication. If the threshold is set 
to **100.0**, only completely equal functions are considered duplications. In its current state, 
the tool provides good results with similarity thresholds around **90.0**.

For more details about the similarity model, see [**Similarity Model**](docs/developer-guide/similarity-model.md).

## How to install

Download the source code:

```sh
git clone https://github.com/arkanjo-tool/arkanjo.git
cd arkanjo
```

Build the binary:

```sh
mkdir build && cd build

cmake ..
cmake --build .
```

The binaries will be generated in the `build/` directory.

Optionally, install the binaries using:

```sh
cmake --install . [--prefix ~/.local]
```

> [!NOTE]
> The `--prefix` option only specifies the installation directory. It does not change ArKanjo's runtime paths. By default, ArKanjo uses its configured cache directory. To override the third-party directory, for example, set `ARKANJO_THIRD_PARTY_DIR` when running the program.

### Methods

ArKanjo supports multiple code duplication detection methods.

See the documentation for details:

- [Methods documentation](docs/methods/index.md)

## How to Run

### Preprocessing and Cache

The tool is designed with **heavy preprocessing**, which enables it to answer different kinds of queries quickly. Cache files are stored on your system and can grow significantly depending on codebase size.

To perform the preprocessing, execute the preprocessor:

```sh
arkanjo-preprocessor build
```

* The preprocessor will ask for the complete path to the codebase you want to analyze and the desired similarity threshold. 
* **Cache fallback:** `./tmp/arkanjo`

### Execute the tool

To execute the tool's commands, you need to run a command that follows this format:

```sh
arkanjo <command> [command_parameters] [--preprocessor] [-S <SIMILARITY>]
```

If the preprocessor has not been run yet, the tool will automatically execute it.

The parameters common to all commands are:

- `--preprocessor`: Forces the preprocessor to execute.

- `-S <SIMILARITY>`: Changes the similarity threshold to `SIMILARITY` for the current command only.

#### Commands (summary)

| Command       | Purpose | Example |
|---------------|---------|---------|
| `explorer`    | Explore duplicated functions detected in the project | `arkanjo explorer -l 10 -s` |
| `function`    | Search for functions using a substring match. | `arkanjo function offset_to_id` |
| `duplication` | Analyze and report duplicated lines across the codebase, grouped by folder hierarchy. | `arkanjo duplication` |

For full details and options for each command, run:

```sh
arkanjo <command> --help
```
