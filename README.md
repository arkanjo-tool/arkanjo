**Arkanjo** is a CLI tool designed to help developers find code duplication within 
their codebases, specifically within the scope of functions.

The current functionalities of the tool are:

- Explore code duplication in a codebase, with a limited number of filters available to the user.

- Find all functions that are duplicates of a function specified by the user.

- Create a report detailing the number of duplications in the codebase, separated by folder.

Some other commands were used for the creator's master's degree, but they are not relevant to end-users.

The tool currently supports the C programming language and also supports Java with some limitations.

# Similarity

The tool currently uses the concept of **similarity**. A user can pass a **similarity threshold** to the 
tool, which is a number between 0 and 100. This threshold is used to limit what the tool 
considers a duplication.

If the threshold is set to **0**, everything is considered a duplication. If the threshold is set 
to **100**, only completely equal functions are considered duplications. In its current state, 
the tool provides good results with similarity thresholds around **90**.

The Arkanjo tool uses the
[Duplicate Code Detection Tool](https://github.com/platisd/duplicate-code-detection-tool) 
as a subroutine to generate the similarity metrics.

# Requirements

The tool has only been tested on **Ubuntu** operating systems. An installation guide could be included.

# How to install

Run the following commands in the terminal to install the dependencies:

```sh
pip3 install --user nltk
pip3 install --user gensim
pip3 install --user astor
python3 -m nltk.downloader punkt
```

Download the source code:

```sh
git clone https://github.com/arkanjo-tool/arkanjo.git
cd arkanjo
```

Build the binary:

```sh
mkdir build
cd build

cmake ..
cmake --build .
```

The binaries will be generated in the `build/` directory.

# How to Run

## Preprocessing and Cache

The tool is designed with **heavy preprocessing**, which enables it to answer different kinds of queries quickly. Cache files are stored on your system and can grow significantly depending on codebase size.

To perform the preprocessing, execute the preprocessor:

```sh
arkanjo-preprocessor build
```

* The preprocessor will ask for the complete path to the codebase you want to analyze and the desired similarity threshold. 
* **Cache fallback:** `./tmp/arkanjo`

## Execute the tool

To execute the tool's commands, you need to run a command that follows this format:

```sh
arkanjo <command> [command_parameters] [--preprocessor] [-S <SIMILARITY>]
```

If the preprocessor has not been run yet, the tool will automatically execute it.

The parameters common to all commands are:

- `--preprocessor`: Forces the preprocessor to execute.

- `-S <SIMILARITY>`: Changes the similarity threshold to `SIMILARITY` for the current command only.

### Commands (summary)

| Command       | Purpose | Example |
|---------------|---------|---------|
| `explorer`    | Explore duplicated functions detected in the project | `arkanjo explorer -l 10 -s` |
| `function`    | Search for functions using a substring match. | `arkanjo function offset_to_id` |
| `duplication` | Analyze and report duplicated lines across the codebase, grouped by folder hierarchy. | `arkanjo duplication` |

For full details and options for each command, run:

```sh
arkanjo <command> --help
```
