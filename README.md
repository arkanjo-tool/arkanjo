Arkanjo is a CLI tool designed to help developers find code duplication within 
their codebases, specifically within the scope of functions.

The tool's current commands are:

The current functionalities of the tool are:

- Explore code duplication in a codebase, with a limited number of filters available to the user.

- Find all functions that are duplicates of a function specified by the user.

- Create a report detailing the number of duplications in the codebase, separated by folder.

Some other commands were used for the creator's master's degree, but they are not relevant to end-users.

The tool currently supports the C programming language and also supports Java with some limitations.

# Similarity

The tool currently uses the concept of similarity. A user can pass a similarity threshold to the 
tool, which is a number between 0 and 100. This threshold is used to limit what the tool 
considers a duplication.

If the threshold is set to 0, everything is considered a duplication. If the threshold is set 
to 100, only completely equal functions are considered duplications. In its current state, 
the tool provides good results with similarity thresholds around 90.

The Arkanjo tool uses the
[Duplicate Code Detection Tool](https://github.com/platisd/duplicate-code-detection-tool) 
as a subroutine to generate the similarity metrics.

# Requirements

The tool has only been tested on Ubuntu operating systems. An installation guide could be included.

# How to install

Run the following commands in the terminal to install the dependencies:

```
pip3 install --user nltk
pip3 install --user gensim
pip3 install --user astor
python3 -m nltk.downloader punkt
```

Download the source code:

```
git clone https://github.com/arkanjo-tool/arkanjo.git
```

Go to the tool folder:

```
cd arkanjo
```

Build the binary:

```
make preprocessor
make
```

The binaries will be generated in the `bin/` directory.

# How to Run

## Preprocessor

The tool is designed with heavy preprocessing, which enables it to answer different kinds of queries quickly.

To perform the preprocessing, execute the preprocessor:

```
./preprocessor
```

The preprocessor will ask for the complete path to the codebase you want to analyze and the desired 
similarity threshold.

The preprocessor can take a while to execute. Depending on the size of the codebase, it may take several 
hours or fail to run altogether.

## Execute the tool

To execute the tool's commands, you need to run a command that follows this format:

```
./exec command [command_parameters] [-pre] [-s <NUMBER>]
```

If the preprocessor has not been run yet, the tool will automatically execute it.

The parameters common to all commands are:


- `-p`: Forces the preprocessor to execute.

- `-s <NUMBER>`: Changes the similarity threshold to `NUMBER` for the current command only.

The following is a guide on how to execute the tool's current commands:

### Explore duplications

To execute the explore duplications command, run:

```
./exec ex [-l <number>] [-p <string>] [-b <'T' or 'F'>] [-c 'T' or 'F'] 
```

All of the following options are optional, and their meanings are as follows:

- `-l \<number\>`: Limits the number of results shown to the user. By default, all results are shown.

- `-p \<string\>`: Defines a pattern that function names must match to be included in the results. 
A function is considered a match if the pattern is a substring of the function's concatenated 
file path and name (e.g.,`path/to/file.c:function_name`).

- `-b <T/F>`: Determines how pattern matching applies when comparing two functions. The default value is 'F'.

	- 'T': The pattern must match both functions in the comparison.

	- 'F': The pattern must match at least one of the functions.

- `-c <T/F>`: Sets the sort order for the results. The default value is 'F'.

	- 'T': Sorts results by the number of duplicated lines.

	- 'F': Sorts results by the similarity metric.

### Find function duplicates

To execute the function command, execute:

```
./exec fu <FUNCTION_NAME>
```

`<FUNCTION_NAME>` is a parameter passed by the user. The tool will match the 
parameter with any function that has the `FUNCTION_NAME` as a substring.

### Relatory

To execute the relatory command, execute:

```
./exec du
```
